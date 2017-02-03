#include "Authenticator.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

Authenticator::Authenticator(tcp::socket other_socket, Dispatcher& other_dispatcher)
	: socket(std::move(other_socket)), database("database.db"), dispatcher(other_dispatcher)
{
}

void Authenticator::authenticate(std::function<void(std::shared_ptr<Session>)> on_success)
{
	auto self(shared_from_this());

	socket.async_read_some(boost::asio::buffer(buffer, 1024),
		[this, on_success, self](boost::system::error_code ec, size_t length) {
			if (ec)
				return;
			// TODO
			// authenticate
			buffer[length] = 0;
			bool is_authenticated = false;
			auto users = database.execute("SELECT username FROM users");
			for (auto&& user : users)
			{
				for (auto&& row : user)
				{
					if (row.second == buffer)
					{
						is_authenticated = true;
					}
				}
			}

			if (is_authenticated)
			{
				on_success(std::make_shared<Session>(std::move(socket), dispatcher, buffer));
			}
			else
			{
				socket.write_some(
					boost::asio::buffer("Failed to authenticate: Username not registered"));
			}
		});
}
