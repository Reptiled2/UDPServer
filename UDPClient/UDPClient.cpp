#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::udp;


class UDPClient {
private:
    std::unique_ptr<udp::socket> p_socket;
    boost::asio::io_context p_iocontext;
    std::unique_ptr<udp::endpoint> p_serverSocket;

    std::string sessionToken = "";

private:
    void receiveMessage() {
        auto receivedBuffer = std::make_shared<boost::array<char, 2048>>();
        
        std::cout << "receiving\n";
        p_socket->async_receive_from(
            boost::asio::buffer(*receivedBuffer),
            *p_serverSocket,
            [this, receivedBuffer](const boost::system::error_code& err, size_t bytes) {
            if (err) {
                std::cout << "Error ocurred while receiving data: " << err.value() << "\n";
                return;
            };

            std::cout << "Received message: " << receivedBuffer->data() << "\n";
            receiveMessage();
        }
        );
    };

public:
    UDPClient(const char* HOST, const int& PORT) {
        p_socket = std::make_unique<udp::socket>(p_iocontext);
        p_socket->open(udp::v4());

        p_serverSocket = std::make_unique<udp::endpoint>(boost::asio::ip::address::from_string(HOST), PORT);

    };

    ~UDPClient() {
        p_socket->close();
    };

    void sendMessage(std::vector<char> buffer) {
        std::cout << "sending message\n";
        p_socket->async_send_to(boost::asio::buffer(buffer), *p_serverSocket, 0, 
            [](const boost::system::error_code &err, const std::size_t &bytes) {
                if (err) {
                    std::cout << "Error ocurred while sending message: " << err.message() << "\n";
                };
            }
        );
    };

    void start() {
        p_iocontext.run();
        receiveMessage();
    }
};

int main() {
    UDPClient client("127.0.0.1", 12345);
    client.start();

    while (true) {
        std::cout << "\nMessage to send: ";
        std::string msg;
        std::cin >> msg;
        std::vector<char> buffer;
        buffer.insert(buffer.end(), msg.begin(), msg.end());

        client.sendMessage(buffer);
    }

    return 0;
}
