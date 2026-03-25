#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
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

// Function to get random sample of songs
vector<Song> getRandomSongs(int count) {
    vector<Song> allSongs = songDatabase;
    
    // Shuffle
    random_device rd;
    mt19937 g(rd());
    shuffle(allSongs.begin(), allSongs.end(), g);
    
    // Take first 'count' songs
    vector<Song> sample(allSongs.begin(), allSongs.begin() + min(count, (int)allSongs.size()));
    return sample;
}

// Function to get songs by mood
vector<Song> getSongsByMood(string mood) {
    vector<Song> filtered;
    
    for (const Song& song : songDatabase) {
        if (mood == "high" && song.energy > 0.7) {
            filtered.push_back(song);
        } else if (mood == "medium" && song.energy >= 0.4 && song.energy <= 0.7) {
            filtered.push_back(song);
        } else if (mood == "low" && song.energy < 0.4) {
            filtered.push_back(song);
        } else if (mood == "party" && song.energy > 0.7 && song.valence > 0.6) {
            filtered.push_back(song);
        } else if (mood == "chill" && song.energy < 0.5) {
            filtered.push_back(song);
        }
    }
    
    // Shuffle and take first 15
    random_device rd;
    mt19937 g(rd());
    shuffle(filtered.begin(), filtered.end(), g);
    
    if (filtered.size() > 15) {
        filtered.resize(15);
    }
    
    return filtered;
}

// Display menu
void displayMenu() {
    cout << "\n╔════════════════════════════════════════════╗" << endl;
    cout << "║   Music Playlist Optimizer (C++)          ║" << endl;
    cout << "║   Graph Algorithms Implementation          ║" << endl;
    cout << "╚════════════════════════════════════════════╝" << endl;
    cout << "\n1. Load Random Songs (10)" << endl;
    cout << "2. Load Mood-Based Playlist" << endl;
    cout << "3. Run Dijkstra's Algorithm" << endl;
    cout << "4. Run Greedy TSP" << endl;
    cout << "5. Run Prim's MST" << endl;
    cout << "6. Compare All Algorithms" << endl;
    cout << "7. Exit" << endl;
    cout << "\nEnter choice: ";
}

int main() {
    vector<Song> currentPlaylist;
    Graph* graph = nullptr;
    int startVertex = 0;
    
    cout << "\n🎵 Music Playlist Optimizer Started! 🎵" << endl;
    cout << "Total songs in database: " << songDatabase.size() << endl;
    
    while (true) {
        displayMenu();
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: {
                cout << "\nLoading 10 random songs..." << endl;
                currentPlaylist = getRandomSongs(10);
                
                cout << "\n✅ Songs Loaded:" << endl;
                for (size_t i = 0; i < currentPlaylist.size(); i++) {
                    cout << (i + 1) << ". " << currentPlaylist[i].title 
                         << " - " << currentPlaylist[i].artist << endl;
                }
                
                // Build graph
                if (graph != nullptr) delete graph;
                graph = new Graph(currentPlaylist);
                graph->buildGraph();
                
                cout << "\n✅ Graph built successfully!" << endl;
                break;
            }
            
            case 2: {
                cout << "\nChoose mood:" << endl;
                cout << "1. High Energy" << endl;
                cout << "2. Medium Energy" << endl;
                cout << "3. Low Energy" << endl;
                cout << "4. Party" << endl;
                cout << "5. Chill" << endl;
                cout << "Enter choice: ";
                
                int moodChoice;
                cin >> moodChoice;
                
                string mood;
                switch (moodChoice) {
                    case 1: mood = "high"; break;
                    case 2: mood = "medium"; break;
                    case 3: mood = "low"; break;
                    case 4: mood = "party"; break;
                    case 5: mood = "chill"; break;
                    default: mood = "medium";
                }
                
                currentPlaylist = getSongsByMood(mood);
                
                cout << "\n✅ " << currentPlaylist.size() << " songs loaded for mood: " << mood << endl;
                for (size_t i = 0; i < currentPlaylist.size(); i++) {
                    cout << (i + 1) << ". " << currentPlaylist[i].title 
                         << " - " << currentPlaylist[i].artist << endl;
                }
                
                // Build graph
                if (graph != nullptr) delete graph;
                graph = new Graph(currentPlaylist);
                graph->buildGraph();
                
                cout << "\n✅ Graph built successfully!" << endl;
                break;
            }
            
            case 3: {
                if (graph == nullptr) {
                    cout << "\n❌ Please load songs first!" << endl;
                    break;
                }
                
                cout << "\nEnter start song index (0-" << (currentPlaylist.size() - 1) << "): ";
                cin >> startVertex;
                
                if (startVertex < 0 || startVertex >= (int)currentPlaylist.size()) {
                    cout << "❌ Invalid index!" << endl;
                    break;
                }
                
                Dijkstra dijkstra(graph);
                dijkstra.printResults(startVertex);
                break;
            }
            
            case 4: {
                if (graph == nullptr) {
                    cout << "\n❌ Please load songs first!" << endl;
                    break;
                }
                
                cout << "\nEnter start song index (0-" << (currentPlaylist.size() - 1) << "): ";
                cin >> startVertex;
                
                if (startVertex < 0 || startVertex >= (int)currentPlaylist.size()) {
                    cout << "❌ Invalid index!" << endl;
                    break;
                }
                
                TSP tsp(graph);
                tsp.printResults(startVertex);
                break;
            }
            
            case 5: {
                if (graph == nullptr) {
                    cout << "\n❌ Please load songs first!" << endl;
                    break;
                }
                
                cout << "\nEnter start song index (0-" << (currentPlaylist.size() - 1) << "): ";
                cin >> startVertex;
                
                if (startVertex < 0 || startVertex >= (int)currentPlaylist.size()) {
                    cout << "❌ Invalid index!" << endl;
                    break;
                }
                
                MST mst(graph);
                mst.printResults(startVertex);
                break;
            }
            
            case 6: {
                if (graph == nullptr) {
                    cout << "\n❌ Please load songs first!" << endl;
                    break;
                }
                
                cout << "\nEnter start song index (0-" << (currentPlaylist.size() - 1) << "): ";
                cin >> startVertex;
                
                if (startVertex < 0 || startVertex >= (int)currentPlaylist.size()) {
                    cout << "❌ Invalid index!" << endl;
                    break;
                }
                
                cout << "\n" << string(80, '=') << endl;
                cout << "COMPARING ALL THREE ALGORITHMS" << endl;
                cout << string(80, '=') << endl;
                
                Dijkstra dijkstra(graph);
                dijkstra.printResults(startVertex);
                
                TSP tsp(graph);
                tsp.printResults(startVertex);
                
                MST mst(graph);
                mst.printResults(startVertex);
                
                cout << "\n" << string(80, '=') << endl;
                cout << "COMPARISON COMPLETE" << endl;
                cout << string(80, '=') << endl;
                break;
            }
            
            case 7: {
                cout << "\n👋 Thank you for using Music Playlist Optimizer!" << endl;
                if (graph != nullptr) delete graph;
                return 0;
            }
            
            default:
                cout << "\n❌ Invalid choice! Please try again." << endl;
        }
    }
    
    return 0;
}