#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "Packet.h"
#include "Peer.h"
#include "TokenGenerator.h"
#include "Buffer.h"


class UDPServer {
private:
    std::unique_ptr<boost::asio::ip::udp::socket> p_serverSocket;
    boost::asio::io_context p_iocontext;
    std::vector<Peer> p_peers;
    int p_peerCount;

private:
    void send(std::vector<char> buffer, boost::asio::ip::udp::endpoint remoteInfo) {
        boost::system::error_code err;
        p_serverSocket->send_to(boost::asio::buffer(buffer), remoteInfo, 0, err);

        if (err) {
            std::cout << "Error ocurred while sending message: " << err.message() << "\n";
        }
    };

    void handleMessage(std::shared_ptr<boost::array<char, 2048>> receivedBuffer, std::shared_ptr<boost::asio::ip::udp::endpoint> remoteInfo) {
        std::cout << "Message received: " << receivedBuffer->data() << "\n";
        switch (atoi(&receivedBuffer->data()[0])) {
        case PacketTypes::CONNECT: {
            bool found = false;

            for (const Peer& peer : p_peers) {
                if (peer.socket.address().to_string() != remoteInfo->address().to_string() && peer.socket.port() != remoteInfo->port()) {
                    continue;
                };

                found = true;
                break;
            };

            if (found) {
                break;
            };

            Peer newPeer;
            newPeer.peerId = p_peerCount;
            newPeer.socket = *remoteInfo;
            newPeer.sessionToken = tokenGenerator(p_peers);

            Packet packet;
            packet.type = PacketTypes::CONNECT;
            packet.peerId = { newPeer.peerId };
            
            BufferVector buffer;
            buffer.peerId(newPeer.peerId);
            buffer.addElement(newPeer.sessionToken);
            

            p_peers.push_back(newPeer);
            p_peerCount++;
            break;
        }

        case PacketTypes::DISCONNECT: {
            std::cout << "To be added\n";
            break;
        }

        case PacketTypes::MESSAGE: {
            break;
        }

        default: {
            std::cout << "Couldnt read message\n";
            break;
        }
        };
    };

    void receiveMessage() {
        auto receivedBuffer = std::make_shared<boost::array<char, 2048>>();
        auto remoteInfo = std::make_shared<boost::asio::ip::udp::endpoint>();

        p_serverSocket->async_receive_from(
            boost::asio::buffer(*receivedBuffer),
            *remoteInfo,
            [this, receivedBuffer, remoteInfo](const boost::system::error_code &err, size_t bytes) {
                if (err) {
                    std::cout << "Error ocurred while receiving data: " << err.message() << "\n";
                    return;
                };


                handleMessage(receivedBuffer, remoteInfo);
                receiveMessage();
            }
        );
    };

public:
    UDPServer(const int& PORT)
        : p_peerCount(0)
    {
        p_serverSocket = std::make_unique<boost::asio::ip::udp::socket>(p_iocontext);

        p_serverSocket->open(boost::asio::ip::udp::v4());
        p_serverSocket->bind(boost::asio::ip::udp::endpoint(
            boost::asio::ip::address::from_string("127.0.0.1"),
            PORT)
        );

        receiveMessage();
    };

    ~UDPServer() {
        p_iocontext.stop();
        p_serverSocket->close();
    };

    void start() {
        std::cout << "Server started!\n";
        p_iocontext.run();
    };

    void sendMessage(Packet packet) {
        if (sizeof(packet.buffer) > 2040) {
            std::cout << "Buffer size limit exceeded!\n";
            return;
        }
        
        std::vector<char> packetBuffer = packet.buffer.getBuffer();
        std::vector<char> buffer;
        buffer.reserve(2048);
        buffer.push_back(static_cast<char>(packet.type));
        buffer.push_back('\0');
        buffer.insert(buffer.end(), packetBuffer.begin(), packetBuffer.end());

        for (int i : packet.peerId) {
            for (Peer &peer : p_peers) {
                if (peer.peerId != i) {
                    continue;
                };

                send(buffer, peer.socket);
                break;
            }
        };
    };
};

int main()
{
    UDPServer server(12345);
    server.start();
    return 0;
};