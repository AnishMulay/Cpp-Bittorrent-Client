#ifndef TORRENT_FILE_HPP
#define TORRENT_FILE_HPP

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

class TorrentFile {
public:
    // Constructor that takes a file path as an argument
    explicit TorrentFile(const std::string& filePath);

    // Method to get the file contents
    void readFile();

    // Method to get the file data as a string
    std::string toString() const;

    // Get the announce URL for the torrent file
    std::string getAnnounceURL() const;
private:
    std::string filePath; // Path to the torrent file
    std::vector<char> fileData; // Vector to hold the file data
};

#endif