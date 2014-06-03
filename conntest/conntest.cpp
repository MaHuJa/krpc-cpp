// conntest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../protobuf/krpc.pb.h"

int main(int argc, char** argv)
{
	asio::io_service ioservice;
	asio::ip::tcp::socket socket(ioservice);
	try {
		auto addr = asio::ip::address::from_string("127.0.0.1");
		asio::ip::tcp::endpoint remote_endpoint(addr, 50000);
		socket.connect(remote_endpoint);

		if (!socket.is_open()) {
			std::cout << "Failed to open\n";
			return 1;
		}
		auto i_am_krpc_client = asio::buffer("\x48\x45\x4C\x4C\x4F\xBA\xDA\x55", 8);
		asio::write(socket, i_am_krpc_client);
		{	char namebuf[32] = "krpc-cpp-mhj";
		asio::write(socket, asio::buffer(namebuf, 32));
		}

		return 0;
	}
	catch (std::exception& e) {
		std::cerr << e.what();
		return 1;
	}
}

