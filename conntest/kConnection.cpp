#include "stdafx.h"
#include "kConnection.h"
#include <google/protobuf/io/coded_stream.h>

kConnection::kConnection(string host, string port, string name) : stream(host,port)
{
	if (!stream) throw std::runtime_error(stream.error().message());
	stream << "\x48\x45\x4C\x4C\x4F\xBA\xDA\x55"; // "I am a krpc client"
	name.resize(32);
	stream.write(name.data(), 32);

}

kConnection::~kConnection()
{
}

void kConnection::send_request(const Request& m) {
	std::string s;
	m.SerializeToString(&s);
	stream << varint(s.size()) << s;

	// TODO: Return future to the response
}

Response kConnection::receive_response() {
	Response r;
	auto s = readmessage();
	r.ParseFromString(s);
	return r;
}

std::string kConnection::readmessage () {
	std::vector<char> buf(10);
	for (int i = 0; i < 10; i++) { buf[i] = 0; }
	for (int i = 0; i < 10; i++){
		char byte;
		stream >> byte;
		buf[i] = byte;
		if ((byte & 0x80) == 0) break;
	}
	google::protobuf::uint8* u8 = reinterpret_cast<google::protobuf::uint8*> (buf.data());
	unsigned long long len;
	google::protobuf::io::CodedInputStream(u8, buf.size()).ReadVarint64(&len);
	buf.resize(static_cast<decltype(buf)::size_type>(len));
	stream.read(&buf[0], len);
	return std::string(begin(buf), end(buf));

	
}

unsigned long long kConnection::read_varint(std::string in) {
	google::protobuf::uint64 ret = 0;
	auto input = reinterpret_cast<const google::protobuf::uint8*>(in.data());
	google::protobuf::io::CodedInputStream(input, in.size()).ReadVarint64(&ret);

	return ret;
}
std::string kConnection::varint(unsigned long long x) {
	char buf[10];
	google::protobuf::io::CodedOutputStream::WriteVarint64ToArray(x, reinterpret_cast<google::protobuf::uint8*>(buf));
	return std::string(buf, google::protobuf::io::CodedOutputStream::VarintSize64(x));
}
