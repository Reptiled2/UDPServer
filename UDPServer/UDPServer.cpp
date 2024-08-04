#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <functional>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

struct Peer {
    int id;
    in_addr ip;
};

enum {
    CONNECT = 1,
    DISCONNECT = 2,
    MESSAGE = 3,
    KEEPALIVE = 4,
    ALREADY_CONNECTED = 5
};

class UDPServer {
private:
    SOCKET p_serverSocket;
    std::vector<std::pair<int, sockaddr_in>> peers;
    int peerCount;

private:
    void send(const sockaddr_in &client, const char* buffer[2048]) {
        if (sendto(p_serverSocket, *buffer, sizeof(buffer), 0, (SOCKADDR*)&client, sizeof(client)) == SOCKET_ERROR) {
            std::cout << "Error ocurred while sending message!\n" << WSAGetLastError();
        };
        return;
    }

public:
    UDPServer(const int& PORT) {
        WSAData data;
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
            std::cout << "Error ocurred while loading winsock!\n";
            exit(-1);
        };

        p_serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        sockaddr_in serverProperties;
        serverProperties.sin_family = AF_INET;
        serverProperties.sin_port = htons(PORT);
        serverProperties.sin_addr.s_addr = htonl(INADDR_ANY);
        inet_pton(AF_INET, "127.0.0.1", &serverProperties.sin_addr);

        if (bind(p_serverSocket, (SOCKADDR*)&serverProperties, sizeof(serverProperties)) == SOCKET_ERROR) {
            std::cout << "Error ocurred while binding the socket!\n";
            exit(-1);
        };
    };

    ~UDPServer() {
        closesocket(p_serverSocket);
    };

    void start(const std::function<void(int, char[2048])> &callback) {
        std::cout << "Server started\n";

        while (true) {
            sockaddr_in clientAddr;
            int clientAddrLen = sizeof(clientAddr);
            char buffer[2048];

            int bytes = recvfrom(p_serverSocket, buffer, sizeof(buffer), 0, (SOCKADDR*)&clientAddr, &clientAddrLen);
            if (bytes == SOCKET_ERROR) {
                std::cout << "Error ocurred while reading data!\n";
                continue;
            };

            std::cout << "Received: " << buffer << "\n";
            switch (buffer[0]) {
            case CONNECT:
                const char* buffer[2048];
                for (std::pair<int, sockaddr_in> peer : peers) {
                    std::cout << peer.first;
                    if (peer.second.sin_addr.s_addr == clientAddr.sin_addr.s_addr && peer.second.sin_port == clientAddr.sin_port) {
                        const char* buffer[2048];

                        memset(buffer, 5, 1);
                        send(clientAddr, buffer);
                        std::cout << "sent message\n";
                        return;
                    };
                };

                peers.push_back(std::pair(peerCount, clientAddr));
                peerCount++;
                memset(buffer, 1, 1);
                send(clientAddr, buffer);
                break;

            case DISCONNECT:
                break;

            default:
                std::cout << "Couldnt read message\n";
                break;
            }

        };
    };

    void sendMessage(int peerId, const char buffer[2046]) {
        
    };
};

int main()
{
    UDPServer server(12345);

    server.start([](int byte, char data[2048]) {
        std::cout << "Message received: " << data << "\n";
    });

    return 0;
};