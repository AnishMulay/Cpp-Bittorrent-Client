#include <iostream>
#include <string>
#include "torrent_file.hpp"

int main() {
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "BitTorrent client started" << std::endl;
    
    try {
        TorrentFile torrentFile("puppy.torrent");
        torrentFile.readFile();

        std::cout << "Torrent file contents:" << std::endl;
        std::cout << torrentFile.toString() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}