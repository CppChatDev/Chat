#include <boost/asio.hpp>
#include "Authenticator.h"

using boost::asio::ip::tcp;

Authenticator::Authenticator(tcp::socket socket) :
	socket(std::move(socket)), database("database.db"), buffer(1024)
{

}

void Authenticator::authenticate(std::function<void(std::shared_ptr<Session>)> on_success)
{
	auto self(shared_from_this());
	socket.async_read_some(boost::asio::buffer(buffer),
		[this, on_success, self](boost::system::error_code ec, size_t length)
	{
		// TODO
		// authenticate

		// for now - only read username
		on_success(std::make_shared<Session>(std::move(socket),
			std::string(buffer.begin(), buffer.begin() + length)));
	});
}
