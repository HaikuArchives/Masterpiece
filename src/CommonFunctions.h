#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include <sqlite3.h>
#include <Application.h>
#include <Roster.h>
#include <Path.h>
#include <String.h>

#include "ErrorAlert.h"

sqlite3*	OpenSqliteDB(void);
int			PrepareSql2(sqlite3* tmpdb, const char* sqlquery, sqlite3_stmt** tmpStatement, const char* errorNumber, int tmpInt=-1, const char** unused=NULL);
int			BindInteger2(sqlite3_stmt* tmpStatement, int bindPlace, int bindValue, const char* errorNumber);
int			StepSql2(sqlite3_stmt* tmpStatement, const char* errorNumber);
int			RunSql2(sqlite3* tmpdb, const char* sqlquery, sqlite3_stmt** tmpStatement, const char* errorNumber, int bindCount, int tmpInt=-1, const char** unused=NULL);

class SqlObject
{
	public:
								SqlObject(sqlite3* sqlDB, sqlite3_stmt* sqlStatement, const char* errorNumber);
								~SqlObject(void);
				void			PrepareSql(const char* sqlQuery);
				void			BindInt(int bindPlace, int bindValue);
				void			StepSql(void);
				void			ResetSql(void);
	private:
				sqlite3*		sqldb;
				const char*		sqlquery;
				const char*		errornumber;
				ErrorAlert*		ealert;
				sqlite3_stmt*	sqlstatement;
				int				sqlcode;
				int				bindplace;
				int				bindvalue;
				BString			tmpstring;
				
};
#endif
