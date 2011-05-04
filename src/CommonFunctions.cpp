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
