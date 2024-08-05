#ifndef PEER_H
#define PEER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <boost/asio.hpp>

typedef struct Peer {
	int peerId;
	std::string sessionToken;
	boost::asio::ip::udp::endpoint socket;
};

#endif