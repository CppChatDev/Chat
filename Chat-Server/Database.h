#pragma once
#include <sqlite3.h>
#include <string>
#include <functional>
#include <vector>

using callback_type = std::function<void(std::vector<std::string>, std::vector<std::string>)>;

class Database
{
public:
	Database();
	~Database();

	void execute(std::string query, callback_type callback);
private:
	sqlite3* db;
};