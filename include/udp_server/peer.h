#pragma once

struct Peer {
	int peerId;
	std::string sessionToken;
	boost::asio::ip::udp::endpoint socket;
};
