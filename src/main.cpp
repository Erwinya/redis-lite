#include "protocol.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
using socket_t = SOCKET;
#define CLOSESOCK closesocket
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using socket_t = int;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define CLOSESOCK close
#endif

namespace {

bool init_net() {
#ifdef _WIN32
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
#else
    return true;
#endif
}

void cleanup_net() {
#ifdef _WIN32
    WSACleanup();
#endif
}

}  // namespace

int main(int argc, char **argv) {
    int port = 6379;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--port" && i + 1 < argc) {
            port = std::atoi(argv[++i]);
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: redis-lite [--port N]\n"
                      << "Commands: PING GET SET DEL EXISTS QUIT\n";
            return 0;
        }
    }

    if (!init_net()) {
        std::cerr << "network init failed\n";
        return 1;
    }

    socket_t server = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server == INVALID_SOCKET) {
        std::cerr << "socket failed\n";
        cleanup_net();
        return 1;
    }

    int yes = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR,
               reinterpret_cast<const char *>(&yes), sizeof(yes));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(static_cast<uint16_t>(port));

    if (bind(server, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "bind failed on port " << port << '\n';
        CLOSESOCK(server);
        cleanup_net();
        return 1;
    }
    if (listen(server, 8) == SOCKET_ERROR) {
        std::cerr << "listen failed\n";
        CLOSESOCK(server);
        cleanup_net();
        return 1;
    }

    std::cout << "redis-lite listening on port " << port << std::endl;
    redislite::Store store;

    while (true) {
        socket_t client = accept(server, nullptr, nullptr);
        if (client == INVALID_SOCKET) continue;

        std::string buffer;
        char chunk[512];
        while (true) {
            int n = recv(client, chunk, sizeof(chunk), 0);
            if (n <= 0) break;
            buffer.append(chunk, chunk + n);
            std::size_t pos;
            while ((pos = buffer.find('\n')) != std::string::npos) {
                std::string line = buffer.substr(0, pos);
                buffer.erase(0, pos + 1);
                if (!line.empty() && line.back() == '\r') line.pop_back();
                std::string reply = redislite::handle_line(store, line);
                if (!reply.empty()) {
                    send(client, reply.data(), static_cast<int>(reply.size()), 0);
                }
                if (line.size() >= 4) {
                    std::string u = line.substr(0, 4);
                    for (char &c : u) {
                        if (c >= 'a' && c <= 'z') c = static_cast<char>(c - 'a' + 'A');
                    }
                    if (u == "QUIT") {
                        CLOSESOCK(client);
                        goto next_client;
                    }
                }
            }
        }
        CLOSESOCK(client);
    next_client:;
    }

    CLOSESOCK(server);
    cleanup_net();
    return 0;
}
