#include "Registration.h"

std::pair<std::string, bool> Registration::handle_registration(const char registration_string[], Database& database)
{
	std::vector<std::string> registration_parts;
	boost::split(registration_parts, registration_string, boost::is_any_of(" "));
	if (registration_parts.size() != 3)
	{
		// expected format is "register <username> <password>", if the registrationString doesn't
		// match the format, abort!
		return {"0", false};
	}

	auto& username = registration_parts.at(1);
	auto& password = registration_parts.at(2);

	auto users = database.execute("SELECT username FROM users WHERE username = ?", {username});
	if (users.size() != 0)
	{
		return {username, false}; // username already exists, abort!
	}

	// std::cout << "Your username is: " << registration_parts.at(1) << " and password is: " <<
	// registration_parts.at(2) << "\n";

	database.execute("INSERT INTO users(username, password) VALUES(?, ?)", {username, password});

	users = database.execute("SELECT username FROM users WHERE username = ?", {username});
	if (users.size() != 0)
	{
		return {username, true}; // was the user successfully added to the database?
	}

	return {username, false};
}
