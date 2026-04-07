import React, { useState, useEffect } from 'react';
import './Controls.css';

const BRIDGE_URL = 'http://localhost:3001';

function Controls({ songs, setSongs, algorithm, setAlgorithm, setResults, loading, setLoading }) {
  const [selectedSongs, setSelectedSongs]   = useState([]);
  const [startSong, setStartSong]           = useState(null);
  const [endSong, setEndSong]               = useState(null);
  const [backendStatus, setBackendStatus]   = useState('checking'); // 'ok' | 'error' | 'checking'

  useEffect(() => {
    checkBackendAndLoadSongs();
  }, []);

  const checkBackendAndLoadSongs = async () => {
    try {
      const health = await fetch(`${BRIDGE_URL}/api/health`);
      const { binaryFound } = await health.json();

      if (!binaryFound) {
        setBackendStatus('error');
        console.error('❌ C++ binary not found. Build it first: cd backend-cpp && make');
        return;
      }

      setBackendStatus('ok');

      // Load 10 random songs from SongDatabase
      const allSongs = await fetchAllSongs();
      const shuffled = [...allSongs].sort(() => Math.random() - 0.5);
      const sample   = shuffled.slice(0, 10);
      setSongs(sample);
      setSelectedSongs(sample.map(s => s.id));
      setStartSong(sample[0]);
      setEndSong(sample[sample.length - 1]);
    } catch {
      setBackendStatus('error');
      console.error('❌ Bridge server not running. Start it: node server.js');
    }
  };

  const fetchAllSongs = async () => {
    const res  = await fetch(`${BRIDGE_URL}/api/songs`);
    const data = await res.json();
    if (data.error) throw new Error(data.error);
    console.log(`✅ Loaded ${data.songs.length} songs from C++ backend`);
    return data.songs;
  };

  const loadSampleData = async () => {
    try {
      const allSongs = await fetchAllSongs();
      const shuffled = [...allSongs].sort(() => Math.random() - 0.5);
      const sample   = shuffled.slice(0, 10);

      setSongs(sample);
      setSelectedSongs(sample.map(s => s.id));
      setStartSong(sample[0]);
      setEndSong(sample[sample.length - 1]);
      setResults(null);
    } catch (err) {
      alert(`Could not load songs from C++ backend.\n\n${err.message}\n\nMake sure:\n1. cd backend-cpp && make\n2. node server.js`);
    }
  };

  const handleFileUpload = () => {
    alert('File upload coming soon! Use "Use Sample Data" for now.');
  };

  const toggleSongSelection = (songId) => {
    setSelectedSongs(prev =>
      prev.includes(songId) ? prev.filter(id => id !== songId) : [...prev, songId]
    );
  };

  const runAlgorithm = async () => {
    if (selectedSongs.length === 0) {
      alert('Please select at least one song!');
      return;
    }

    setLoading(true);

    const request = {
      algorithm,
      songIds:     selectedSongs,
      startSongId: startSong?.id ?? selectedSongs[0],
      endSongId:   endSong?.id   ?? selectedSongs[selectedSongs.length - 1],
    };

    console.log('📤 Sending to C++ bridge:', request);

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
      console.log('✅ Results from C++ backend:', results);
      setResults(results);
    } catch (err) {
      console.error('❌ Algorithm error:', err);
      alert(`Error running C++ algorithm:\n${err.message}`);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="controls-panel">
      <div className="controls-header">
        <span className="controls-icon">⚙️</span>
        <h2>Controls</h2>
      </div>

      <div className={`backend-status ${backendStatus}`}>
        {backendStatus === 'checking' && '⏳ Connecting to C++ backend…'}
        {backendStatus === 'ok'       && '✅ C++ backend connected'}
        {backendStatus === 'error'    && (
          <>
            ❌ Backend offline —{' '}
            <code>node server.js</code> &amp; rebuild C++
          </>
        )}
      </div>

      {/* Upload Section */}
      <div className="control-section">
        <h3>1. Upload Songs</h3>
        <div className={`upload-area ${songs.length > 0 ? 'uploaded' : ''}`}>
          {songs.length === 0 ? (
            <>
              <div className="upload-icon">📁</div>
              <p>Drag &amp; drop or click to upload</p>
              <input
                type="file"
                accept=".csv,.json"
                onChange={handleFileUpload}
                style={{ display: 'none' }}
                id="file-upload"
              />
              <label htmlFor="file-upload" className="upload-label">Choose File</label>
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
          {[
            { id: 'dijkstra', icon: '🎯', title: "Dijkstra's Algorithm", desc: 'Smooth transition A → B' },
            { id: 'tsp',      icon: '🔄', title: 'Greedy TSP',           desc: 'Order selected songs'   },
            { id: 'mst',      icon: '🌳', title: "MST (Prim's)",         desc: 'Organize entire library' },
          ].map(({ id, icon, title, desc }) => (
            <div
              key={id}
              className={`algorithm-card ${algorithm === id ? 'selected' : ''}`}
              onClick={() => setAlgorithm(id)}
            >
              <span className="algo-icon">{icon}</span>
              <div>
                <h4>{title}</h4>
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

          {algorithm === 'dijkstra' && (
            <>
              <div className="parameter-group">
                <label>Start Song</label>
                <select
                  value={startSong?.id || ''}
                  onChange={e => setStartSong(songs.find(s => s.id === parseInt(e.target.value)))}
                >
                  {songs.map(song => (
                    <option key={song.id} value={song.id}>
                      {song.title} — {song.artist}
                    </option>
                  ))}
                </select>
              </div>
              <div className="parameter-group">
                <label>End Song</label>
                <select
                  value={endSong?.id || ''}
                  onChange={e => setEndSong(songs.find(s => s.id === parseInt(e.target.value)))}
                >
                  {songs.map(song => (
                    <option key={song.id} value={song.id}>
                      {song.title} — {song.artist}
                    </option>
                  ))}
                </select>
              </div>
            </>
          )}

          {algorithm === 'tsp' && (
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
            </div>
          )}

          {algorithm === 'mst' && (
            <div className="info-box">
              <span className="info-icon">ℹ️</span>
              <p>MST will connect all songs with minimum total cost</p>
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
          {loading ? (
            <><span className="spinner">⏳</span> Running C++ Algorithm…</>
          ) : (
            <><span>▶️</span> Run Algorithm</>
          )}
        </button>
      )}
    </div>
  );
}

export default Controls;