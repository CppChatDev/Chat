#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Database.h"
#include "Server.h"

using boost::asio::ip::tcp;

int main(void)
{
	boost::asio::io_service io_service;

	try
	{
		Server server(io_service, 1234);
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	getchar();

	return 0;
}
