import React, { useState } from 'react';
import './Controls.css';

// Sample song database
const sampleSongs = [
  { id: 1, title: "Thunder", artist: "Imagine Dragons", bpm: 168, energy: 81 },
  { id: 2, title: "Blinding Lights", artist: "The Weeknd", bpm: 171, energy: 73 },
  { id: 3, title: "Fix You", artist: "Coldplay", bpm: 138, energy: 58 },
  { id: 4, title: "All of Me", artist: "John Legend", bpm: 120, energy: 35 },
  { id: 5, title: "Uptown Funk", artist: "Mark Ronson", bpm: 115, energy: 84 },
  { id: 6, title: "Eye of the Tiger", artist: "Survivor", bpm: 109, energy: 88 },
  { id: 7, title: "Shape of You", artist: "Ed Sheeran", bpm: 96, energy: 65 },
  { id: 8, title: "Weightless", artist: "Marconi Union", bpm: 60, energy: 26 },
  { id: 9, title: "Someone Like You", artist: "Adele", bpm: 67, energy: 24 },
  { id: 10, title: "Hallelujah", artist: "Jeff Buckley", bpm: 62, energy: 18 }
];

function Controls({ songs, setSongs, algorithm, setAlgorithm, setResults, loading, setLoading }) {
  const [selectedSongs, setSelectedSongs] = useState([]);
  const [startSong, setStartSong] = useState(null);
  const [endSong, setEndSong] = useState(null);

  // Load sample data
  const loadSampleData = () => {
    setSongs(sampleSongs);
    setSelectedSongs(sampleSongs.map(s => s.id));
    setStartSong(sampleSongs[0]);
    setEndSong(sampleSongs[sampleSongs.length - 1]);
    setResults(null);
  };

  // Handle file upload (placeholder)
  const handleFileUpload = (event) => {
    alert('File upload feature coming soon! For now, use "Use Sample Data".');
  };

  // Toggle song selection
  const toggleSongSelection = (songId) => {
    setSelectedSongs(prev => 
      prev.includes(songId) 
        ? prev.filter(id => id !== songId)
        : [...prev, songId]
    );
  };

  // Run algorithm
  const runAlgorithm = () => {
    if (selectedSongs.length === 0) {
      alert('Please select at least one song!');
      return;
    }

    setLoading(true);
    
    // Simulate algorithm execution
    setTimeout(() => {
      const selectedSongData = songs.filter(s => selectedSongs.includes(s.id));
      const optimized = optimizePlaylist(selectedSongData, algorithm, startSong, endSong);
      setResults(optimized);
      setLoading(false);
    }, 1000);
  };

  // Optimize playlist (simple mock implementation)
  const optimizePlaylist = (songList, algo, start, end) => {
    let ordered = [...songList];
    
    if (algo === 'dijkstra') {
      // Sort by similarity to start song
      if (start) {
        ordered.sort((a, b) => {
          const distA = Math.abs(a.bpm - start.bpm) + Math.abs(a.energy - start.energy);
          const distB = Math.abs(b.bpm - start.bpm) + Math.abs(b.energy - start.energy);
          return distA - distB;
        });
      }
    } else if (algo === 'tsp') {
      // Greedy nearest neighbor
      if (songList.length > 0) {
        let result = [songList[0]];
        let remaining = songList.slice(1);
        
        while (remaining.length > 0) {
          let current = result[result.length - 1];
          let nearest = remaining[0];
          let minDist = calculateDistance(current, nearest);
          
          for (let song of remaining) {
            let dist = calculateDistance(current, song);
            if (dist < minDist) {
              minDist = dist;
              nearest = song;
            }
          }
          
          result.push(nearest);
          remaining = remaining.filter(s => s.id !== nearest.id);
        }
        ordered = result;
      }
    } else if (algo === 'mst') {
      // Sort by energy for MST simulation
      ordered.sort((a, b) => a.energy - b.energy);
    }

    // Calculate transition costs
    const withCosts = ordered.map((song, idx) => {
      if (idx === 0) return { ...song, transitionCost: 0 };
      const prev = ordered[idx - 1];
      const cost = calculateDistance(prev, song);
      return { ...song, transitionCost: cost };
    });

    const totalCost = withCosts.reduce((sum, s) => sum + s.transitionCost, 0);
    const avgCost = ordered.length > 1 ? totalCost / (ordered.length - 1) : 0;

    return {
      playlist: withCosts,
      totalCost: totalCost.toFixed(1),
      avgCost: avgCost.toFixed(1),
      songCount: ordered.length
    };
  };

  const calculateDistance = (song1, song2) => {
    const bpmDiff = Math.abs(song1.bpm - song2.bpm);
    const energyDiff = Math.abs(song1.energy - song2.energy);
    return Math.sqrt(bpmDiff * bpmDiff + energyDiff * energyDiff);
  };

  return (
    <div className="controls-panel">
      <div className="controls-header">
        <span className="controls-icon">⚙️</span>
        <h2>Controls</h2>
      </div>

      {/* Upload Section */}
      <div className="control-section">
        <h3>1. Upload Songs</h3>
        <div className={`upload-area ${songs.length > 0 ? 'uploaded' : ''}`}>
          {songs.length === 0 ? (
            <>
              <div className="upload-icon">📁</div>
              <p>Drag & drop or click to upload</p>
              <input 
                type="file" 
                accept=".csv,.json" 
                onChange={handleFileUpload}
                style={{ display: 'none' }}
                id="file-upload"
              />
              <label htmlFor="file-upload" className="upload-label">
                Choose File
              </label>
            </>
          ) : (
            <>
              <div className="upload-success">✅</div>
              <h4>Songs Loaded!</h4>
              <p>✓ {songs.length} songs loaded</p>
            </>
          )}
        </div>
        <button className="sample-btn" onClick={loadSampleData}>
          Use Sample Data
        </button>
      </div>

      {/* Algorithm Selection */}
      <div className="control-section">
        <h3>2. Choose Algorithm</h3>
        <div className="algorithm-options">
          <div 
            className={`algorithm-card ${algorithm === 'dijkstra' ? 'selected' : ''}`}
            onClick={() => setAlgorithm('dijkstra')}
          >
            <span className="algo-icon">🎯</span>
            <div>
              <h4>Dijkstra's Algorithm</h4>
              <p>Smooth transition A → B</p>
            </div>
          </div>

          <div 
            className={`algorithm-card ${algorithm === 'tsp' ? 'selected' : ''}`}
            onClick={() => setAlgorithm('tsp')}
          >
            <span className="algo-icon">🔄</span>
            <div>
              <h4>Greedy TSP</h4>
              <p>Order selected songs</p>
            </div>
          </div>

          <div 
            className={`algorithm-card ${algorithm === 'mst' ? 'selected' : ''}`}
            onClick={() => setAlgorithm('mst')}
          >
            <span className="algo-icon">🌳</span>
            <div>
              <h4>MST (Prim's)</h4>
              <p>Organize entire library</p>
            </div>
          </div>
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
                  onChange={(e) => setStartSong(songs.find(s => s.id === parseInt(e.target.value)))}
                >
                  {songs.map(song => (
                    <option key={song.id} value={song.id}>
                      {song.title} - {song.artist}
                    </option>
                  ))}
                </select>
              </div>

              <div className="parameter-group">
                <label>End Song</label>
                <select 
                  value={endSong?.id || ''} 
                  onChange={(e) => setEndSong(songs.find(s => s.id === parseInt(e.target.value)))}
                >
                  {songs.map(song => (
                    <option key={song.id} value={song.id}>
                      {song.title} - {song.artist}
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
          disabled={loading}
        >
          {loading ? (
            <>
              <span className="spinner">⏳</span>
              Running...
            </>
          ) : (
            <>
              <span>▶️</span>
              Run Algorithm
            </>
          )}
        </button>
      )}
    </div>
  );
}

export default Controls;