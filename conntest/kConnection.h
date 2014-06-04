#pragma once
//#include <google/protobuf/message.h>

class kConnection
{
	typedef std::string string;
	typedef google::protobuf::Message Message;

	//static asio::io_service io_service;

	asio::ip::tcp::iostream stream;
	std::string readmessage();
public:
	static std::string varint(unsigned long long x);
	static unsigned long long kConnection::read_varint(std::string in);

	// In the future, send_request will return future<Response> and receive_response will be removed.
	void send_request(const Request& r);
	Response receive_response();

	kConnection(string host, string port, string name = "krpc-cpp");
	~kConnection();
};

