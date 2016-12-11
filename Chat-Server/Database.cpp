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

void Database::execute(std::string query, callback_type callback)
{
	char *zErrMsg = nullptr;
	auto sql_callback = [](void *out_callback, int argc, char **argv, char **cols) -> int
	{
		std::vector<std::string> col_names(cols, cols + argc);
		std::vector<std::string> fields(argv, argv + argc);

		callback_type* callback = reinterpret_cast<callback_type*>(out_callback);

		(*callback)(col_names, fields);

		return 0;
	};

	/* Execute SQL statement */
	auto rc = sqlite3_exec(db, query.c_str(), sql_callback, &callback, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg); // TODO: throw exception?
		sqlite3_free(zErrMsg);
	}
}
