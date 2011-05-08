#include "CommonFunctions.h"

/*
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
*/
void DisplayError(const char* errorNumber, const char* errorType, const char* errorValue)
{
	BString tmpString = errorNumber;
	ErrorAlert* ealert;
	tmpString += ". SqlError: ";
	ealert = new ErrorAlert(tmpString);
	ealert->Launch();
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
int SqlObject::ReturnInt(int returnPlace)
{
	returnplace = returnPlace;
	return sqlite3_column_int(sqlstatement, returnplace);
}
double SqlObject::ReturnDouble(int returnPlace)
{
	returnplace = returnPlace;
	return sqlite3_column_double(sqlstatement, returnplace);
}
int64 SqlObject::ReturnInt64(int returnPlace)
{
	returnplace = returnPlace;
	return sqlite3_column_int64(sqlstatement, returnplace);
}
const char* SqlObject::ReturnText(int returnPlace)
{
	returnplace = returnPlace;
	return sqlite3_mprintf("%s", sqlite3_column_text(sqlstatement, returnplace));
}
const void* SqlObject::ReturnBlob(int returnPlace)
{
	returnplace = returnPlace;
	return sqlite3_column_blob(sqlstatement, returnplace);
}
int64 SqlObject::ReturnLastInsertRowID(void)
{
	return sqlite3_last_insert_rowid(sqldb);
}
int SqlObject::StepSql(void)
{
	sqlcode = sqlite3_step(sqlstatement);
	if(sqlcode != SQLITE_ROW && sqlcode != SQLITE_DONE)
	{
		tmpstring = errornumber;
		tmpstring += " Sql Error: Step Failed";
		ealert = new ErrorAlert(tmpstring);
		ealert->Launch();
	}
	return sqlcode;
}
void SqlObject::ClearBindings(void)
{
	if(sqlite3_clear_bindings(sqlstatement) != SQLITE_OK)
	{
		tmpstring = errornumber;
		tmpstring += " Sql Error: Clear Bindings Failed";
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
// issues to address...
/*
BIND:

If any of the sqlite3_bind_*() routines are called with a NULL pointer for the prepared statement or with a prepared statement for which sqlite3_step() has been called more recently than sqlite3_reset(), then the call will return SQLITE_MISUSE. If any sqlite3_bind_() routine is passed a prepared statement that has been finalized, the result is undefined and probably harmful.

STEP:

In the legacy interface, the return value will be either SQLITE_BUSY, SQLITE_DONE, SQLITE_ROW, SQLITE_ERROR, or SQLITE_MISUSE. With the "v2" interface, any of the other result codes or extended result codes might be returned as well.

SQLITE_BUSY means that the database engine was unable to acquire the database locks it needs to do its job. If the statement is a COMMIT or occurs outside of an explicit transaction, then you can retry the statement. If the statement is not a COMMIT and occurs within a explicit transaction then you should rollback the transaction before continuing.

SQLITE_DONE means that the statement has finished executing successfully. sqlite3_step() should not be called again on this virtual machine without first calling sqlite3_reset() to reset the virtual machine back to its initial state.

If the SQL statement being executed returns any data, then SQLITE_ROW is returned each time a new row of data is ready for processing by the caller. The values may be accessed using the column access functions. sqlite3_step() is called again to retrieve the next row of data.

SQLITE_ERROR means that a run-time error (such as a constraint violation) has occurred. sqlite3_step() should not be called again on the VM. More information may be found by calling sqlite3_errmsg(). With the legacy interface, a more specific error code (for example, SQLITE_INTERRUPT, SQLITE_SCHEMA, SQLITE_CORRUPT, and so forth) can be obtained by calling sqlite3_reset() on the prepared statement. In the "v2" interface, the more specific error code is returned directly by sqlite3_step().

SQLITE_MISUSE means that the this routine was called inappropriately. Perhaps it was called on a prepared statement that has already been finalized or on one that had previously returned SQLITE_ERROR or SQLITE_DONE. Or it could be the case that the same database connection is being used by two or more threads at the same moment in time.

For all versions of SQLite up to and including 3.6.23.1, a call to sqlite3_reset() was required after sqlite3_step() returned anything other than SQLITE_ROW before any subsequent invocation of sqlite3_step(). Failure to reset the prepared statement using sqlite3_reset() would result in an SQLITE_MISUSE return from sqlite3_step(). But after version 3.6.23.1, sqlite3_step() began calling sqlite3_reset() automatically in this circumstance rather than returning SQLITE_MISUSE. This is not considered a compatibility break because any application that ever receives an SQLITE_MISUSE error is broken by definition. The SQLITE_OMIT_AUTORESET compile-time option can be used to restore the legacy behavior.

INTERRUPT:

void sqlite3_interrupt(sqlite3*);
This function causes any pending database operation to abort and return at its earliest opportunity. This routine is typically called in response to a user action such as pressing "Cancel" or Ctrl-C where the user wants a long query operation to halt immediately.

It is safe to call this routine from a thread different from the thread that is currently running the database operation. But it is not safe to call this routine with a database connection that is closed or might close before sqlite3_interrupt() returns.

If an SQL operation is very nearly finished at the time when sqlite3_interrupt() is called, then it might not have an opportunity to be interrupted and might continue to completion.

An SQL operation that is interrupted will return SQLITE_INTERRUPT. If the interrupted SQL operation is an INSERT, UPDATE, or DELETE that is inside an explicit transaction, then the entire transaction will be rolled back automatically.

The sqlite3_interrupt(D) call is in effect until all currently running SQL statements on database connection D complete. Any new SQL statements that are started after the sqlite3_interrupt() call and before the running statements reaches zero are interrupted as if they had been running prior to the sqlite3_interrupt() call. New SQL statements that are started after the running statement count reaches zero are not effected by the sqlite3_interrupt(). A call to sqlite3_interrupt(D) that occurs when there are no running SQL statements is a no-op and has no effect on SQL statements that are started after the sqlite3_interrupt() call returns.

If the database connection closes while sqlite3_interrupt() is running then bad things will likely happen.

FINALIZE:

The sqlite3_finalize() function is called to delete a prepared statement. If the most recent evaluation of the statement encountered no errors or or if the statement is never been evaluated, then sqlite3_finalize() returns SQLITE_OK. If the most recent evaluation of statement S failed, then sqlite3_finalize(S) returns the appropriate error code or extended error code.

The sqlite3_finalize(S) routine can be called at any point during the life cycle of prepared statement S: before statement S is ever evaluated, after one or more calls to sqlite3_reset(), or after any call to sqlite3_step() regardless of whether or not the statement has completed execution.

Invoking sqlite3_finalize() on a NULL pointer is a harmless no-op.

The application must finalize every prepared statement in order to avoid resource leaks. It is a grievous error for the application to try to use a prepared statement after it has been finalized. Any use of a prepared statement after it has been finalized can result in undefined and undesirable behavior such as segfaults and heap corruption.

RESET:

The sqlite3_reset() function is called to reset a prepared statement object back to its initial state, ready to be re-executed. Any SQL statement variables that had values bound to them using the sqlite3_bind_*() API retain their values. Use sqlite3_clear_bindings() to reset the bindings.

The sqlite3_reset(S) interface resets the prepared statement S back to the beginning of its program.

If the most recent call to sqlite3_step(S) for the prepared statement S returned SQLITE_ROW or SQLITE_DONE, or if sqlite3_step(S) has never before been called on S, then sqlite3_reset(S) returns SQLITE_OK.

If the most recent call to sqlite3_step(S) for the prepared statement S indicated an error, then sqlite3_reset(S) returns an appropriate error code.

The sqlite3_reset(S) interface does not change the values of any bindings on the prepared statement S.

RETURN:

If the SQL statement does not currently point to a valid row, or if the column index is out of range, the result is undefined. These routines may only be called when the most recent call to sqlite3_step() has returned SQLITE_ROW and neither sqlite3_reset() nor sqlite3_finalize() have been called subsequently. If any of these routines are called after sqlite3_reset() or sqlite3_finalize() or after sqlite3_step() has returned something other than SQLITE_ROW, the results are undefined. If sqlite3_step() or sqlite3_reset() or sqlite3_finalize() are called from a different thread while any of these routines are pending, then the results are undefined.

CLOSE:

int sqlite3_close(sqlite3 *);
The sqlite3_close() routine is the destructor for the sqlite3 object. Calls to sqlite3_close() return SQLITE_OK if the sqlite3 object is successfully destroyed and all associated resources are deallocated.

Applications must finalize all prepared statements and close all BLOB handles associated with the sqlite3 object prior to attempting to close the object. If sqlite3_close() is called on a database connection that still has outstanding prepared statements or BLOB handles, then it returns SQLITE_BUSY.

If sqlite3_close() is invoked while a transaction is open, the transaction is automatically rolled back.

The C parameter to sqlite3_close(C) must be either a NULL pointer or an sqlite3 object pointer obtained from sqlite3_open(), sqlite3_open16(), or sqlite3_open_v2(), and not previously closed. Calling sqlite3_close() with a NULL pointer argument is a harmless no-op.

OPEN:

may want to open and close everytime i need it or just open and close once, but i must reset, finalize and clear all bindings
with each prepare... i'm not doing that properly right now and i think that is where i am getting into trouble... 
also might to expand my error statements to include the different error responses... such as if returnvalue = sqlite_busy then
tmpstring += " BUSY...", etc..

also might want to expand my error wrapper, to simply call DisplayError("15", "PREPARE", "BUSY"); which in turn calls the erroralert and populates the tmpstring with 15: Sql Error: Prepare Failed with Busy" built and then calls the launch, so my stuff is only 1 line versus 4 or 5 everytime i need to call an error for the wrapper.

multiple processes can read from a single open db, but only process can write to it and locks the db.  so i think the db gets locked, the busy command comes back so the process never finishes and the db gets stuck in a locked state.

i will modify wrapper class to open connection with OpenSql("READWRITE", "CREATE") and close with CloseSql() and any variables, etc... and then i will open and close with every need for data and then also re-initialize the sqlobject everytime i need it as well.  this will be safest and i can clear/reset/finalize without any locks or problems, hopefully...

*/
