/***************************************************************************
 * (c) 2008-2011 Aleksandar Topuzović                                      *
 * <aleksandar.topuzovic@fer.hr>, <aleksandar.topuzovic@gmail.com>         *
 ***************************************************************************/
#ifndef util_hpp
#define util_hpp

#include <sstream>

#define SQLITE_OPEN(rc,file_name,database) \
rc = sqlite3_open(file_name, &database); \
if( rc ) { \
	std::cerr << "Unable to open database: " << file_name << ", " << sqlite3_errmsg(database) << std::endl; \
	sqlite3_close(database); \
	throw "Unable to open database!"; \
}

#define SQLITE_PREPARE_STATEMENT_TEST(statement, sql, database) \
if ( sqlite3_prepare( database, sql.c_str(), sql.size(), &statement, NULL ) != SQLITE_OK ) { \
	std::cerr << "Unable to prepare SQL statement: " << sql << std::endl; \
	throw "Unable to prepare SQL statement!"; \
}

#define SQLITE_STEP_TEST(statement) \
if ( sqlite3_step(statement) != SQLITE_ROW ) { \
	std::cout << "Unable to return the value of the SQL query!\n" << std::endl; \
	throw "Unable to return the value of the SQL query!\n"; \
}

#endif // util_hpp
