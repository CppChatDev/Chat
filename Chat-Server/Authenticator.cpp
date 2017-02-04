#include "Authenticator.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

Authenticator::Authenticator(tcp::socket other_socket, Dispatcher& other_dispatcher)
	: socket(std::move(other_socket)), database("database.db"), dispatcher(other_dispatcher), registration()
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
			while (buffer[length - 1] == '\n' || buffer[length - 1] == '\r')
				length--;

			buffer[length] = 0;
            
            auto registrationResult = registration.handleRegistration(buffer);
            std::string registeredUsername = std::get<0>(registrationResult);
            bool registerSuccess = std::get<1>(registrationResult);
            if (registeredUsername != "0" && registerSuccess == true)
            {
                on_success(std::make_shared<Session>(std::move(socket), dispatcher, registeredUsername.c_str()));
            }
            
			bool is_authenticated = false;
			auto users = database.execute("SELECT username FROM users WHERE username = ?", {buffer});
			is_authenticated = users.size();

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
