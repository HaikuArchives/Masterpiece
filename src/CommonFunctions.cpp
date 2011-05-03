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
void SqlObject::BindValue(int bindPlace, const char* bindValue)
{
	bindplace = bindPlace;
	bindstring = bindValue;
	if(sqlite3_bind_text(sqlstatement, bindplace, bindstring, -1, SQLITE_TRANSIENT) != SQLITE_OK) // sql text bind failed
	{
		tmpstring = errornumber;
		tmpstring += " Sql Error: Bind Text Failed";
		ealert = new ErrorAlert(tmpstring);
		ealert->Launch();
	}
}
void SqlObject::BindValue(int bindPlace) // bind null
{
	bindplace = bindPlace;
	if(sqlite3_bind_null(sqlstatement, bindplace) != SQLITE_OK)
	{
		tmpstring = errornumber;
		tmpstring += " Sql Error: Bind NULL Failed";
		ealert = new ErrorAlert(tmpstring);
		ealert->Launch();
	}
}
void SqlObject::BindValue(int bindPlace, const void* bindValue)
{
	bindplace = bindPlace;
	bindblob = bindValue;
	if(sqlite3_bind_blob(sqlstatement, bindplace, bindblob, -1, SQLITE_TRANSIENT) != SQLITE_OK) // sql blob bind failed
	{
		tmpstring = errornumber;
		tmpstring += " Sql Error: Bind Blob Failed";
		ealert = new ErrorAlert(tmpstring);
		ealert->Launch();
	}
}
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
/* Return Value Information */
/*
Result Values From A Query

sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 0))

const void *sqlite3_column_blob(sqlite3_stmt*, int iCol);
double sqlite3_column_double(sqlite3_stmt*, int iCol);
int sqlite3_column_int(sqlite3_stmt*, int iCol);
sqlite3_int64 sqlite3_column_int64(sqlite3_stmt*, int iCol);
const unsigned char *sqlite3_column_text(sqlite3_stmt*, int iCol);
const void *sqlite3_column_text16(sqlite3_stmt*, int iCol);
int sqlite3_column_type(sqlite3_stmt*, int iCol);
sqlite3_value *sqlite3_column_value(sqlite3_stmt*, int iCol);
These routines form the "result set" interface.

These routines return information about a single column of the current result row of a query.
In every case the first argument is a pointer to the prepared statement that is being evaluated
(the sqlite3_stmt* that was returned from sqlite3_prepare_v2() or one of its variants) and the
second argument is the index of the column for which information should be returned. The leftmost
column of the result set has the index 0. The number of columns in the result can be determined
using sqlite3_column_count().

If the SQL statement does not currently point to a valid row, or if the column index is out of
range, the result is undefined. These routines may only be called when the most recent call to
sqlite3_step() has returned SQLITE_ROW and neither sqlite3_reset() nor sqlite3_finalize() have
been called subsequently. If any of these routines are called after sqlite3_reset() or
sqlite3_finalize() or after sqlite3_step() has returned something other than SQLITE_ROW, the
results are undefined. If sqlite3_step() or sqlite3_reset() or sqlite3_finalize() are called from
a different thread while any of these routines are pending, then the results are undefined.

The sqlite3_column_type() routine returns the datatype code for the initial data type of the result
column. The returned value is one of SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB, or
SQLITE_NULL. The value returned by sqlite3_column_type() is only meaningful if no type conversions
have occurred as described below. After a type conversion, the value returned by
sqlite3_column_type() is undefined. Future versions of SQLite may change the behavior of
sqlite3_column_type() following a type conversion.

If the result is a BLOB or UTF-8 string then the sqlite3_column_bytes() routine returns the number
of bytes in that BLOB or string. If the result is a UTF-16 string, then sqlite3_column_bytes()
converts the string to UTF-8 and then returns the number of bytes. If the result is a numeric
value then sqlite3_column_bytes() uses sqlite3_snprintf() to convert that value to a UTF-8 string 
and returns the number of bytes in that string. If the result is NULL, then sqlite3_column_bytes()
returns zero.

If the result is a BLOB or UTF-16 string then the sqlite3_column_bytes16() routine returns the
number of bytes in that BLOB or string. If the result is a UTF-8 string, then
sqlite3_column_bytes16() converts the string to UTF-16 and then returns the number of bytes. If the
result is a numeric value then sqlite3_column_bytes16() uses sqlite3_snprintf() to convert that
value to a UTF-16 string and returns the number of bytes in that string. If the result is NULL,
then sqlite3_column_bytes16() returns zero.

The values returned by sqlite3_column_bytes() and sqlite3_column_bytes16() do not include the
zero terminators at the end of the string. For clarity: the values returned by
sqlite3_column_bytes() and sqlite3_column_bytes16() are the number of bytes in the string, not the
number of characters.

Strings returned by sqlite3_column_text() and sqlite3_column_text16(), even empty strings, are
always zero terminated. The return value from sqlite3_column_blob() for a zero-length BLOB is a
NULL pointer.

The object returned by sqlite3_column_value() is an unprotected sqlite3_value object. An
unprotected sqlite3_value object may only be used with sqlite3_bind_value() and
sqlite3_result_value(). If the unprotected sqlite3_value object returned by sqlite3_column_value()
is used in any other way, including calls to routines like sqlite3_value_int(),
sqlite3_value_text(), or sqlite3_value_bytes(), then the behavior is undefined.

These routines attempt to convert the value where appropriate. For example, if the internal
representation is FLOAT and a text result is requested, sqlite3_snprintf() is used internally to
perform the conversion automatically. The following table details the conversions that are applied:

Internal
Type	 Requested
Type	 Conversion
NULL	 INTEGER	 Result is 0
NULL	 FLOAT	 Result is 0.0
NULL	 TEXT	 Result is NULL pointer
NULL	 BLOB	 Result is NULL pointer
INTEGER	 FLOAT	 Convert from integer to float
INTEGER	 TEXT	 ASCII rendering of the integer
INTEGER	 BLOB	 Same as INTEGER->TEXT
FLOAT	 INTEGER	 Convert from float to integer
FLOAT	 TEXT	 ASCII rendering of the float
FLOAT	 BLOB	 Same as FLOAT->TEXT
TEXT	 INTEGER	 Use atoi()
TEXT	 FLOAT	 Use atof()
TEXT	 BLOB	 No change
BLOB	 INTEGER	 Convert to TEXT then use atoi()
BLOB	 FLOAT	 Convert to TEXT then use atof()
BLOB	 TEXT	 Add a zero terminator if needed
The table above makes reference to standard C library functions atoi() and atof(). SQLite does
not really use these functions. It has its own equivalent internal routines. The atoi() and atof()
names are used in the table for brevity and because they are familiar to most C programmers.

Note that when type conversions occur, pointers returned by prior calls to sqlite3_column_blob(),
sqlite3_column_text(), and/or sqlite3_column_text16() may be invalidated. Type conversions and
pointer invalidations might occur in the following cases:

The initial content is a BLOB and sqlite3_column_text() or sqlite3_column_text16() is called. A
zero-terminator might need to be added to the string.
The initial content is UTF-8 text and sqlite3_column_bytes16() or sqlite3_column_text16() is called.
The content must be converted to UTF-16.
The initial content is UTF-16 text and sqlite3_column_bytes() or sqlite3_column_text() is called.
The content must be converted to UTF-8.
Conversions between UTF-16be and UTF-16le are always done in place and do not invalidate a prior
pointer, though of course the content of the buffer that the prior pointer references will have
been modified. Other kinds of conversion are done in place when it is possible, but sometimes they
are not possible and in those cases prior pointers are invalidated.

The safest and easiest to remember policy is to invoke these routines in one of the following ways:

sqlite3_column_text() followed by sqlite3_column_bytes()
sqlite3_column_blob() followed by sqlite3_column_bytes()
sqlite3_column_text16() followed by sqlite3_column_bytes16()
In other words, you should call sqlite3_column_text(), sqlite3_column_blob(), or
sqlite3_column_text16() first to force the result into the desired format, then invoke
sqlite3_column_bytes() or sqlite3_column_bytes16() to find the size of the result. Do not mix calls
to sqlite3_column_text() or sqlite3_column_blob() with calls to sqlite3_column_bytes16(), and do
not mix calls to sqlite3_column_text16() with calls to sqlite3_column_bytes().

The pointers returned are valid until a type conversion occurs as described above, or until
sqlite3_step() or sqlite3_reset() or sqlite3_finalize() is called. The memory space used to hold
strings and BLOBs is freed automatically. Do not pass the pointers returned sqlite3_column_blob(),
sqlite3_column_text(), etc. into sqlite3_free().

If a memory allocation error occurs during the evaluation of any of these routines, a default value
is returned. The default value is either the integer 0, the floating point number 0.0, or a NULL
pointer. Subsequent calls to sqlite3_errcode() will return SQLITE_NOMEM.

*/
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
