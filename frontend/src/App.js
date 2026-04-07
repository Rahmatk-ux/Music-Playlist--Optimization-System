import React, { useState } from 'react';
import './App.css';
import Controls from './components/Controls';
import Results from './components/Results';

function App() {
  const [songs, setSongs] = useState([]);
  const [algorithm, setAlgorithm] = useState('dijkstra');
  const [results, setResults] = useState(null);
  const [loading, setLoading] = useState(false);

  return (
    <div className="App">
      <header className="app-header">
        <div className="header-icon">🎵</div>
        <div className="header-content">
          <h1>Music Playlist Optimizer</h1>
          <p>Smart Playlist Generation Using Graph Algorithms</p>
        </div>
      </header>

      <div className="main-container">
        <Controls 
          songs={songs}
          setSongs={setSongs}
          algorithm={algorithm}
          setAlgorithm={setAlgorithm}
          setResults={setResults}
          loading={loading}
          setLoading={setLoading}
        />
        
        <Results 
          results={results}
          songs={songs}
          algorithm={algorithm}
        />
      </div>
    </div>
  );
}

export default App;