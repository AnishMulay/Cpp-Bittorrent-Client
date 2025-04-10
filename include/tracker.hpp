#ifndef TRACKER_HPP
#define TRACKER_HPP

#include <string>
#include <vector>
#include <functional>
#include <cstdint>
#include <netinet/in.h>

struct Peer {
    std::string ip;
    int port;
};

class Tracker {
public:
    static void getPeers(const std::string& announceURL, 
                         const std::string& infoHash, 
                         const std::string& peerId, int port,
                         std::function<void(const std::vector<Peer>&)> callback);
private:
    // Helper functions for UDP communication
    static void udpSend(int socket, const char* message, int length, const std::string& hostName, int port);

    // Helper functions for parsing tracker responses and getting message type
    static int respType(const char* response, int size);

    // Message building functions
    static std::vector<char> buildConnectRequest();
    static std::vector<char> buildAnnounceRequest(const char* connectionId, const std::string& infoHash, const std::string& peerId);

    // Message parsing functions
    struct ConnectResponse {
        int action;
        int transactionId;
        const char* connectionId;
    };
    
    static ConnectResponse parseConnectResponse(const char* response);

    struct AnnounceResponse {
        int action;
        int transactionId;
        int interval;
        int leechers;
        int seeders;
        std::vector<Peer> peers;
    };

    static AnnounceResponse parseAnnounceResponse(const char* response, int size);
};

#endif