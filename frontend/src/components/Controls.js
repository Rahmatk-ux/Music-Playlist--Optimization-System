import React, { useState, useEffect } from 'react';
import './Controls.css';

const BRIDGE_URL = 'http://localhost:3001';

// All 6 algorithms with metadata
const ALGORITHMS = [
  { id: 'dijkstra', icon: '🎯', title: "Dijkstra's Algorithm",  desc: 'Smooth flow from start song',    phase: 1 },
  { id: 'tsp',      icon: '🔄', title: 'Greedy TSP',            desc: 'Visit all songs optimally',       phase: 1 },
  { id: 'twoopt',   icon: '⚡', title: '2-opt TSP Improvement', desc: 'Improved greedy TSP',             phase: 3 },
  { id: 'mst',      icon: '🌳', title: "MST (Prim's)",          desc: 'Min spanning tree playlist',      phase: 1 },
  { id: 'kruskal',  icon: '🔗', title: "MST (Kruskal's)",       desc: 'Min spanning tree via sorting',   phase: 3 },
  { id: 'astar',    icon: '⭐', title: 'A* Search',             desc: 'Optimal path start → end song',  phase: 3 },
];

function Controls({ songs, setSongs, algorithm, setAlgorithm, setResults, loading, setLoading }) {
  const [selectedSongs,  setSelectedSongs]  = useState([]);
  const [startSong,      setStartSong]      = useState(null);
  const [endSong,        setEndSong]        = useState(null);
  const [backendStatus,  setBackendStatus]  = useState('checking');

  useEffect(() => { checkBackendAndLoadSongs(); }, []);

  // ── Backend health check + auto-load ──────────────────────────────────────
  const checkBackendAndLoadSongs = async () => {
    try {
      const health = await fetch(`${BRIDGE_URL}/api/health`);
      const { binaryFound } = await health.json();
      if (!binaryFound) { setBackendStatus('error'); return; }
      setBackendStatus('ok');
      const allSongs = await fetchAllSongs();
      loadRandomSample(allSongs);
    } catch {
      setBackendStatus('error');
    }
  };

  const fetchAllSongs = async () => {
    const res  = await fetch(`${BRIDGE_URL}/api/songs`);
    const data = await res.json();
    if (data.error) throw new Error(data.error);
    return data.songs;
  };

  const loadRandomSample = (allSongs) => {
    const shuffled = [...allSongs].sort(() => Math.random() - 0.5);
    const sample   = shuffled.slice(0, 10);
    setSongs(sample);
    setSelectedSongs(sample.map(s => s.id));
    setStartSong(sample[0]);
    setEndSong(sample[sample.length - 1]);
    setResults(null);
  };

  const loadSampleData = async () => {
    try {
      const allSongs = await fetchAllSongs();
      loadRandomSample(allSongs);
    } catch (err) {
      alert(`Could not load songs: ${err.message}`);
    }
  };

  const toggleSongSelection = (songId) => {
    setSelectedSongs(prev =>
      prev.includes(songId)
        ? prev.filter(id => id !== songId)
        : [...prev, songId]
    );
  };

  // ── Run algorithm ─────────────────────────────────────────────────────────
  const runAlgorithm = async () => {
    if (selectedSongs.length === 0) { alert('Select at least one song!'); return; }
    if (algorithm === 'astar' && startSong?.id === endSong?.id) {
      alert('A* Search requires a different start and end song!');
      return;
    }

    setLoading(true);
    const request = {
      algorithm,
      songIds:     selectedSongs,
      startSongId: startSong?.id ?? selectedSongs[0],
      endSongId:   endSong?.id   ?? selectedSongs[selectedSongs.length - 1],
    };

    try {
      const res = await fetch(`${BRIDGE_URL}/api/optimize`, {
        method:  'POST',
        headers: { 'Content-Type': 'application/json' },
        body:    JSON.stringify(request),
      });
      if (!res.ok) {
        const err = await res.json();
        throw new Error(err.error || `HTTP ${res.status}`);
      }
      const results = await res.json();
      setResults(results);
    } catch (err) {
      alert(`Error running algorithm:\n${err.message}`);
    } finally {
      setLoading(false);
    }
  };

  // ── Render ────────────────────────────────────────────────────────────────
  return (
    <div className="controls-panel">
      <div className="controls-header">
        <span className="controls-icon">⚙️</span>
        <h2>Controls</h2>
      </div>

      {/* Backend status */}
      <div className={`backend-status ${backendStatus}`}>
        {backendStatus === 'checking' && '⏳ Connecting to C++ backend…'}
        {backendStatus === 'ok'       && '✅ C++ backend connected'}
        {backendStatus === 'error'    && <>❌ Backend offline — <code>node server.js</code></>}
      </div>

      {/* Upload Section */}
      <div className="control-section">
        <h3>1. Upload Songs</h3>
        <div className={`upload-area ${songs.length > 0 ? 'uploaded' : ''}`}>
          {songs.length === 0 ? (
            <>
              <div className="upload-icon">📁</div>
              <p>Click "Use Sample Data" to load songs</p>
            </>
          ) : (
            <>
              <div className="upload-success">✅</div>
              <h4>Songs Loaded!</h4>
              <p>✓ {songs.length} songs from C++ backend</p>
            </>
          )}
        </div>
        <button className="sample-btn" onClick={loadSampleData} disabled={backendStatus !== 'ok'}>
          Use Sample Data
        </button>
      </div>

      {/* Algorithm Selection */}
      <div className="control-section">
        <h3>2. Choose Algorithm</h3>
        <div className="algorithm-options">
          {ALGORITHMS.map(({ id, icon, title, desc, phase }) => (
            <div
              key={id}
              className={`algorithm-card ${algorithm === id ? 'selected' : ''} ${phase === 3 ? 'phase3' : ''}`}
              onClick={() => setAlgorithm(id)}
            >
              <span className="algo-icon">{icon}</span>
              <div>
                <h4>{title} {phase === 3}</h4>
                <p>{desc}</p>
              </div>
            </div>
          ))}
        </div>
      </div>

      {/* Parameters */}
      {songs.length > 0 && (
        <div className="control-section">
          <h3>3. Set Parameters</h3>

          {/* Dijkstra and A* — show start + end song */}
          {(algorithm === 'dijkstra' || algorithm === 'astar') && (
            <>
              <div className="parameter-group">
                <label>Start Song</label>
                <select
                  value={startSong?.id || ''}
                  onChange={e => setStartSong(songs.find(s => s.id === parseInt(e.target.value)))}
                >
                  {songs.map(song => (
                    <option key={song.id} value={song.id}>{song.title} — {song.artist}</option>
                  ))}
                </select>
              </div>
              <div className="parameter-group">
                <label>End Song {algorithm === 'astar' && '(required for A*)'}  </label>
                <select
                  value={endSong?.id || ''}
                  onChange={e => setEndSong(songs.find(s => s.id === parseInt(e.target.value)))}
                >
                  {songs.map(song => (
                    <option key={song.id} value={song.id}>{song.title} — {song.artist}</option>
                  ))}
                </select>
              </div>
            </>
          )}

          {/* TSP and 2-opt — show song checkboxes */}
          {(algorithm === 'tsp' || algorithm === 'twoopt') && (
            <div className="parameter-group">
              <label>Select Songs ({selectedSongs.length} selected)</label>
              <div className="song-selection-list">
                {songs.map(song => (
                  <div key={song.id} className="song-checkbox">
                    <input
                      type="checkbox"
                      checked={selectedSongs.includes(song.id)}
                      onChange={() => toggleSongSelection(song.id)}
                      id={`song-${song.id}`}
                    />
                    <label htmlFor={`song-${song.id}`}>
                      <strong>{song.title}</strong>
                      <span className="song-meta">{song.artist} • {song.bpm} BPM</span>
                    </label>
                  </div>
                ))}
              </div>
              {algorithm === 'twoopt' && (
                <div className="info-box" style={{ marginTop: '10px' }}>
                  <span className="info-icon">⚡</span>
                  <p>2-opt runs greedy TSP first then improves by reversing costly segments</p>
                </div>
              )}
            </div>
          )}

          {/* MST algorithms — no parameters needed */}
          {(algorithm === 'mst' || algorithm === 'kruskal') && (
            <div className="info-box">
              <span className="info-icon">ℹ️</span>
              <p>
                {algorithm === 'mst'
                  ? "Prim's MST connects all songs starting from any vertex, expanding greedily"
                  : "Kruskal's MST sorts all edges by weight and adds cheapest edges that don't form cycles"}
              </p>
            </div>
          )}
        </div>
      )}

      {/* Run Button */}
      {songs.length > 0 && (
        <button
          className="run-btn"
          onClick={runAlgorithm}
          disabled={loading || backendStatus !== 'ok'}
        >
          {loading
            ? <><span className="spinner">⏳</span> Running C++ Algorithm…</>
            : <><span>▶️</span> Run Algorithm</>
          }
        </button>
      )}
    </div>
  );
}

export default Controls;
