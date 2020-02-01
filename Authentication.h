#pragma once
#include <string>
#include <jdbc/cppconn/sqlstring.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/prepared_statement.h>

namespace Auth
{
	extern bool Authenitcation;
	extern bool bHWID;

	bool AdminRights();
	void SavePassword(bool Load);
	void Login();
	bool HWID(int id, sql::SQLString sqlHWID, sql::Driver* driver, sql::Connection* con, sql::PreparedStatement* pstmt);
	bool CheckLicense(std::string Date);
}