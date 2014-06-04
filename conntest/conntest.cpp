// conntest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "kConnection.h"

int main(int argc, char** argv)
{
	try {
		kConnection connection("localhost","50000");

		Request r;
		r.set_service("KRPC");
		r.set_procedure("GetStatus");
		connection.send_request(r);

		Response resp = connection.receive_response();
		auto& s = resp.error();
		if (!s.empty()) {
			std::cerr << s << '\n';
		} else {
			Status st;
			st.ParseFromString(resp.return_value());
			std::cout << st.version();
		}
		
		return 0;
	}
	catch (std::exception& e) {
		std::cerr << e.what();
		return 1;
	}
}

