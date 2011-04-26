#include "CommonFunctions.h"

// open sqlite db and return it as the db object i need
sqlite3* OpenSqliteDB()
{
	char*		sqlErrMsg;
	int			sqlValue;
	sqlite3* 	opendb;
	BString		tmpString;
	ErrorAlert*	eAlert;
	
	sqlErrMsg = 0;
	opendb = NULL;
	app_info info;
	be_app->GetAppInfo(&info);
	BPath path(&info.ref);
	path.GetParent(&path);
	BString tmpPath = path.Path();
	tmpPath += "/MasterPiece.db";
	sqlValue = sqlite3_open_v2(tmpPath, &opendb, SQLITE_OPEN_READWRITE, NULL); // open db
	if(sqlite3_errcode(opendb) == 14) // if error is SQLITE_CANTOPEN, then create db with structure
	{
		sqlValue = sqlite3_open_v2(tmpPath, &opendb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL); 
		if(sqlite3_errcode(opendb) == 0) // sqlite_ok
		{
			tmpString = "CREATE TABLE ideatable(ideaid integer primary key autoincrement, ideaname text, ideatext text, ismp integer, mpid integer, ordernumber integer);";
			sqlValue = sqlite3_exec(opendb, tmpString, NULL, NULL, &sqlErrMsg);
			if(sqlValue == SQLITE_OK) // if sql was successful
			{
				// no error, so i will return opendb at end;
			}
			else // sql not successful
			{
				eAlert = new ErrorAlert("1.1 Sql Error: ", sqlErrMsg);
				eAlert->Launch();
			}
		}
		else // some kind of failure
		{
			eAlert = new ErrorAlert("1.0 Sql Error: ", sqlite3_errmsg(opendb));
			eAlert->Launch();
		}
	}
	else if(sqlite3_errcode(opendb) == 0) // sqlite_OK, it exists
	{
		//no error, so i will return opendb at end;
	}
	else // if error is not ok or not existing, then display error in alert
	{
		eAlert = new ErrorAlert("1.2 Sql Error: ", sqlite3_errmsg(opendb));
		eAlert->Launch();
	}
	return opendb;
}

// might need to do an if bind count else if return count... or just know what we're calling...
// int SqlObject::ReturnInt(1);
// BString SqlObject::ReturnString();

