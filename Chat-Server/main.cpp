#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>

#include "Server.h"
#include "Database.h"


using boost::asio::ip::tcp;

int main(int argc, char** argv)
{
	//try
	//{
	//	Database db("database.db");
	//	auto elements = db.execute("SELECT * FROM test;");

	//	for(auto &element : elements)
	//		for (auto &col : element)
	//			std::cout << col.first << " " << col.second << std::endl;
	//}
	//catch(std::exception &e)
	//{
	//	std::cout << e.what();
	//}

	boost::asio::io_service io_service;

	try
	{
		Server server(io_service, 1234);
		io_service.run();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	getchar();
	
	return 0;
}