// conntest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "kConnection.h"

void dump_services(kConnection& connection) {
	std::ofstream dump("krpcdump.txt");
	Request r;
	r.set_service("KRPC");
	r.set_procedure("GetServices");
	connection.send_request(r);

	Response resp = connection.receive_response();
	// TODO: Error handling
	Services services_msg;
	services_msg.ParseFromString(resp.return_value());
	auto services = services_msg.services();

	for (const auto& serv : services) {
		std::string servicename = serv.name();
		for (const auto& proc : serv.procedures()) {
			dump << "PROC  " << servicename << ':' << proc.name() << '\n';
			for (const auto& param : proc.parameters()) {
				dump << '\t' << param.type() << ' ' << param.name() << '\n';
			}
			std::string ret = proc.return_type();
			if (!ret.empty()) {
				dump << "\t-> " << ret << '\n';
			}
			for (const auto& attr : proc.attributes()) {
				dump << "\t-- " << attr << '\n';
			}
		}
		for (const auto& clas : serv.classes()) {
			dump << "CLASS " << servicename << ':' << clas.name() << '\n';
		}
		for (const auto& enu : serv.enumerations()) {
			dump << "ENUM  " << servicename << ':' << enu.name() << '\n';
		}
	}
}

int main(int argc, char** argv)
{
	
	try {
		kConnection connection("localhost","50000");

		Request r;
		r.set_service("KRPC");
		r.set_procedure("GetStatus");
		connection.send_request(r);

		Response resp = connection.receive_response();
		auto& err = resp.error();
		if (!err.empty()) {
			std::cerr << err << '\n';
		} else {
			Status st;
			st.ParseFromString(resp.return_value());
			std::cout << st.version();
		}
		
		dump_services(connection);

		return 0;
	}
	catch (std::exception& e) {
		std::cerr << e.what();
		return 1;
	}
}

