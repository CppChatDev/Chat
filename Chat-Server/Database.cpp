#include "Database.h"

Database::Database(std::string db_name):
	db(nullptr)
{
	sqlite3* sqlite3_db;
	if (sqlite3_open(db_name.c_str(), &sqlite3_db) != SQLITE_OK)
		throw std::runtime_error(sqlite3_errmsg(db.get()));
	
	// encapsulates sqlite3 pointer in unique pointer, sqlite3_close is deleter
	db = sql_pointer<sqlite3>(sqlite3_db, sqlite3_close);
}


std::vector<row> Database::execute(std::string query, std::vector<std::string> params) const
{
	std::vector<row> data;

	// prepare statement 
	sqlite3_stmt* stmt_p;
	auto result = sqlite3_prepare_v2(db.get(), query.c_str(), -1, &stmt_p, nullptr);
	if (result != SQLITE_OK)
		throw std::runtime_error(sqlite3_errmsg(db.get()));

	auto stmt = sql_pointer<sqlite3_stmt>(stmt_p, sqlite3_finalize);

	// bind parameters to query
	for (auto i = 0; i < params.size(); ++i)
	{
		result = sqlite3_bind_text(stmt.get(), i + 1, params[i].c_str(), -1, SQLITE_STATIC);
		if (result != SQLITE_OK)
			throw std::runtime_error(sqlite3_errmsg(db.get()));
	}

	// execute query
	while(1)
	{
		result = sqlite3_step(stmt.get());
		if(result == SQLITE_ROW)
		{
			auto count = sqlite3_data_count(stmt.get());
			row curr_row;
			for (auto i = 0; i < count; ++i)
			{
				auto name = std::string(sqlite3_column_name(stmt.get(), i));
				auto text = std::string(reinterpret_cast<const char*>(
					sqlite3_column_text(stmt.get(), i)));

				curr_row[name] = text;
			}

			data.emplace_back(curr_row);
		}
		else if (result == SQLITE_DONE)
		{
			break;
		}
		else
		{
			throw std::runtime_error(sqlite3_errmsg(db.get()));
		}
	} 

	return data;
}