SqlObject::SqlObject(sqlite3* sqlDB, sqlite3_stmt* sqlStatement, const char* errorNumber)
{
	sqldb = sqlDB;
	sqlstatement = sqlStatement;
	errornumber = errorNumber;
}
void SqlObject::PrepareSql(const char* sqlQuery)
{
	sqlquery = sqlQuery;
	if(sqlite3_prepare_v2(sqldb, sqlquery, -1, &sqlstatement, NULL) != SQLITE_OK) // sql statement was not prepared
	{
		tmpstring = errornumber;
		tmpstring += " Sql Error: Prepare Statement Failed";
		ealert = new ErrorAlert(tmpstring);
		ealert->Launch();
	}
}
void SqlObject::BindValue(int bindPlace, int bindValue)
{
	bindplace = bindPlace;
	bindint = bindValue;
	if(sqlite3_bind_int(sqlstatement, bindplace, bindint) != SQLITE_OK) // sql int bind failed
	{
		tmpstring = errornumber;
		tmpstring += " Sql Error: Bind Int Failed";
		ealert = new ErrorAlert(tmpstring);
		ealert->Launch();
	}
}
void SqlObject::BindValue(int bindPlace, double bindValue)
{
	bindplace = bindPlace;
	binddouble = bindValue;
	if(sqlite3_bind_double(sqlstatement, bindplace, binddouble) != SQLITE_OK) // sql double bind failed
	{
		tmpstring = errornumber;
		tmpstring += " Sql Error: Bind Double Failed";
		ealert = new ErrorAlert(tmpstring);
		ealert->Launch();
	}
}
void SqlObject::BindValue(int bindPlace, int64 bindValue)
{
	bindplace = bindPlace;
	bindint64 = bindValue;
	if(sqlite3_bind_int64(sqlstatement, bindplace, bindint64) != SQLITE_OK) // sql int64 bind failed
	{
		tmpstring = errornumber;
		tmpstring += " Sql Error: Bind Int64 Failed";
		ealert = new ErrorAlert(tmpstring);
		ealert->Launch();
	}
}
void SqlObject::BindValue(int bindPlace) // bind null
{
	bindplace = bindPlace;
}
/* bind functions */
/*
int sqlite3_bind_blob(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
int sqlite3_bind_null(sqlite3_stmt*, int);
int sqlite3_bind_text(sqlite3_stmt*, int, const char*, int n, void(*)(void*));
int sqlite3_bind_text16(sqlite3_stmt*, int, const void*, int, void(*)(void*));
int sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);
int sqlite3_bind_zeroblob(sqlite3_stmt*, int, int n);

The first argument to the sqlite3_bind_*() routines is always a pointer to the sqlite3_stmt object
returned from sqlite3_prepare_v2() or its variants.

The second argument is the index of the SQL parameter to be set. The leftmost SQL parameter has an
index of 1. When the same named SQL parameter is used more than once, second and subsequent
occurrences have the same index as the first occurrence. The index for named parameters can be looked
up using the sqlite3_bind_parameter_index() API if desired. The index for "?NNN" parameters is the
value of NNN. The NNN value must be between 1 and the sqlite3_limit() parameter
SQLITE_LIMIT_VARIABLE_NUMBER (default value: 999).

The third argument is the value to bind to the parameter.

In those routines that have a fourth argument, its value is the number of bytes in the parameter.
To be clear: the value is the number of bytes in the value, not the number of characters. If the
fourth parameter is negative, the length of the string is the number of bytes up to the first zero
terminator.

The fifth argument to sqlite3_bind_blob(), sqlite3_bind_text(), and sqlite3_bind_text16() is a
destructor used to dispose of the BLOB or string after SQLite has finished with it. The destructor
is called to dispose of the BLOB or string even if the call to sqlite3_bind_blob(),
sqlite3_bind_text(), or sqlite3_bind_text16() fails. If the fifth argument is the special value
SQLITE_STATIC, then SQLite assumes that the information is in static, unmanaged space and does not
need to be freed. If the fifth argument has the value SQLITE_TRANSIENT, then SQLite makes its own
private copy of the data immediately, before the sqlite3_bind_*() routine returns.

The sqlite3_bind_zeroblob() routine binds a BLOB of length N that is filled with zeroes. A zeroblob
uses a fixed amount of memory (just an integer to hold its size) while it is being processed.
Zeroblobs are intended to serve as placeholders for BLOBs whose content is later written using
incremental BLOB I/O routines. A negative value for the zeroblob results in a zero-length BLOB.

If any of the sqlite3_bind_*() routines are called with a NULL pointer for the prepared statement
or with a prepared statement for which sqlite3_step() has been called more recently than
sqlite3_reset(), then the call will return SQLITE_MISUSE. If any sqlite3_bind_() routine is passed
a prepared statement that has been finalized, the result is undefined and probably harmful.

Bindings are not cleared by the sqlite3_reset() routine. Unbound parameters are interpreted as NULL.

The sqlite3_bind_* routines return SQLITE_OK on success or an error code if anything goes wrong.
SQLITE_RANGE is returned if the parameter index is out of range. SQLITE_NOMEM is returned if
malloc() fails.
*/

int SqlObject::ReturnValue(int returnPlace)
{
	returnplace = returnPlace;
	if(sqlite3_column_type(sqlstatement, returnplace) == SQLITE_INTEGER)
	{
		return sqlite3_column_int(sqlstatement, returnplace);
	}
	else
	{
		// return error not valid
		return -15;
	}
}

void SqlObject::StepSql(void)
{
	sqlcode = sqlite3_step(sqlstatement);
	if(sqlcode == SQLITE_ROW)
	{
		// do something here
		// might do the while here... or call the while in while(SqlObject->SqlStep()=SQITE_ROW)
	}
	else if(sqlcode == SQLITE_DONE)
	{
		// do something here
	}
	else
	{
		tmpstring = errornumber;
		tmpstring += " Sql Error: Step Failed";
		ealert = new ErrorAlert(tmpstring);
		ealert->Launch();
	}
}
void SqlObject::ResetSql(void)
{
	if(sqlite3_reset(sqlstatement) != SQLITE_OK)
	{
		tmpstring = errornumber;
		tmpstring += " Sql Error: Reset Failed";
		ealert = new ErrorAlert(tmpstring);
		ealert->Launch();
	}
}
void SqlObject::FinalizeSql(void)
{
	if(sqlite3_finalize(sqlstatement) != SQLITE_OK)
	{
		tmpstring = errornumber;
		tmpstring += " Sql Error: Finalize Failed";
		ealert = new ErrorAlert(tmpstring);
		ealert->Launch();
	}
}
