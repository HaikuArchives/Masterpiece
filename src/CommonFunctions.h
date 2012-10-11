#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include "pyembed.h"

#include <Alert.h>
#include <Application.h>
#include <Button.h>
#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <Font.h>
#include <GridLayoutBuilder.h>
#include <GroupLayout.h>
#include <ListView.h>
#include <malloc.h>
#include <Message.h>
#include <Messenger.h>
#include <Path.h>
#include <Roster.h>
#include <ScrollView.h>
#include <SpaceLayoutItem.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <String.h>
#include <StringView.h>
#include <TextView.h>
#include <View.h>
#include <Window.h>
#include <OS.h>

#define SET_STATUS	'stst' // set status for builder/editor window

void	DisplayError(const char* errorNumber, const char* errorType, const char* errorValue);
void	TmpCleanUp(BString tmpExt);
void	ExecutePreview(BString tmpData);
void	ExecutePublish(BMessage* tmpMsg, BString tmpData, BString tmpExt);
BString	GetAppDirPath(void);
BString GetUserDirPath(void);

class ErrorAlert
{
public:
				ErrorAlert(BString tmpText);
				ErrorAlert(BString tmpText1, BString tmpText2);
				~ErrorAlert(void);
		int 	Launch(void);			
		BAlert*	tmpAlert;
};

class SqlObject
{
	public:
								SqlObject(sqlite3_stmt* sqlStatement, const char* errorNumber);
								SqlObject(sqlite3_stmt* sqlStatement, const char* errorNumber, sqlite3* openDB);
								~SqlObject(void);
				void			PrepareSql(const char* sqlQuery);
				void			BindValue(int bindPlace, int bindValue);
				void			BindValue(int bindPlace, double bindValue);
				void			BindValue(int bindPlace, int64 bindValue);
				void			BindValue(int bindPlace, const char* bindValue);
				void			BindValue(int bindPlace, const void* bindValue);// blob bind
				void			BindValue(int bindPlace);
				int				ReturnInt(int returnPlace);
				double			ReturnDouble(int returnPlace);
				int64			ReturnInt64(int returnPlace);
				const char*		ReturnText(int returnPlace);
				const void*		ReturnBlob(int returnPlace);
				int				StepSql(void);
				void			ClearBindings(void);
				int64			ReturnLastInsertRowID(void);
				void			ResetSql(void);
				void			FinalizeSql(void);
				void			CloseSql(void);
				sqlite3*		ReturnSqlDB(void);
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
