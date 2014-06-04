// conntest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../protobuf/krpc.pb.h"
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
namespace pbio = google::protobuf::io;

std::string varint(unsigned long long x) {
	char buf[10];
	pbio::CodedOutputStream::WriteVarint64ToArray(x, reinterpret_cast<google::protobuf::uint8*>(buf));
	return std::string(buf, pbio::CodedOutputStream::VarintSize64(x));
}
unsigned long long varint(const char* in) {
	unsigned long long ret;
	pbio::CodedInputStream::ReadLittleEndian64FromArray(reinterpret_cast<const google::protobuf::uint8*>(in), &ret);
	return ret;
}
std::string getmessage(std::istream& in) {
	std::vector<char> buf(10);
	for (int i = 0; i < 10; i++){
		char byte;
		in >> byte;
		buf[i] = byte;
		if (byte < 128) break;
	}
	auto len = varint(buf.data());
	buf.resize(len);
	in.read(&buf[0], len);
	return std::string(begin(buf), end(buf));
}

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
		iostr << "\x48\x45\x4C\x4C\x4F\xBA\xDA\x55"; // "I am a krpc client"
		{	char namebuf[32] = "krpc-cpp-mhj";
			iostr.write(namebuf, 32);
		}
		std::string s;

		Request r;
		r.set_service("KRPC");
		r.set_procedure("GetStatus");
		r.SerializeToString(&s);
		iostr << varint(s.size()) << s;
		
		Response resp;
		resp.ParseFromString(getmessage(iostr));
		std::string err = resp.error();
		std::string msg = resp.return_value();
		Status status;
		status.ParseFromString(msg);
		std::cerr << err << status.version();

		return 0;
	}
	catch (std::exception& e) {
		std::cerr << e.what();
		return 1;
	}
}

