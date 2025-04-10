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