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

// Responsible for db connection and executing SQL queries
class Database
{
public:
	explicit Database(std::string db_name);

	// Executes SQL query
	// SQL code and data are passed independently
	// Query is passed as first parameter and '?' is used instead of real data
	// The data is passed as second argument (vector of strings) where each element of this vector
	// corresponds to appropriate '?' in SQL query
	// Example: query = "Select ? from X where id = ?", paramas = {"name", "5"}
	// so resulting query is: "Select "name" from X where id = "5"
	std::vector<row> execute(std::string query, std::vector<std::string> params = {}) const;
private:
	sql_pointer<sqlite3> db;
};
