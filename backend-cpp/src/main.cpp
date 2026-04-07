#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

#include <random>
#include <sstream>
#include "SongDatabase.h"
#include "Graph.h"
#include "Dijkstra.h"
#include "TSP.h"
#include "MST.h"

using namespace std;

vector<Song> songDatabase = {
    // High Energy / Fast BPM
    {1, "Blinding Lights", "The Weeknd", "After Hours", 171, 0.73, 0.33, "Synthpop", 2020},
    {2, "Uptown Funk", "Mark Ronson ft. Bruno Mars", "Uptown Special", 115, 0.87, 0.86, "Funk", 2014},
    {3, "Don't Stop Me Now", "Queen", "Jazz", 156, 0.92, 0.94, "Rock", 1978},
    {4, "September", "Earth Wind & Fire", "Best of Vol 1", 126, 0.85, 0.95, "Funk", 1978},
    {5, "Can't Stop the Feeling!", "Justin Timberlake", "Trolls", 113, 0.81, 0.90, "Pop", 2016},
    
    // Medium-High Energy
    {6, "Shape of You", "Ed Sheeran", "Divide", 96, 0.65, 0.93, "Pop", 2017},
    {7, "Levitating", "Dua Lipa", "Future Nostalgia", 103, 0.70, 0.92, "Disco Pop", 2020},
    {8, "Mr. Brightside", "The Killers", "Hot Fuss", 148, 0.89, 0.35, "Alternative Rock", 2003},
    {9, "Counting Stars", "OneRepublic", "Native", 122, 0.68, 0.57, "Pop Rock", 2013},
    {10, "Dynamite", "BTS", "BE", 114, 0.74, 0.86, "Disco Pop", 2020},
    
    // Medium Energy
    {11, "Thinking Out Loud", "Ed Sheeran", "x", 79, 0.44, 0.56, "Pop", 2014},
    {12, "Perfect", "Ed Sheeran", "Divide", 95, 0.45, 0.37, "Pop", 2017},
    {13, "Viva La Vida", "Coldplay", "Viva la Vida", 138, 0.56, 0.43, "Alternative Rock", 2008},
    {14, "Clocks", "Coldplay", "A Rush of Blood", 131, 0.58, 0.48, "Alternative Rock", 2002},
    {15, "Radioactive", "Imagine Dragons", "Night Visions", 136, 0.72, 0.22, "Alternative Rock", 2012},
    
    // Medium-Low Energy
    {16, "Someone Like You", "Adele", "21", 67, 0.24, 0.24, "Pop Ballad", 2011},
    {17, "The Scientist", "Coldplay", "A Rush of Blood", 75, 0.36, 0.23, "Alternative Rock", 2002},
    {18, "Fix You", "Coldplay", "X&Y", 138, 0.43, 0.22, "Alternative Rock", 2005},
    {19, "Let It Be", "The Beatles", "Let It Be", 72, 0.35, 0.45, "Rock", 1970},
    {20, "Imagine", "John Lennon", "Imagine", 76, 0.23, 0.38, "Soft Rock", 1971},
    
    // Low Energy / Calm
    {21, "Hallelujah", "Jeff Buckley", "Grace", 62, 0.18, 0.18, "Alternative Rock", 1994},
    {22, "Skinny Love", "Bon Iver", "For Emma Forever Ago", 65, 0.29, 0.15, "Indie Folk", 2007},
    {23, "Mad World", "Gary Jules", "Donnie Darko OST", 82, 0.19, 0.11, "Alternative", 2001},
    {24, "The Night We Met", "Lord Huron", "Strange Trails", 115, 0.31, 0.21, "Indie Folk", 2015},
    {25, "Tears in Heaven", "Eric Clapton", "Unplugged", 80, 0.22, 0.16, "Acoustic", 1992},
    
    // Electronic / Dance
    {26, "Wake Me Up", "Avicii", "True", 124, 0.70, 0.64, "EDM", 2013},
    {27, "Animals", "Martin Garrix", "Animals", 128, 0.88, 0.42, "Big Room House", 2013},
    {28, "Closer", "The Chainsmokers ft Halsey", "Collage", 95, 0.62, 0.60, "EDM Pop", 2016},
    {29, "Titanium", "David Guetta ft Sia", "Nothing but the Beat", 126, 0.78, 0.36, "EDM", 2011},
    {30, "Scary Monsters", "Skrillex", "Scary Monsters EP", 140, 0.93, 0.42, "Dubstep", 2010},
    
    // Hip Hop / Rap
    {31, "HUMBLE", "Kendrick Lamar", "DAMN", 150, 0.61, 0.42, "Hip Hop", 2017},
    {32, "Sicko Mode", "Travis Scott", "ASTROWORLD", 155, 0.73, 0.45, "Hip Hop", 2018},
    {33, "God's Plan", "Drake", "Scorpion", 77, 0.45, 0.36, "Hip Hop", 2018},
    {34, "Lose Yourself", "Eminem", "8 Mile OST", 171, 0.88, 0.42, "Hip Hop", 2002},
    {35, "In Da Club", "50 Cent", "Get Rich or Die Tryin", 90, 0.74, 0.82, "Hip Hop", 2003},
    
    // R&B / Soul
    {36, "Redbone", "Childish Gambino", "Awaken My Love", 158, 0.43, 0.35, "R&B", 2016},
    {37, "Earned It", "The Weeknd", "Beauty Behind Madness", 174, 0.42, 0.29, "R&B", 2015},
    {38, "Stay With Me", "Sam Smith", "In the Lonely Hour", 84, 0.31, 0.19, "Soul Pop", 2014},
    {39, "All of Me", "John Legend", "Love in the Future", 120, 0.39, 0.59, "R&B", 2013},
    {40, "Say Something", "A Great Big World", "Is There Anybody", 126, 0.20, 0.12, "Pop Ballad", 2013},
    
    // Indie / Alternative
    {41, "Take Me to Church", "Hozier", "Hozier", 129, 0.65, 0.22, "Indie Rock", 2013},
    {42, "Riptide", "Vance Joy", "Dream Your Life Away", 102, 0.67, 0.87, "Indie Pop", 2013},
    {43, "Ho Hey", "The Lumineers", "The Lumineers", 80, 0.55, 0.72, "Indie Folk", 2012},
    {44, "Little Talks", "Of Monsters and Men", "My Head Is an Animal", 103, 0.82, 0.50, "Indie Pop", 2011},
    {45, "Electric Feel", "MGMT", "Oracular Spectacular", 105, 0.74, 0.69, "Indie Rock", 2007},
    
    // Classic Rock
    {46, "Bohemian Rhapsody", "Queen", "A Night at the Opera", 72, 0.49, 0.27, "Progressive Rock", 1975},
    {47, "Stairway to Heaven", "Led Zeppelin", "Led Zeppelin IV", 82, 0.44, 0.34, "Rock", 1971},
    {48, "Hotel California", "Eagles", "Hotel California", 75, 0.48, 0.30, "Rock", 1976},
    {49, "Sweet Child O Mine", "Guns N Roses", "Appetite Destruction", 125, 0.93, 0.63, "Hard Rock", 1987},
    {50, "Smells Like Teen Spirit", "Nirvana", "Nevermind", 117, 0.91, 0.56, "Grunge", 1991}
};

