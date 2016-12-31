#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>

template<class T>
using sql_pointer = std::unique_ptr<T, std::function<void(T*)>>;

using row = std::map<std::string, std::string>;

class Database
{
public:
	Database(std::string db_name);

	std::vector<row> execute(std::string query, std::vector<std::string> params) const;
private:
	sql_pointer<sqlite3> db;
};
