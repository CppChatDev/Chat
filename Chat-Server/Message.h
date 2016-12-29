#pragma once

#include <vector>
#include <boost/asio.hpp>

class Message
{
public:
	Message(std::vector<char> data);

	std::string get_header();

	void set_header(std::string header);

	// get buffer to data
	boost::asio::const_buffers_1 to_buffers() const;

	const std::vector<char>& get_data() const;

private:
	std::vector<char> data;
};