void writeJSONArray(ofstream& file, const vector<int>& arr) {
    file << "[";
    for (size_t i = 0; i < arr.size(); i++) {
        file << arr[i];
        if (i < arr.size() - 1) file << ",";
    }
    file << "]";
}

void writeSongJSON(ofstream& file, const Song& song, double transitionCost = -1) {
    file << "{";
    file << "\"id\":" << song.id << ",";
    file << "\"title\":\"" << song.title << "\",";
    file << "\"artist\":\"" << song.artist << "\",";
    file << "\"album\":\"" << song.album << "\",";
    file << "\"bpm\":" << song.bpm << ",";
    file << "\"energy\":" << (int)(song.energy * 100) << ",";
    file << "\"valence\":" << song.valence << ",";
    file << "\"genre\":\"" << song.genre << "\",";
    file << "\"year\":" << song.year;
    if (transitionCost >= 0) {
        file << ",\"transitionCost\":" << fixed << setprecision(1) << transitionCost;
    }
    file << "}";
}

void exportAllSongsJSON(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not create file " << filename << endl;
        return;
    }

    file << "{\"songs\":[";
    for (size_t i = 0; i < songDatabase.size(); i++) {
        writeSongJSON(file, songDatabase[i]);
        if (i < songDatabase.size() - 1) file << ",";
    }
    file << "]}";

    file.close();
    cout << "✅ Exported " << songDatabase.size() << " songs to " << filename << endl;
}

