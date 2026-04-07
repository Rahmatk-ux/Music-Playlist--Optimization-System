#ifndef SONGDATABASE_H
#define SONGDATABASE_H

#include <string>
#include <vector>

using namespace std;

struct Song {
    int id;
    string title;
    string artist;
    string album;
    int bpm;
    double energy;
    double valence;
    string genre;
    int year;
};

extern vector<Song> songDatabase; 


#endif