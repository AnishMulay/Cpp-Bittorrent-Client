#include "tracker.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <random>
#include <stdexcept>

// Constants for the tracker protocol
constexpr int CONNECT = 0;
constexpr int ANNOUNCE = 1;
constexpr int SCRAPE = 2;
constexpr int ERROR = 3;

void Tracker::getPeers(const std::string& announceURL, 
                       const std::string& infoHash, 
                       const std::string& peerId, int port, 
                       std::function<void(const std::vector<Peer, std::allocator<Peer>>&)> callback) {
    
    // Parse the announce URL to get the host and port
    std::string hostName;
    int port;

    if (announceURL.substr(0, 6) != "udp://") {
        throw std::invalid_argument("Only UDP Trackers are supported");
    }

    size_t hostStart = 6;
    size_t hostEnd = announceURL.find(':', hostStart);
    hostName = announceURL.substr(hostStart, hostEnd - hostStart);

    size_t portStart = hostEnd + 1;
    size_t portEnd = announceURL.find('/', portStart);
    port = std::stoi(announceURL.substr(portStart, portEnd - portStart));

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    struct timeval tv;
    tv.tv_sec = 15;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv) < 0) {
        close(sockfd);
        throw std::runtime_error("Failed to set socket options");
    }

    // Send connection request
    std::vector<char> connRequest = buildConnectRequest();
    udpSend(sockfd, connRequest.data(), connRequest.size(), hostName, port);

    // Receive buffer
    char recvBuffer[2048];
    sockaddr_in serverAddr;
    socklen_t addrLen = sizeof(serverAddr);

    while(true) {
        int bytesRead = recvfrom(sockfd, recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr*)&serverAddr, &addrLen);
        if (bytesRead < 0) {
            close(sockfd);
            throw std::runtime_error("Failed to receive data");
        }

        // Parse the response
        int messageType = respType(recvBuffer, bytesRead);

        if (messageType == CONNECT) {
            // Parse the connection response
            auto connectResponse = parseConnectResponse(recvBuffer);

            // Build and send the announce request
            std::vector<char> announceRequest = buildAnnounceRequest(connectResponse.connectionId, infoHash, peerId);
            udpSend(sockfd, announceRequest.data(), announceRequest.size(), hostName, port);
        }
        else if (messageType == ANNOUNCE) {
            // Parse the announce response
            auto announceResponse = parseAnnounceResponse(recvBuffer, bytesRead);

            // Call the callback with the peers
            callback(announceResponse.peers);
            break;
        }
        else if(messageType == ERROR) {
            close(sockfd);
            throw std::runtime_error("Tracker returned an error");
        }
    }

    close(sockfd);
}