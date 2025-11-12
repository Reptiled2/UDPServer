#pragma once

typedef struct Peer {
	int peerId;
	std::string sessionToken;
	boost::asio::ip::udp::endpoint socket;
};
