import React, { useState } from 'react';
import './Results.css';

function Results({ results, songs, algorithm }) {
  const [activeTab, setActiveTab] = useState('playlist');

  if (!results) {
    return (
      <div className="results-panel">
        <div className="results-header">
          <span className="results-icon">📊</span>
          <h2>Results</h2>
        </div>
        <div className="no-results">
          <div className="no-results-icon">🎵</div>
          <h3>No Results Yet</h3>
          <p>Upload songs and run an algorithm to see results</p>
        </div>
      </div>
    );
  }

  return (
    <div className="results-panel">
      <div className="results-header">
        <span className="results-icon">📊</span>
        <h2>Results</h2>
      </div>

      {/* Summary Cards */}
      <div className="summary-cards">
        <div className="summary-card">
          <div className="card-value">{results.songCount}</div>
          <div className="card-label">Songs</div>
        </div>
        <div className="summary-card">
          <div className="card-value">{results.totalCost}</div>
          <div className="card-label">Total Cost</div>
        </div>
        <div className="summary-card">
          <div className="card-value">{results.avgCost}</div>
          <div className="card-label">Avg Cost</div>
        </div>
      </div>

      {/* Tabs */}
      <div className="results-tabs">
        <button 
          className={`tab ${activeTab === 'playlist' ? 'active' : ''}`}
          onClick={() => setActiveTab('playlist')}
        >
          <span>📋</span> Playlist
        </button>
        <button 
          className={`tab ${activeTab === 'graph' ? 'active' : ''}`}
          onClick={() => setActiveTab('graph')}
        >
          <span>🔗</span> Graph
        </button>
      </div>

      {/* Tab Content */}
      {activeTab === 'playlist' && (
        <div className="playlist-view">
          {results.playlist.map((song, index) => (
            <div key={song.id} className="song-card">
              <div className="song-number">{index + 1}</div>
              <div className="song-details">
                <h4>{song.title}</h4>
                <p className="song-artist">{song.artist} • {song.bpm} BPM • Energy: {song.energy}%</p>
              </div>
              {index > 0 && (
                <div className="transition-cost">
                  <span className="cost-label">↓ Transition Cost:</span>
                  <span className={`cost-value ${getCostClass(song.transitionCost)}`}>
                    {song.transitionCost.toFixed(1)}
                  </span>
                </div>
              )}
            </div>
          ))}
        </div>
      )}

      {activeTab === 'graph' && (
        <div className="graph-view">
          <div className="graph-placeholder">
            <div className="graph-icon">📊</div>
            <h3>Graph Visualization</h3>
            <p>Graph View is set for 3rd Phase</p>
            <div className="graph-info">
              <div className="info-item">
                <strong>Vertices:</strong> {results.songCount} songs
              </div>
              <div className="info-item">
                <strong>Algorithm:</strong> {getAlgorithmName(algorithm)}
              </div>
              <div className="info-item">
                <strong>Optimization:</strong> Minimum transition cost
              </div>
            </div>
          </div>
        </div>
      )}
    </div>
  );
}

function getCostClass(cost) {
  if (cost < 30) return 'good';
  if (cost < 50) return 'medium';
  return 'high';
}

function getAlgorithmName(algo) {
  const names = {
    'dijkstra': "Dijkstra's Shortest Path",
    'tsp': 'Greedy TSP (Nearest Neighbor)',
    'mst': "Prim's Minimum Spanning Tree"
  };
  return names[algo] || algo;
}

export default Results;