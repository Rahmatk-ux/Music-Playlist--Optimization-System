import React, { useState, useEffect, useRef } from 'react';
import './Results.css';

function Results({ results, songs, algorithm }) {
  const [activeTab, setActiveTab] = useState('playlist');
  const graphRef  = useRef(null);
  const chartRef  = useRef(null);

  // ── D3 graph — draws when results change and graph tab is active ───────────
  useEffect(() => {
    if (activeTab !== 'graph' || !results || !window.d3) return;
    drawD3Graph();
  }, [results, activeTab]);

  // ── BPM chart — draws when analytics tab is active ────────────────────────
  useEffect(() => {
    if (activeTab !== 'analytics' || !results || !window.d3) return;
    drawBPMChart();
  }, [results, activeTab]);

  function drawD3Graph() {
    const d3     = window.d3;
    const el     = graphRef.current;
    if (!el) return;
    d3.select(el).selectAll('*').remove();

    const width  = el.clientWidth  || 600;
    const height = el.clientHeight || 400;

    const svg = d3.select(el)
      .append('svg')
      .attr('width', width)
      .attr('height', height);

    const playlist = results.playlist;

    // Nodes
    const nodes = playlist.map((song, i) => ({
      id: song.id, title: song.title, bpm: song.bpm,
      energy: song.energy, index: i
    }));

    // Edges — connect consecutive songs in playlist
    const links = [];
    for (let i = 0; i < playlist.length - 1; i++) {
      links.push({
        source: i, target: i + 1,
        cost: playlist[i + 1].transitionCost || 0
      });
    }

    const maxCost = Math.max(...links.map(l => l.cost), 1);

    // Color scale for edges
    const edgeColor = (cost) => {
      if (cost < 30) return '#4caf50';
      if (cost < 50) return '#ffa726';
      return '#f44336';
    };

    // Force simulation
    const simulation = d3.forceSimulation(nodes)
      .force('link',   d3.forceLink(links).id(d => d.index).distance(80))
      .force('charge', d3.forceManyBody().strength(-200))
      .force('center', d3.forceCenter(width / 2, height / 2));

    // Draw edges
    const link = svg.append('g')
      .selectAll('line')
      .data(links)
      .join('line')
      .attr('stroke',       d => edgeColor(d.cost))
      .attr('stroke-width', d => Math.max(1, 4 - (d.cost / maxCost) * 3))
      .attr('stroke-opacity', 0.8);

    // Edge cost labels
    const edgeLabel = svg.append('g')
      .selectAll('text')
      .data(links)
      .join('text')
      .attr('fill',      '#666')
      .attr('font-size', '9px')
      .attr('text-anchor', 'middle')
      .text(d => d.cost.toFixed(1));

    // Draw nodes
    const node = svg.append('g')
      .selectAll('circle')
      .data(nodes)
      .join('circle')
      .attr('r',    18)
      .attr('fill', d => `hsl(${220 + d.index * 15}, 60%, 55%)`)
      .attr('stroke', '#fff')
      .attr('stroke-width', 2)
      .call(d3.drag()
        .on('start', (event, d) => { if (!event.active) simulation.alphaTarget(0.3).restart(); d.fx = d.x; d.fy = d.y; })
        .on('drag',  (event, d) => { d.fx = event.x; d.fy = event.y; })
        .on('end',   (event, d) => { if (!event.active) simulation.alphaTarget(0); d.fx = null; d.fy = null; })
      );

    // Node number labels
    svg.append('g')
      .selectAll('text')
      .data(nodes)
      .join('text')
      .attr('fill',        '#fff')
      .attr('font-size',   '11px')
      .attr('font-weight', 'bold')
      .attr('text-anchor', 'middle')
      .attr('dominant-baseline', 'central')
      .text(d => d.index + 1);

    // Node title tooltip on hover
    node.append('title').text(d => `${d.title}\n${d.bpm} BPM • Energy: ${d.energy}%`);

    simulation.on('tick', () => {
      link
        .attr('x1', d => d.source.x).attr('y1', d => d.source.y)
        .attr('x2', d => d.target.x).attr('y2', d => d.target.y);
      edgeLabel
        .attr('x', d => (d.source.x + d.target.x) / 2)
        .attr('y', d => (d.source.y + d.target.y) / 2);
      node
        .attr('cx', d => Math.max(20, Math.min(width - 20,  d.x)))
        .attr('cy', d => Math.max(20, Math.min(height - 20, d.y)));
      svg.selectAll('text').filter((d, i, nodes) => nodes[i].parentNode.tagName === 'g')
        .attr('x', d => d.x).attr('y', d => d.y);
    });
  }

  function drawBPMChart() {
    const d3  = window.d3;
    const el  = chartRef.current;
    if (!el) return;
    d3.select(el).selectAll('*').remove();

    const margin = { top: 20, right: 20, bottom: 60, left: 50 };
    const width  = (el.clientWidth  || 560) - margin.left - margin.right;
    const height = 200 - margin.top - margin.bottom;

    const svg = d3.select(el).append('svg')
      .attr('width',  width  + margin.left + margin.right)
      .attr('height', height + margin.top  + margin.bottom)
      .append('g')
      .attr('transform', `translate(${margin.left},${margin.top})`);

    const data = results.playlist;

    const x = d3.scaleLinear().domain([0, data.length - 1]).range([0, width]);
    const y = d3.scaleLinear().domain([50, 200]).range([height, 0]);

    // BPM line
    const line = d3.line()
      .x((d, i) => x(i))
      .y(d => y(d.bpm))
      .curve(d3.curveCatmullRom);

    svg.append('path')
      .datum(data)
      .attr('fill', 'none')
      .attr('stroke', '#667eea')
      .attr('stroke-width', 2)
      .attr('d', line);

    // Dots
    svg.selectAll('.dot')
      .data(data)
      .join('circle')
      .attr('class', 'dot')
      .attr('cx', (d, i) => x(i))
      .attr('cy', d => y(d.bpm))
      .attr('r', 4)
      .attr('fill', '#667eea');

    // Axes
    svg.append('g').attr('transform', `translate(0,${height})`).call(
      d3.axisBottom(x).ticks(data.length).tickFormat(i => data[i] ? data[i].title.substring(0, 8) + '…' : '')
    ).selectAll('text').attr('transform', 'rotate(-30)').style('text-anchor', 'end').style('font-size', '9px');

    svg.append('g').call(d3.axisLeft(y).ticks(5));

    svg.append('text')
      .attr('x', width / 2).attr('y', -5)
      .attr('text-anchor', 'middle')
      .style('font-size', '12px').style('fill', '#666')
      .text('BPM Flow Through Playlist');
  }

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
          <p>Select songs and run an algorithm to see results</p>
        </div>
      </div>
    );
  }

  return (
    <div className="results-panel">
      <div className="results-header">
        <span className="results-icon">📊</span>
        <h2>Results — {getAlgorithmName(algorithm)}</h2>
      </div>

      {/* Summary cards */}
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
        {[
          { id: 'playlist',  label: '📋 Playlist'   },
          { id: 'graph',     label: '🔗 Graph'       },
          { id: 'analytics', label: '📈 Analytics'  },
          { id: 'export',    label: '💾 Export'      },
        ].map(tab => (
          <button
            key={tab.id}
            className={`tab ${activeTab === tab.id ? 'active' : ''}`}
            onClick={() => setActiveTab(tab.id)}
          >
            {tab.label}
          </button>
        ))}
      </div>

      {/* Playlist tab */}
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

      {/* Graph tab — D3 force-directed */}
      {activeTab === 'graph' && (
        <div className="graph-view">
          <p className="graph-note">
            Drag nodes to rearrange. Edge color: <span style={{color:'#4caf50'}}>green</span> = smooth, <span style={{color:'#ffa726'}}>yellow</span> = moderate, <span style={{color:'#f44336'}}>red</span> = jarring
          </p>
          <div ref={graphRef} className="d3-graph-container" style={{ width: '100%', height: '380px' }}></div>
          {!window.d3 && (
            <div className="graph-placeholder">
              <p>D3.js not loaded. Add to index.html: <code>&lt;script src="https://d3js.org/d3.v7.min.js"&gt;&lt;/script&gt;</code></p>
            </div>
          )}
        </div>
      )}

      {/* Analytics tab */}
      {activeTab === 'analytics' && (
        <div className="analytics-view">
          <div ref={chartRef} className="bpm-chart"></div>
          <div className="analytics-stats">
            <div className="stat-row">
              <span className="stat-label">Algorithm</span>
              <span className="stat-value">{getAlgorithmName(algorithm)}</span>
            </div>
            <div className="stat-row">
              <span className="stat-label">Highest BPM transition</span>
              <span className="stat-value">
                {Math.max(...results.playlist.slice(1).map(s => s.transitionCost)).toFixed(1)}
              </span>
            </div>
            <div className="stat-row">
              <span className="stat-label">Lowest BPM transition</span>
              <span className="stat-value">
                {Math.min(...results.playlist.slice(1).map(s => s.transitionCost)).toFixed(1)}
              </span>
            </div>
            <div className="stat-row">
              <span className="stat-label">Smooth transitions (under 30)</span>
              <span className="stat-value good">
                {results.playlist.slice(1).filter(s => s.transitionCost < 30).length}
              </span>
            </div>
            <div className="stat-row">
              <span className="stat-label">Harsh transitions (over 50)</span>
              <span className="stat-value high">
                {results.playlist.slice(1).filter(s => s.transitionCost > 50).length}
              </span>
            </div>
          </div>
        </div>
      )}

      {/* Export tab */}
      {activeTab === 'export' && (
        <div className="export-view">
          <div className="export-options">
            <button className="export-btn" onClick={() => exportJSON()}>
              💾 Download JSON
            </button>
            <button className="export-btn" onClick={() => exportCSV()}>
              📊 Download CSV
            </button>
          </div>
          <pre className="export-preview">
            {JSON.stringify({ algorithm, ...results }, null, 2).substring(0, 500)}...
          </pre>
        </div>
      )}
    </div>
  );

  function exportJSON() {
    const data = JSON.stringify({ algorithm, ...results }, null, 2);
    const blob = new Blob([data], { type: 'application/json' });
    const url  = URL.createObjectURL(blob);
    const a    = document.createElement('a');
    a.href     = url;
    a.download = `playlist_${algorithm}.json`;
    a.click();
    URL.revokeObjectURL(url);
  }

  function exportCSV() {
    const rows = ['Position,Title,Artist,BPM,Energy,TransitionCost'];
    results.playlist.forEach((song, i) => {
      rows.push(`${i+1},"${song.title}","${song.artist}",${song.bpm},${song.energy},${song.transitionCost || 0}`);
    });
    const blob = new Blob([rows.join('\n')], { type: 'text/csv' });
    const url  = URL.createObjectURL(blob);
    const a    = document.createElement('a');
    a.href     = url;
    a.download = `playlist_${algorithm}.csv`;
    a.click();
    URL.revokeObjectURL(url);
  }
}

function getCostClass(cost) {
  if (cost < 30) return 'good';
  if (cost < 50) return 'medium';
  return 'high';
}

function getAlgorithmName(algo) {
  const names = {
    dijkstra: "Dijkstra's Shortest Path",
    tsp:      'Greedy TSP',
    twoopt:   '2-opt TSP Improvement',
    mst:      "Prim's MST",
    kruskal:  "Kruskal's MST",
    astar:    'A* Search'
  };
  return names[algo] || algo;
}

export default Results;
