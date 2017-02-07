#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>

#include "Authenticator.h"
#include "bcrypt.h"

using boost::asio::ip::tcp;

Authenticator::Authenticator(tcp::socket other_socket, Dispatcher& other_dispatcher)
	: socket(std::move(other_socket)), database("database.db"), dispatcher(other_dispatcher)
{
}

void Authenticator::authenticate(std::function<void(std::shared_ptr<Session>)> on_success)
{
	auto self(shared_from_this());
	socket.async_read_some(boost::asio::buffer(buffer, 1024), [this, on_success, self](
																  boost::system::error_code ec, size_t length) {
		if (ec)
			return;

		while (buffer[length - 1] == '\n' || buffer[length - 1] == '\r')
			length--;

		buffer[length] = 0;

		std::vector<std::string> buffer_parts;
		boost::split(buffer_parts, buffer, boost::is_any_of(" "));

		if (buffer_parts.size() == 3 && buffer_parts.at(0) == "register")
		{
			auto& username = buffer_parts.at(1);
			auto& password = buffer_parts.at(2);

			auto users = database.execute("SELECT username FROM users WHERE username = ?", {username});
			if (users.size() == 0)
			{
				auto hash = generate_hash(password);
				database.execute("INSERT INTO users(username, password) VALUES(?, ?)", {username, hash});

				users = database.execute("SELECT username FROM users WHERE username = ?", {username});
				if (users.size() != 0)
				{
					on_success(std::make_shared<Session>(std::move(socket), dispatcher, username.c_str()));
				}
				else
				{
					socket.write_some(boost::asio::buffer("Invalid format. To register please type "
														  "\"register <username> <password>\""));
				}
			}
			else
			{
				socket.write_some(boost::asio::buffer("That username is already taken!"));
			}
		}
		else if (buffer_parts.size() == 3 && buffer_parts.at(0) == "login")
		{
			auto& username = buffer_parts.at(1);
			auto& password = buffer_parts.at(2);

			auto users =
				database.execute("SELECT username, password FROM users WHERE username = ?", {username});
			bool is_authenticated = false;

			if (users.size() > 0)
			{
				is_authenticated = check_hash(password, users[0]["password"]);
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
		else
		{
			socket.write_some(boost::asio::buffer("Invalid format."));
		}

	});
}

std::string Authenticator::generate_hash(const std::string& input, int work_factor)
{
	char salt[BCRYPT_HASHSIZE];
	char hash[BCRYPT_HASHSIZE];

	int ret = bcrypt_gensalt(work_factor, salt);
	if (ret != 0)
		throw std::runtime_error("generating salt error");
	ret = bcrypt_hashpw(input.c_str(), salt, hash);
	if (ret != 0)
		throw std::runtime_error("hashing error");

	return std::string(hash);
}

bool Authenticator::check_hash(const std::string& input, const std::string& hash)
{
	int ret = bcrypt_checkpw(input.c_str(), hash.c_str());
	if (ret == -1)
		throw std::runtime_error("hash checking error");

	return ret == 0;
}
