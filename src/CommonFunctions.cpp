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
void SqlObject::BindInt(int bindPlace, int bindValue)
{
	bindplace = bindPlace;
	bindvalue = bindValue;
	if(sqlite3_bind_int(sqlstatement, bindplace, bindvalue) != SQLITE_OK) // sql int bind failed
	{
		tmpstring = errornumber;
		tmpstring += " Sql Error: Bind Int Failed";
		ealert = new ErrorAlert(tmpstring);
		ealert->Launch();
	}
}
void SqlObject::StepSql(void)
{
	sqlcode = sqlite3_step(sqlstatement);
	if(sqlcode == SQLITE_ROW)
	{
		// do something here
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
	sqlite3_reset(sqlstatement);
}
void SqlObject::FinalizeSql(void)
{
	sqlite3_finalize(sqlstatement);
}

int PrepareSql2(sqlite3* tmpdb, const char* sqlquery, sqlite3_stmt** tmpstatement, const char* errornumber, int tmpint=-1, const char** unused=NULL)
{
	ErrorAlert* eAlert;
	BString		tmpString;
	
	if(sqlite3_prepare_v2(tmpdb, sqlquery, tmpint, tmpstatement, unused) == SQLITE_OK) // sql statement was prepared
	{
		return SQLITE_OK;
	}
	else
	{
		tmpString = errornumber;
		tmpString += " Sql Error: Prepare Statement Failed";
		eAlert = new ErrorAlert(tmpString);
		eAlert->Launch();
		return -15;
	}
}
// might not be able to step since it iterates... will have to look at that.
int StepSql2(sqlite3_stmt* tmpstatement, const char* errornumber)
{
	ErrorAlert* eAlert;
	BString		tmpString;
	int			tmpReturn;
	
	tmpReturn = sqlite3_step(tmpstatement);
	if(tmpReturn == SQLITE_ROW)
	{
		return SQLITE_ROW;
	}
	else if(tmpReturn == SQLITE_DONE)
	{
		// do stuff here prior to reset call...
		sqlite3_reset(tmpstatement);
		return SQLITE_DONE;
	}
	else
	{
		tmpString = errornumber;
		tmpString += " Sql Error: Step Failed";
		eAlert = new ErrorAlert(tmpString);
		eAlert->Launch();
		return -15;
	}
}
int BindInteger2(sqlite3_stmt* tmpstatement, int bindplace, int bindvalue, const char* errornumber)
{
	ErrorAlert* eAlert;
	BString		tmpString;
	
	if(sqlite3_bind_int(tmpstatement, bindplace, bindvalue) == SQLITE_OK)
	{
		return SQLITE_OK;
	}
	else
	{
		tmpString = errornumber;
		tmpString += " Sql Error: Bind Integer Failed";
		eAlert =  new ErrorAlert(tmpString);
		eAlert->Launch();
		return -15;
	}
}
int RunSql2(sqlite3* tmpdb, const char* sqlquery, sqlite3_stmt** tmpstatement, const char* errornumber, int bindcount, int tmpint=-1, const char** unused=NULL)
{
	return -15;
}
