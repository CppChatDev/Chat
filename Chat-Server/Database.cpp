#include "Database.h"

Database::Database()
{
	if (sqlite3_open("database.db", &db) != SQLITE_OK)
		throw std::exception("There was a problem opening the database. Exiting!");
}

Database::~Database()
{
	sqlite3_close(db);
}

std::vector<db_element> Database::execute(std::string query)
{
	char *zErrMsg = nullptr;
	auto sql_callback = [](void *data, int argc, char **argv, char **cols) -> int
	{
		db_element element;
		for (auto i = 0; i < argc; i++)
			element[cols[i]] = argv[i];

		reinterpret_cast<std::vector<db_element>*>(data)->push_back(element);
		return 0;
	};

	std::vector<db_element> items;

	/* Execute SQL statement */
	auto rc = sqlite3_exec(db, query.c_str(), sql_callback, &items, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg); // TODO: throw exception?
		sqlite3_free(zErrMsg);
	}

	return items;
}
