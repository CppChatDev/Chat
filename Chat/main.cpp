#include<iostream>
#include "client.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main()
{
	try
	{
		boost::asio::io_service io_service;
		Client client(io_service);

		std::string input;
		std::getline(std::cin, input);

		std::vector<char> message(input.begin(), input.end());
		client.send_message(message);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}

	return 0;
}