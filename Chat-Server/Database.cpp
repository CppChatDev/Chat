#include "Database.h"

Database::Database(std::string db_name)
{
	if (sqlite3_open(db_name.c_str(), &db) != SQLITE_OK)
		throw std::exception("There was a problem opening the database.");
}

Database::~Database()
{
	sqlite3_close(db);
}

std::vector<db_element> Database::execute(std::string query)
{
	auto sql_callback = [](void *data, int argc, char **argv, char **cols) -> int
	{
		db_element element;
		for (auto i = 0; i < argc; i++)
			element[cols[i]] = argv[i];

		reinterpret_cast<std::vector<db_element>*>(data)->push_back(element);
		return 0;
	};

	std::vector<db_element> items;
	char *err_msg = nullptr;

	/* Execute SQL statement */
	auto rc = sqlite3_exec(db, query.c_str(), sql_callback, &items, &err_msg);
	if (rc != SQLITE_OK) {
		std::exception e(err_msg);
		sqlite3_free(err_msg);

		throw e;
	}

	return items;
}
