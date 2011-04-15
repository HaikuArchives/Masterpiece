#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include <sqlite3.h>
#include <Application.h>
#include <Roster.h>
#include <Path.h>
#include <String.h>

#include "ErrorAlert.h"

sqlite3*	OpenSqliteDB(void);
int			PrepareSql(sqlite3* tmpdb, const char* sqlstatement, sqlite3_stmt** tmpStatement, int tmpInt=-1, const char** unused=NULL);


#endif
