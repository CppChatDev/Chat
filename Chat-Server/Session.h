#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket) : session_socket(std::move(socket)), data(1024)
	{
	}

	void start();

private:
	void do_read();
	void do_write(std::size_t length);

	tcp::socket session_socket;
	std::vector<char> data;
};

