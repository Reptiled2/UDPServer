#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")


class UDPClient {
private:
    SOCKET p_socket;
    sockaddr_in serverProperties;

public:
    UDPClient(const char* HOST, const int& PORT) {
        WSAData data;
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
            std::cout << "Error ocurred while loading winsock!\n";
            exit(-1);
        };

        p_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        serverProperties.sin_family = AF_INET;
        serverProperties.sin_port = htons(PORT);
        inet_pton(AF_INET, HOST, &serverProperties.sin_addr);

        std::cout << "Client established!\n";
    };

    ~UDPClient() {
        closesocket(p_socket);
    };

    void sendMessage(const char buffer[2048]) {
        int bytes = sendto(p_socket, buffer, sizeof(buffer), 0, (SOCKADDR*) &serverProperties, sizeof(serverProperties));
        if (bytes == SOCKET_ERROR) {
            std::cout << "Error ocurred while sending buffer!\n" << WSAGetLastError();
            exit(-1);
        };
    };
};

int main() {
    UDPClient client("127.0.0.1", 12345);

    while (true) {
        std::cout << "\nMessage to send: ";
        char buffer[2048];
        std::cin >> buffer;

        client.sendMessage(buffer);
    }

    return 0;
}
