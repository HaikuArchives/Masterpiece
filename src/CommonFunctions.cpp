#include "CommonFunctions.h"

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
				//return opendb;
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
		//return opendb;
	}
	else // if error is not ok or not existing, then display error in alert
	{
		eAlert = new ErrorAlert("1.2 Sql Error: ", sqlite3_errmsg(opendb));
		eAlert->Launch();
	}
	return opendb;
}
