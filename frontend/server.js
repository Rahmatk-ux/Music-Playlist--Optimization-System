// server.js — Phase 3 Bridge
// New algorithms: twoopt, kruskal, astar
const express = require('express');
const { exec } = require('child_process');
const fs       = require('fs');
const path     = require('path');
const cors     = require('cors');

const app = express();
app.use(cors());
app.use(express.json());

const CPP_BINARY  = path.resolve(__dirname, '../backend-cpp/playlist_optimizer.exe');
const INPUT_FILE  = path.resolve(__dirname, 'input.json');
const OUTPUT_FILE = path.resolve(__dirname, 'output.json');
const SONGS_FILE  = path.resolve(__dirname, 'public/songs.json');

// Valid algorithms — Phase 3 adds twoopt, kruskal, astar
const VALID_ALGORITHMS = ['dijkstra', 'tsp', 'twoopt', 'mst', 'kruskal', 'astar'];

// ── GET /api/health ───────────────────────────────────────────────────────────
app.get('/api/health', (req, res) => {
    const binaryExists = fs.existsSync(CPP_BINARY);
    res.json({
        status:      'ok',
        binaryFound: binaryExists,
        binaryPath:  CPP_BINARY,
        algorithms:  VALID_ALGORITHMS
    });
});

// ── GET /api/songs ────────────────────────────────────────────────────────────
app.get('/api/songs', (req, res) => {
    const cmd = `"${CPP_BINARY}" export "${SONGS_FILE}"`;
    exec(cmd, (error, stdout, stderr) => {
        if (error) {
            console.error('Export error:', stderr || error.message);
            return res.status(500).json({ error: `C++ binary failed: ${stderr || error.message}` });
        }
        try {
            const data = JSON.parse(fs.readFileSync(SONGS_FILE, 'utf8'));
            console.log(`Loaded ${data.songs.length} songs`);
            res.json(data);
        } catch (e) {
            res.status(500).json({ error: 'Failed to read songs.json' });
        }
    });
});

// ── POST /api/optimize ────────────────────────────────────────────────────────
app.post('/api/optimize', (req, res) => {
    const { algorithm, songIds, startSongId, endSongId } = req.body;

    // Validation
    if (!algorithm || !Array.isArray(songIds) || songIds.length === 0) {
        return res.status(400).json({ error: 'Invalid request body' });
    }
    if (!VALID_ALGORITHMS.includes(algorithm)) {
        return res.status(400).json({ error: `Unknown algorithm: ${algorithm}. Valid: ${VALID_ALGORITHMS.join(', ')}` });
    }
    if (algorithm === 'astar' && startSongId === endSongId) {
        return res.status(400).json({ error: 'A* requires different start and end songs' });
    }

    // Write input.json — NO pretty printing (breaks C++ parser)
    const inputPayload = {
        algorithm,
        songIds,
        startSongId: startSongId ?? songIds[0],
        endSongId:   endSongId   ?? songIds[songIds.length - 1]
    };

    try {
        fs.writeFileSync(INPUT_FILE, JSON.stringify(inputPayload));
    } catch (e) {
        return res.status(500).json({ error: `Cannot write input.json: ${e.message}` });
    }

    // Run C++ binary
    const cmd = `"${CPP_BINARY}" run "${INPUT_FILE}" "${OUTPUT_FILE}"`;
    console.log(`Running: ${algorithm} on ${songIds.length} songs`);

    exec(cmd, { timeout: 30000 }, (error, stdout, stderr) => {
        console.log('stdout:', stdout.trim());
        if (stderr) console.error('stderr:', stderr.trim());

        if (error) {
            return res.status(500).json({ error: `C++ failed: ${stderr || error.message}` });
        }

        try {
            const raw    = fs.readFileSync(OUTPUT_FILE, 'utf8');
            if (!raw || raw.trim() === '') {
                return res.status(500).json({ error: 'C++ wrote empty output' });
            }
            const result = JSON.parse(raw);
            res.json(result);
        } catch (e) {
            console.error('Parse error:', e.message);
            res.status(500).json({ error: 'Failed to parse output.json from C++ binary' });
        }
    });
});

// ── POST /api/compare ─────────────────────────────────────────────────────────
// Run ALL algorithms on the same songs and return results for dashboard
app.post('/api/compare', async (req, res) => {
    const { songIds, startSongId, endSongId } = req.body;
    if (!Array.isArray(songIds) || songIds.length === 0) {
        return res.status(400).json({ error: 'Invalid songIds' });
    }

    const results = {};
    const algos   = ['dijkstra', 'tsp', 'twoopt', 'mst', 'kruskal'];
    // Note: astar excluded from compare (needs different start/end)

    const runOne = (algo) => new Promise((resolve) => {
        const payload = { algorithm: algo, songIds, startSongId, endSongId };
        fs.writeFileSync(INPUT_FILE, JSON.stringify(payload));
        const cmd = `"${CPP_BINARY}" run "${INPUT_FILE}" "${OUTPUT_FILE}"`;
        exec(cmd, { timeout: 30000 }, (err, stdout) => {
            if (err) { resolve({ algorithm: algo, error: err.message }); return; }
            try {
                const data = JSON.parse(fs.readFileSync(OUTPUT_FILE, 'utf8'));
                resolve(data);
            } catch (e) {
                resolve({ algorithm: algo, error: 'Parse failed' });
            }
        });
    });

    try {
        // Run sequentially to avoid file conflicts
        for (const algo of algos) {
            results[algo] = await runOne(algo);
        }
        res.json({ comparison: results });
    } catch (e) {
        res.status(500).json({ error: e.message });
    }
});

const PORT = process.env.PORT || 3001;
app.listen(PORT, () => {
    console.log(`\nBridge server running on http://localhost:${PORT}`);
    console.log(`Binary: ${CPP_BINARY}`);
    console.log(`Algorithms: ${VALID_ALGORITHMS.join(', ')}\n`);
});
