#include "CommonFunctions.h"

sqlite3* OpenSqliteDB()
{
	char*		sqlErrMsg;
	int			sqlValue;
	sqlite3* 	opendb;
	
	sqlErrMsg = 0;
	app_info info;
	be_app->GetAppInfo(&info);
	BPath path(&info.ref);
	path.GetParent(&path);
	BString tmpPath = path.Path();
	tmpPath += "/MasterPiece.db";
	sqlValue = sqlite3_open_v2(tmpPath, &mpdb, SQLITE_OPEN_READWRITE, NULL); // open db
	if(sqlite3_errcode(mpdb) == 14) // if error is SQLITE_CANTOPEN, then create db with structure
	{
		sqlValue = sqlite3_open_v2(tmpPath, &mpdb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL); 
		if(sqlite3_errcode(mpdb) == 0) // sqlite_ok
		{
			tmpString = "CREATE TABLE ideatable(ideaid integer primary key autoincrement, ideaname text, ideatext text, ismp integer, mpid integer, ordernumber integer);";
			sqlValue = sqlite3_exec(mpdb, tmpString, NULL, NULL, &sqlErrMsg);
			if(sqlValue == SQLITE_OK) // if sql was successful
			{
				eAlert = new ErrorAlert("sql was created successfully");
				eAlert->Launch();
			}
			else // sql not successful
			{
				eAlert = new ErrorAlert("1.1 Sql Error: ", sqlErrMsg);
				eAlert->Launch();
			}
		}
		else // some kind of failure
		{
			eAlert = new ErrorAlert("1.0 Sql Error: ", sqlite3_errmsg(mpdb));
			eAlert->Launch();
		}
	}
	else if(sqlite3_errcode(mpdb) == 0) // sqlite_OK, it exists
	{
		sqlValue = sqlite3_prepare_v2(mpdb, "select ideaname, ideaid from ideatable where ismp = 1", -1, &ideaStatement, NULL);
		if(sqlValue == SQLITE_OK) // sql query was successful
		{
			while(sqlite3_step(ideaStatement) == SQLITE_ROW)
			{
				tmpString = sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 0));
				openMasterpieceListView->AddItem(new MPStringItem(tmpString, sqlite3_column_int(ideaStatement, 1)));
			}
			openMasterpieceListView->SetInvocationMessage(new BMessage(OPEN_EXISTING_MP));
		}
		else // sql select failed
		{
			eAlert = new ErrorAlert("No Masterpiece Exist. Please Create One First.");
			eAlert->Launch();
		}
		sqlValue = sqlite3_prepare_v2(mpdb, "select ideaname, ideaid from ideatable where ismp = 0", -1, &ideaStatement, NULL);
		if(sqlValue == SQLITE_OK) // sql statement was prepared
		{
			while(sqlite3_step(ideaStatement) == SQLITE_ROW)
			{
				tmpString = sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 0));
				openThoughtListView->AddItem(new MPStringItem(tmpString, sqlite3_column_int(ideaStatement, 1)));
			}
			openThoughtListView->SetInvocationMessage(new BMessage(OPEN_EXISTING_THT));
		}
		else
		{
			eAlert = new ErrorAlert("No Thoughts Exist.  Please Create One First.");
			eAlert->Launch();
		}
		sqlite3_finalize(ideaStatement);
	}
	else // if error is not ok or not existing, then display error in alert
	{
		eAlert = new ErrorAlert("1.2 Sql Error: ", sqlite3_errmsg(mpdb));
		eAlert->Launch();
	}	
	return opendb;
}
