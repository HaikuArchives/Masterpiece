#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include <sqlite3.h>
#include <Application.h>
#include <Roster.h>
#include <Path.h>
#include <String.h>

#include "ErrorAlert.h"

sqlite3*	OpenSqliteDB(void);

class SqlObject
{
	public:
								SqlObject(sqlite3* sqlDB, sqlite3_stmt* sqlStatement, const char* errorNumber);
								~SqlObject(void);
				void			PrepareSql(const char* sqlQuery, int bindCount, int returnCount);
				void			BindInt(int bindPlace, int bindValue);
				int				ReturnInt(int returnPlace)
				void			StepSql(void);
				void			ResetSql(void);
				void			FinalizeSql(void);
	private:
				sqlite3*		sqldb;
				const char*		sqlquery;
				const char*		errornumber;
				ErrorAlert*		ealert;
				sqlite3_stmt*	sqlstatement;
				int				sqlcode;
				int				bindplace;
				int				bindvalue;
				int				returnplace;
				BString			tmpstring;
				
};
#endif
