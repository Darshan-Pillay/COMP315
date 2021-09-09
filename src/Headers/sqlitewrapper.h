#pragma once
#include <string>
#include <vector>
#include <sqlite3.h>
#include <exception>

/*
	File Name: sqlitewrapper.h
	Date     : 25/05/2021
	Author   : Darshan Pillay (Student#219012395)
	Creation : COMP315 - Advanced Programming { Major Group Project }

	Details  : This header file contains several class,struct and function
			   defintions which together define a simple wrapper for the
			   SQLite C/C++ API which provides a C++ program with a mechanism
			   for connecting to SQLite databases and performing elementrary
			   CRUD operations on them.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

namespace sqlitewrapper {
	constexpr auto RETURNS_NO_ROW_DATA = 0;
	constexpr auto RETURNS_ROW_DATA = 1;
	/*
	* SqliteDatabaseEngine is a class which provides a mechanism for connecting to a sqlite database
	* and performing CRUD operations on then.
	*/
	class SqliteDatabaseEngine {
	public:
		/*
		* Constructor which connects to a sqlite database with filepath
		* databaseFileName if possible
		*/
		SqliteDatabaseEngine(const char* databaseFilename);

		/*
		* Tests connection to sqlite database with filepath databaseFilename
		* and changes the db connected to that database if possible. This process
		* invaildates the current sqlite statement (i.e calls to execute statement
		* and obtainStatementResultRow return false) and the statement type is changed to
		* RETURNS_NO_ROW_DATA. Returns false if this operation fails otherwise returns true.
		*/
		bool changeDatabaseConnection(const char* databaseFilename);

		/*
		* Checks if the sqlite statement described by sqlStatement is
		* a valid statement that can be executed on the current DB. If it can
		*then it invalidates the current sqlite statement for the current
		* db connection and sets the statement type to RETURNS_NO_ROW_DATA.
		* Returns false if this operation fails.
		*/
		bool setExecutionStatement(const char* sqlStatement);

		/*
		*Executes the current sqlite statement against the current db connection
		* and determines if the result has ROW_DATA and sets the statement type accordingly.
		*/
		bool executeStatement();

		//If a statement is executed and returns rows this function returns the number of rows returned
		int getNumberOfReturnedRows();

		/*
		* A statement has statement type of RETURNS_ROW_DATA if and only if
		* i)   There is a valid db connection
		* ii)  The current sqlite statement is valid against the current db connection
		* iii) A call to executeStatement() changes the statement type to RETURNS_ROW_DATA
		* ( i) and ii) are neccessary for iii) )
		*
		* This function takes in a reference to a vector. If the above conditons are met,
		* Returns true and pushes all results obtained if the current sqlite statement is executed
		* against the current db to the vector the reference outputVector aliases. Otherwise it returns
		* false.
		*
		*/
		bool obtainStatementResultRow(std::vector<std::string>* outputVector);
		~SqliteDatabaseEngine() {
			sqlite3_finalize(this->currentStatement);
			sqlite3_close_v2(this->db);
			delete this->statementType;
		}
	private:
		sqlite3* db; //current sqlite db connection handle
		sqlite3_stmt* currentStatement; //current sqlite statement against current db connection
		int* statementType;
	};

	/*
	* Returns true if a connection to the sqlite db with filepath
	* databaseFilename can be obtained otherwise returns false
	*/
	bool checkConnectionToDatabase(const char* databaseFilename);

	/*
	* Returns true if databaseHandle is a valide sqlite db connection
	* and if sqlStatement describes a sqlite statement which can be executed
	* against this db connection
	*/
	bool checkStatement(const char* sqlStatement, sqlite3* databaseHandle);





}
