#pragma once

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/array.hpp>
#include <memory>
#include <vector>
#include <cstring>
#include <string>

#include "udp_server/packet.h"
#include "udp_server/peer.h"
#include "udp_server/buffer.h"