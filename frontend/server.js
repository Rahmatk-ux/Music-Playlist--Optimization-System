const express = require('express');
const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');
const cors = require('cors');

const app = express();
app.use(cors());
app.use(express.json());

// Paths — matched to your project structure:
//   MUSIC-PLAY.../
//     backend-cpp/
//       playlist_optimizer.exe   ← binary here
//       src/
//     frontend/
//       server.js                ← this file
//       public/
const CPP_BINARY  = path.resolve(__dirname, '../backend-cpp/playlist_optimizer.exe');
const INPUT_FILE  = path.resolve(__dirname, 'input.json');
const OUTPUT_FILE = path.resolve(__dirname, 'output.json');
const SONGS_FILE  = path.resolve(__dirname, 'public/songs.json');

// ─── GET /api/songs ───────────────────────────────────────────────────────────
// Export all songs from C++ and return them to the frontend.
app.get('/api/songs', (req, res) => {
  const cmd = `"${CPP_BINARY}" export "${SONGS_FILE}"`;

  exec(cmd, (error, stdout, stderr) => {
    if (error) {
      console.error('❌ C++ export error:', stderr || error.message);
      return res.status(500).json({ error: `C++ binary failed: ${stderr || error.message}` });
    }

    console.log('C++ stdout:', stdout.trim());

    try {
      const data = JSON.parse(fs.readFileSync(SONGS_FILE, 'utf8'));
      res.json(data);
    } catch (parseErr) {
      res.status(500).json({ error: 'Failed to read songs.json after export' });
    }
  });
});

// ─── POST /api/optimize ───────────────────────────────────────────────────────
// Body: { algorithm, songIds, startSongId, endSongId }
// Writes input.json → calls C++ → reads output.json → returns result.
app.post('/api/optimize', (req, res) => {
  const { algorithm, songIds, startSongId, endSongId } = req.body;

  if (!algorithm || !Array.isArray(songIds) || songIds.length === 0) {
    return res.status(400).json({ error: 'Invalid request body' });
  }

  // 1. Write input.json for the C++ binary
  const inputPayload = {
    algorithm,
    songIds,
    startSongId: startSongId ?? songIds[0],
    endSongId:   endSongId   ?? songIds[songIds.length - 1],
  };

  try {
    fs.writeFileSync(INPUT_FILE, JSON.stringify(inputPayload));
  } catch (writeErr) {
    return res.status(500).json({ error: `Could not write input.json: ${writeErr.message}` });
  }

  // 2. Execute C++ binary
  const cmd = `"${CPP_BINARY}" run "${INPUT_FILE}" "${OUTPUT_FILE}"`;
  console.log(`\n▶  Running: ${cmd}`);

  exec(cmd, { timeout: 30000 }, (error, stdout, stderr) => {
    console.log('C++ stdout:', stdout.trim());
    if (stderr) console.error('C++ stderr:', stderr.trim());
    

    if (error) {
      return res.status(500).json({ error: `C++ binary failed: ${stderr || error.message}` });
    }

    // 3. Read and return output.json
    try {
      const result = JSON.parse(fs.readFileSync(OUTPUT_FILE, 'utf8'));
      res.json(result);
    } catch (parseErr) {
      res.status(500).json({ error: 'Failed to parse output.json from C++ binary' });
    }
  });
});

// ─── Health check ─────────────────────────────────────────────────────────────
app.get('/api/health', (req, res) => {
  const binaryExists = fs.existsSync(CPP_BINARY);
  res.json({
    status: 'ok',
    binaryFound: binaryExists,
    binaryPath: CPP_BINARY,
  });
});



const PORT = process.env.PORT || 3001;
app.listen(PORT, () => {
  console.log(`\n🎵 Bridge server running on http://localhost:${PORT}`);
  console.log(`   C++ binary : ${CPP_BINARY}`);
  console.log(`   Input file : ${INPUT_FILE}`);
  console.log(`   Output file: ${OUTPUT_FILE}\n`);
});