vector<int> readSongIdsFromJSON(const string& filename) {
    vector<int> ids;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        return ids;
    }

    string line;
    bool inSongIds = false;
    
    while (getline(file, line)) {
        if (line.find("\"songIds\"") != string::npos) {
            inSongIds = true;
            size_t start = line.find('[');
            size_t end = line.find(']');
            if (start != string::npos && end != string::npos) {
                string arrayContent = line.substr(start + 1, end - start - 1);
                stringstream ss(arrayContent);
                string item;
                while (getline(ss, item, ',')) {
                    // Remove whitespace
                    item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
                    if (!item.empty()) {
                        ids.push_back(stoi(item));
                    }
                }
            }
            break;
        }
    }

    file.close();
    return ids;
}

struct RequestParams {
    string algorithm;
    vector<int> songIds;
    int startSongId;
    int endSongId;
};

RequestParams readRequestJSON(const string& filename) {
    RequestParams params;
    params.algorithm = "dijkstra";
    params.startSongId = 1;
    params.endSongId = 1;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        return params;
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    size_t algPos = content.find("\"algorithm\"");
    if (algPos != string::npos) {
        size_t q1 = content.find("\"", algPos + 11);
        size_t q2 = content.find("\"", q1 + 1);
        if (q1 != string::npos && q2 != string::npos)
            params.algorithm = content.substr(q1 + 1, q2 - q1 - 1);
    }

    size_t arrStart = content.find("\"songIds\"");
    if (arrStart != string::npos) {
        size_t bracketOpen  = content.find("[", arrStart);
        size_t bracketClose = content.find("]", bracketOpen);
        if (bracketOpen != string::npos && bracketClose != string::npos) {
            string arrayContent = content.substr(bracketOpen + 1, bracketClose - bracketOpen - 1);
            stringstream ss(arrayContent);
            string item;
            while (getline(ss, item, ',')) {
                item.erase(remove_if(item.begin(), item.end(), ::isspace), item.end());
                if (!item.empty())
                    params.songIds.push_back(stoi(item));
            }
        }
    }

    auto parseIntField = [&](const string& fieldName) -> int {
        size_t pos = content.find("\"" + fieldName + "\"");
        if (pos == string::npos) return 1;
        size_t colon = content.find(":", pos);
        if (colon == string::npos) return 1;
        // Extract digits only
        string numStr = "";
        for (size_t i = colon + 1; i < content.size(); i++) {
            char c = content[i];
            if (isdigit(c)) numStr += c;
            else if (!numStr.empty()) break;
        }
        return numStr.empty() ? 1 : stoi(numStr);
    };

    params.startSongId = parseIntField("startSongId");
    params.endSongId   = parseIntField("endSongId");

    return params;
}

