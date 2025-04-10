#include "torrent_file.hpp"
#include "bencode.hpp"
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

std::string TorrentFile::getAnnounceURL() const {
    // Decode the bencoded data to find the announce URL
    bencode::data decoded = bencode::decode(fileData.data(), fileData.size());

    const auto& dict = std::get<bencode::dict>(decoded);

    auto it = dict.find("announce");
    if(it == dict.end()) {
        throw std::runtime_error("Announce URL not found in torrent file");
    }

    const auto& url = std::get<std::string>(it->second);
    return url;
}