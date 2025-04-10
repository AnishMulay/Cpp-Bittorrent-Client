#include "torrent_file.hpp"
#include <iostream>
#include <iterator>

TorrentFile::TorrentFile(const std::string& filePath)
    : filePath(filePath) {
    readFile();
}

void TorrentFile::readFile() {
    std::ifstream file(filePath, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    // Find file size
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Resize vector to hold file data
    fileData.resize(static_cast<size_t>(size));

    // Read file data into vector
    if (!file.read(fileData.data(), size)) {
        throw std::runtime_error("Error reading file: " + filePath);
    }
}

std::string TorrentFile::toString() const {
    return std::string(fileData.begin(), fileData.end());
}