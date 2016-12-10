#include "client.h"
#include <string>
#include <iostream>

Client::Client(boost::asio::io_service& io_service):
	socket(io_service)
{
	tcp::resolver resolver(io_service);
	tcp::resolver::query query("localhost", "1234", tcp::resolver::query::canonical_name);
	auto endpoint_iterator = resolver.resolve(query);

	boost::asio::connect(socket, endpoint_iterator);
}

void Client::send_message(std::vector<char> message)
{
	socket.write_some(boost::asio::buffer(message));

	std::vector<char> response(1024);
	socket.read_some(boost::asio::buffer(response));

	for (auto &c : response)
		std::cout << c;

	std::cout << std::endl;
}