void runAlgorithmAndExport(const RequestParams& params, const string& outputFile) {
    // Get selected songs
    vector<Song> selectedSongs;
    for (int id : params.songIds) {
        for (const Song& song : songDatabase) {
            if (song.id == id) {
                selectedSongs.push_back(song);
                break;
            }
        }
    }

    if (selectedSongs.empty()) {
        cerr << "Error: No songs selected!" << endl;
        return;
    }

    cout << "\n🎵 Running " << params.algorithm << " algorithm..." << endl;
    cout << "Selected songs: " << selectedSongs.size() << endl;

    // Build graph
    Graph graph(selectedSongs);
    graph.buildGraph();

    int startVertex = 0;
    for (size_t i = 0; i < selectedSongs.size(); i++) {
        if (selectedSongs[i].id == params.startSongId) {
            startVertex = i;
            break;
        }
    }

    vector<int> resultOrder;
    double totalCost = 0.0;

    if (params.algorithm == "dijkstra") {
        Dijkstra dijkstra(&graph);
        resultOrder = dijkstra.getOptimizedPath(startVertex);
        totalCost = dijkstra.getTotalDistance(resultOrder);
    } else if (params.algorithm == "tsp") {
        TSP tsp(&graph);
        resultOrder = tsp.greedyTSP(startVertex);
        totalCost = tsp.getTourCost(resultOrder);
    } else if (params.algorithm == "mst") {
        MST mst(&graph);
        vector<Edge> edges = mst.primMST(startVertex);
        resultOrder = mst.mstToPlaylist(startVertex);
        totalCost = mst.getTotalWeight(edges);
    }
    
    ofstream file(outputFile);
    if (!file.is_open()) {
        cerr << "Error: Could not create output file!" << endl;
        return;
    }

    file << "{" << endl;
    file << "  \"algorithm\":\"" << params.algorithm << "\"," << endl;
    file << "  \"songCount\":" << selectedSongs.size() << "," << endl;
    file << "  \"totalCost\":" << fixed << setprecision(1) << totalCost << "," << endl;
    file << "  \"avgCost\":" << fixed << setprecision(1) << (totalCost / (selectedSongs.size() - 1)) << "," << endl;
    file << "  \"playlist\":[" << endl;

    for (size_t i = 0; i < resultOrder.size(); i++) {
        int idx = resultOrder[i];
        Song song = selectedSongs[idx];
        
        double transitionCost = 0.0;
        if (i > 0) {
            int prevIdx = resultOrder[i - 1];
            transitionCost = graph.getWeight(prevIdx, idx);
        }

        file << "    ";
        writeSongJSON(file, song, transitionCost);
        if (i < resultOrder.size() - 1) file << ",";
        file << endl;
    }

    file << "  ]" << endl;
    file << "}" << endl;

    file.close();
    cout << "✅ Results exported to " << outputFile << endl;
    cout << "Total Cost: " << fixed << setprecision(1) << totalCost << endl;
}

int main(int argc, char* argv[]) {
    cout << "\n🎵 Music Playlist Optimizer (C++ Backend) 🎵\n" << endl;
    cout << "Total songs in database: " << songDatabase.size() << endl;

    if (argc < 2) {
        cout << "\nUsage:" << endl;
        cout << "  " << argv[0] << " export          - Export all songs to JSON" << endl;
        cout << "  " << argv[0] << " run <input>     - Run algorithm from input JSON" << endl;
        return 1;
    }

    string command = argv[1];

    if (command == "export") {
        string outputFile = argc > 2 ? argv[2] : "songs.json";
        exportAllSongsJSON(outputFile);
    } 
    else if (command == "run") {
        if (argc < 3) {
            cerr << "Error: Please provide input JSON file!" << endl;
            return 1;
        }

        string inputFile = argv[2];
        string outputFile = argc > 3 ? argv[3] : "output.json";

        RequestParams params = readRequestJSON(inputFile);
        runAlgorithmAndExport(params, outputFile);
    }
    else {
        cerr << "Unknown command: " << command << endl;
        return 1;
    }

    return 0;
}