#include "Authenticator.h"
#include "HashGenerator.h"
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

Authenticator::Authenticator(tcp::socket other_socket, Dispatcher& other_dispatcher)
	: socket(std::move(other_socket)), database("database.db"), dispatcher(other_dispatcher)
{
}

void Authenticator::authenticate(std::function<void(std::shared_ptr<Session>)> on_success)
{
	auto self(shared_from_this());
	Registration registration;

	socket.async_read_some(boost::asio::buffer(buffer, 1024), [this, on_success, &registration, self](
																  boost::system::error_code ec, size_t length) {
		if (ec)
			return;

		while (buffer[length - 1] == '\n' || buffer[length - 1] == '\r')
			length--;

		buffer[length] = 0;

		std::vector<std::string> buffer_parts;
		boost::split(buffer_parts, buffer, boost::is_any_of(" "));

		if (buffer_parts.size() >= 1 && buffer_parts.at(0) == "register")
		{
			auto registration_result = registration.handle_registration(buffer, database);
			std::string registered_username = std::get<0>(registration_result);
			bool register_success = std::get<1>(registration_result);
			if (registered_username != "0" && register_success)
			{
				on_success(std::make_shared<Session>(
					std::move(socket), dispatcher, registered_username.c_str()));
			}
			else if (registered_username != "0" && !register_success)
			{
				socket.write_some(boost::asio::buffer("That username is already taken!"));
			}
			else // registered_username == "0"
			{
				socket.write_some(boost::asio::buffer(
					"Invalid format. To register please type \"register <username> <password>\""));
			}
		}
		else if (buffer_parts.size() >= 1 && buffer_parts.at(0) == "login")
		{
			auto& username = buffer_parts.at(1);
			auto& password = buffer_parts.at(2);

			auto users =
				database.execute("SELECT username, password FROM users WHERE username = ?", {username});
			bool is_authenticated = false;

			if (users.size() > 0)
			{
				HashGenerator generator;
				is_authenticated = generator.check_hash(password, users[0]["password"]);
			}

			if (is_authenticated)
			{
				on_success(std::make_shared<Session>(std::move(socket), dispatcher, username));
			}
			else
			{
				socket.write_some(
					boost::asio::buffer("Failed to authenticate: wrong password or username."));
			}
		}

	});
}
