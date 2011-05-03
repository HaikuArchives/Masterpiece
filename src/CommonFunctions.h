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
				void			PrepareSql(const char* sqlQuery);
				void			BindValue(int bindPlace, int bindValue);
				void			BindValue(int bindPlace, double bindValue);
				void			BindValue(int bindPlace, int64 bindValue);
				void			BindValue(int bindPlace, const char* bindValue);
				void			BindValue(int bindPlace, const void* bindValue);// blob bind
				void			BindValue(int bindPlace);
				int				ReturnValue(int returnPlace);
				double			ReturnValue(int returnPlace);
				int64			ReturnValue(int returnPlace);
				BString			ReturnValue(int returnPlace);
				const void*		ReturnValue(int returnPlace);
				void			StepSql(void);
				void			ResetSql(void);
				void			FinalizeSql(void);
	private:
				sqlite3*		sqldb;
				const char*		sqlquery;
				const char*		errornumber;
				const char*		bindstring;
				const char*		returnstring;
				const void*		bindblob;
				const void*		returnblob;
				ErrorAlert*		ealert;
				sqlite3_stmt*	sqlstatement;
				int				sqlcode;
				int				bindplace;
				int				returnplace;
				int				bindint;
				int				returnint;
				double			binddouble;
				double			returndouble;
				int64			bindint64;
				int64			returnint64;
				BString			tmpstring;
				
};
#endif
