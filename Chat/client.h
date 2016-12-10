#pragma once
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Client
{
public:
	Client(boost::asio::io_service &io_service);
	void send_message(std::vector<char> message);

private:
	tcp::socket socket;
};