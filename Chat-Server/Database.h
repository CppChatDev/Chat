#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>

using db_element = std::map<std::string, std::string>;

class Database
{
public:
	Database();
	~Database();

	std::vector<db_element> execute(std::string query);
private:
	sqlite3* db;
};
