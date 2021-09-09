#include "Headers/sqlitewrapper.h"
#include <iostream>

/*
	File Name: sqlitewrapper.cpp
	Date     : 25/05/2021
	Author   : Darshan Pillay (Student#219012395)
	Creation : COMP315 - Advanced Programming { Major Group Project }

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


struct DatabaseConnectionException : public std::exception {
	const char* what() const throw() {
		return "Connection to a database failed!";
	}
};

sqlitewrapper::SqliteDatabaseEngine::SqliteDatabaseEngine(const char* databaseFilename){
	try {

		if (!checkConnectionToDatabase(databaseFilename)) {
			this->db = nullptr;
			throw DatabaseConnectionException();
		}
		else {
			sqlite3_open_v2(databaseFilename,&(this->db),SQLITE_OPEN_READWRITE,nullptr);
			this->currentStatement = nullptr;
			this->statementType = new int(RETURNS_NO_ROW_DATA);
		}
	}
	catch(DatabaseConnectionException& e){
		throw DatabaseConnectionException();
	}
	
}

bool sqlitewrapper::SqliteDatabaseEngine::changeDatabaseConnection(const char* databaseFilename) {
	
	bool connectionSuccessful = checkConnectionToDatabase(databaseFilename);
	if (connectionSuccessful) {
		sqlite3_finalize(this->currentStatement);
		sqlite3_close_v2(this->db);
		sqlite3_open_v2(databaseFilename, &(this->db), SQLITE_OPEN_READWRITE, nullptr);
		this->currentStatement = nullptr;
		*(this->statementType) = RETURNS_NO_ROW_DATA;
		return true;
	}
	return false;
}

bool sqlitewrapper::SqliteDatabaseEngine::setExecutionStatement(const char* sqlStatement) {
	bool validStatement = checkStatement(sqlStatement, this->db);
	if (validStatement) {
		sqlite3_finalize(this->currentStatement);
		sqlite3_prepare_v2(this->db, sqlStatement, -1, &(this->currentStatement), nullptr);
		*(this->statementType) = RETURNS_NO_ROW_DATA;
		return true;
	}
	return false;
}

bool sqlitewrapper::SqliteDatabaseEngine::executeStatement() {
	if (this->currentStatement) {
		int sqliteReturnCode = sqlite3_step(this->currentStatement);
		if (sqliteReturnCode == SQLITE_ROW) {
			*(this->statementType) = RETURNS_ROW_DATA;
		}
		else {
			*(this->statementType) = RETURNS_NO_ROW_DATA;
		}
		return true;
	}
	return false;
}

int sqlitewrapper::SqliteDatabaseEngine::getNumberOfReturnedRows() {
	if (this->statementType) {
		
		if (*(this->statementType))
		{
			if (currentStatement)
			{	
				int count = 0;
				sqlite3_reset(this->currentStatement);
				while (sqlite3_step(this->currentStatement) == SQLITE_ROW)
				{
					count = count + 1;
				}
				sqlite3_reset(this->currentStatement);
;				return count;
			}
		}
		
	}
	return 0;
}

bool sqlitewrapper::SqliteDatabaseEngine::obtainStatementResultRow(std::vector<std::string>* outputVector) {
	if (*(this->statementType) && outputVector) {
		int numberOfColumnsInResult = sqlite3_column_count(this->currentStatement);
		for (int i = 0; i < numberOfColumnsInResult; i++) {
			const unsigned char* resultText = sqlite3_column_text(this->currentStatement,i);
			std::string s = (const char*)resultText;
			(*outputVector).push_back(s);
		}
		return true;
	}
	return false;
}

inline bool sqlitewrapper::checkConnectionToDatabase(const char* databaseFilename)
{	
	sqlite3* temp;
	if (databaseFilename) {
		int sqliteReturnCode = sqlite3_open_v2(databaseFilename, &temp, SQLITE_OPEN_READWRITE, nullptr);
		sqlite3_close_v2(temp);
		return (sqliteReturnCode == SQLITE_OK);
	}
	return false;
}

inline bool sqlitewrapper::checkStatement(const char* sqlStatement,sqlite3* databaseHandle) {
	if (sqlStatement && databaseHandle) {
		sqlite3_stmt* temp;
		int sqliteReturnCode = sqlite3_prepare_v2(databaseHandle, sqlStatement, -1, &temp, nullptr);
		sqlite3_finalize(temp);
		return (sqliteReturnCode == SQLITE_OK || sqliteReturnCode == SQLITE_ROW);
	}
	return false;
}


