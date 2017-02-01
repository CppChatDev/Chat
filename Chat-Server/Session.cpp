#include "Session.h"
#include "Database.h"
#include "Dispatcher.h"

Session::Session(tcp::socket socket, Dispatcher& other_dispatcher, std::string username) :
	User(move(username)),
	dispatcher(other_dispatcher),
	session_socket(std::move(socket))
{

}

void Session::start()
{
	dispatcher.add_participant(shared_from_this());
	do_read();

	deliver_pending();
}

void Session::deliver(const Message& msg, std::string msg_id)
{
	auto write_in_progress = !msg_queue.empty();
	msg_queue.push({ msg, msg_id });
	if (!write_in_progress)
		do_write();
}

void Session::do_read()
{
	auto self(shared_from_this());
	session_socket.async_read_some(read_msg.mutable_buffer(),
		[this, self](boost::system::error_code ec, size_t length)
	{
		if (!ec)
		{
			read_msg.set_size(length);

			try
			{
				auto recipient = read_msg.get_header();	// get recipient of the message
				read_msg.set_header(username);			// set current username (as sender)

				// send message to the recipient
				dispatcher.send(read_msg, recipient);
			}
			catch (std::exception &e)
			{
				// TODO - create separate cases for:
				// - too big header
				// - no header
				// - exception from dispatcher
			}

			do_read();
		}
		else
		{
			// TODO
		}
	});
}

void Session::do_write()
{
	auto self(shared_from_this());
	const boost::asio::const_buffers_1& buf = msg_queue.front().first.const_buffer();
	boost::asio::async_write(session_socket, buf,
		[this, self, buf](boost::system::error_code ec, size_t length)
	{
		if (!ec && length == boost::asio::buffer_size(buf))
		{
			// mark message as delivered
			dispatcher.get_db().execute("UPDATE messages SET delivered = 1 WHERE id = ?",
				{msg_queue.front().second});

			msg_queue.pop();
			if (!msg_queue.empty())
			{
				do_write();
			}
		}
		else
		{
			// TODO
		}
	});
}

void Session::deliver_pending()
{
	// TODO what if there is db exception here?
	auto select = "SELECT id, message FROM messages WHERE\
		recipient_id = (SELECT id FROM users WHERE username = ? )\
		AND delivered = 0 ;";

	Database database("database.db");
	auto results = database.execute(select, { username });

	for (auto &result : results)
	{
		Message msg(move(result["message"]));
		deliver(msg, result["id"]);
	}
}
