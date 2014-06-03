// conntest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../protobuf/krpc.pb.h"

int main(int argc, char** argv)
{
	asio::io_service ioservice;
	asio::ip::tcp::socket socket(ioservice);
	try {
		asio::ip::tcp::iostream iostr("127.0.0.1", "50000");
		if (!iostr) {
			std::cout << "Failed to open\n";
			return 1;
		}
		iostr << "\x48\x45\x4C\x4C\x4F\xBA\xDA\x55";
		{	char namebuf[32] = "krpc-cpp-mhj";
			iostr.write(namebuf, 32);
		}






		return 0;
	}
	catch (std::exception& e) {
		std::cerr << e.what();
		return 1;
	}
}

