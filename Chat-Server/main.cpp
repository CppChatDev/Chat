#include <iostream>
#include <string>
#include <vector>
#include "Server.h"
#include "Database.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char** argv)
{
	try
	{
		Database db;
		auto elements = db.execute("SELECT * FROM test;");

		for(auto &element : elements)
			for (auto &col : element)
				std::cout << col.first << " " << col.second << std::endl;
	}
	catch(std::exception e)
	{
		std::cout << e.what();
	}

	boost::asio::io_service io_service;
	//tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));

	try
	{
		/*while (1)
		{
			tcp::socket socket(io_service);
			acceptor.accept(socket);
			std::vector<char> receive_buffer(socket.available());
			boost::asio::read(socket, boost::asio::buffer(receive_buffer));
			boost::system::error_code error_code;
			boost::asio::write(socket, boost::asio::buffer(receive_buffer), error_code);
		}*/

		Server server(io_service, 1234);
		io_service.run();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}