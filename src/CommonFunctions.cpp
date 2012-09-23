#include "CommonFunctions.h"

using namespace pyembed;

void DisplayError(const char* errorNumber, const char* errorType, const char* errorValue)
{
	BString tmpString = errorNumber;
	ErrorAlert* ealert;
	tmpString += ". SqlError: ";
	tmpString += errorType;
	tmpString += " Returned ";
	tmpString += errorValue;
	ealert = new ErrorAlert(tmpString);
	ealert->Launch();
}
BString GetAppDirPath(void)
{
	app_info info;
	be_app->GetAppInfo(&info);
	BPath path(&info.ref);
	path.GetParent(&path);
	return path.Path();
}
BString GetUserDirPath(void)
{
	BPath tmpUserPath;
	status_t result = find_directory(B_USER_DIRECTORY, &tmpUserPath);
	if(result == B_OK) return tmpUserPath.Path();
	else return "-15";
}
bool CheckExistingScripts(const char* scripttype)
{
	int scriptExist = 0;
	BFile testFile;
	BString tmpPath = GetAppDirPath();
	tmpPath += "/pub";
	tmpPath += scripttype;
	tmpPath += ".py";
	testFile.SetTo(tmpPath, B_READ_ONLY);
	if(testFile.InitCheck() == B_OK) scriptExist = 1;
	testFile.Unset();
	return scriptExist;
}
void ExecutePreview(BString tmpData)
{
	int argc = 1;
	char* argvv = "ladida";
	char** argv = &argvv;
	Python py(argc, argv);
	BString tmpPath; // string path of the tmppub.tht file then string path of tmppub.ext file
	BFile previewFile; // tmppub.tht file
	ErrorAlert* eAlert;
	tmpPath = GetAppDirPath();
	tmpPath += "/tmp.tht";
	previewFile.SetTo(tmpPath, B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE); // B_ERASE_FILE
	if(previewFile.InitCheck() != B_OK)
	{
		eAlert = new ErrorAlert("4.2 Builder Error: Couldn't Write TMP File.");
		eAlert->Launch();
		//printf("Couldn't write file\n");
	}
	previewFile.Write(tmpData, strlen(tmpData));
	previewFile.Unset();
	try
	{
		py.run_file("preview.py");
	}
	catch(Python_exception ex)
	{
		eAlert = new ErrorAlert("4.3 Builder Error: Python Issue - ", ex.what());
		eAlert->Launch();
	}
	
	tmpPath = "/boot/apps/WebPositive file://";
	tmpPath += GetAppDirPath();
	tmpPath += "/tmp.html &";
	system(tmpPath);
}

void ExecutePublish(BMessage* tmpMsg, BString tmpData, BString tmpExt)
{
	ErrorAlert* eAlert;
	const char* name;
	BEntry entry;
	BPath path;
	entry_ref ref;
	int argc = 1;
	char* argvv = "ladida";
	char** argv = &argvv;
	Python py(argc, argv);
	BString publishPath; // user generated filename
	BString tmpPath; // string path of tmppub.tht file, then string path of tmppub.ext
	BFile previewFile; // tmppub.tht file
	BString scriptFile; // python script file name
	BString runPath; // rst2pdf execute path
	BString dirPath; // user created directory path string
	BEntry publishFile; // file that is renamed to the new user generated filename from tmppath
	BEntry removeTmpFile; // tmp file that information that will be removed
	BEntry removeOldFile; // tmp final file that will be removed from appdir
	BDirectory publishDirectory; // user generated directory
	BString oldFilePath; // path to the renamed tmpfile
	BString newFilePath; // path to the actual saved file
	status_t err; // auto errors
	tmpPath = GetAppDirPath();
	tmpPath += "/tmppub.tht";
	removeTmpFile.SetTo(tmpPath);
	previewFile.SetTo(tmpPath, B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE); // B_ERASE_FILE
	if(previewFile.InitCheck() != B_OK)
	{
		//printf("couldn't read file\n");
		eAlert = new ErrorAlert("3.4 Editor Error: Couldn't Create Pub File.");
		eAlert->Launch();
	}
	previewFile.Write(tmpData, strlen(tmpData));
	previewFile.Unset();

	if(tmpExt == "pdf")
	{
		printf(" PDF RUN\n");
		runPath = "/boot/common/bin/rst2pdf ";
		runPath += GetAppDirPath();
		runPath += "/tmppub.tht -o ";
		runPath += GetAppDirPath();
		runPath += "/tmppub.pdf";
		//printf(tmpPath);
		//printf("\n");
		system(runPath);
	}
	else
	{
		printf(" NOT PDF RUN\n");
			
		// build the correct publish python script name
		scriptFile = "pub";
		scriptFile += tmpExt;
		scriptFile += ".py";
		printf(scriptFile);
		printf("\n");
		tmpPath = GetAppDirPath();
		tmpPath += "/tmppub.";
		tmpPath += tmpExt;
		//printf(tmpExt);
		//printf("\n");
		try
		{
			py.run_file(scriptFile.String());
		}
		catch(Python_exception ex)
		{
			//printf("Python error: %s\n", ex.what());
			eAlert = new ErrorAlert("3.5 Editor Error: Python Issue - ", ex.what());
			eAlert->Launch();
			err = removeTmpFile.Remove();
			if(err != B_OK)
			{
				eAlert = new ErrorAlert("3.14 Editor Error: Tmp File could not be removed due to: ", strerror(err));
				eAlert->Launch();
			}
		}
	}
	
	// now i need to get the finished file and mv/rename it to the correct location
	if(tmpMsg->FindString("name", &name) == B_OK)
	{
		//printf("default save message: %s\n", name);
	}
	if(tmpMsg->FindRef("directory", &ref) == B_OK)
	{
		tmpPath = GetAppDirPath();
		tmpPath += "/tmppub.";
		tmpPath += tmpExt;
		printf(" Current tmppath: ");
		printf(tmpPath);
		printf("\n");
		publishPath = name;
		publishPath.Append(".");
		publishPath.Append(tmpExt);
		//printf(publishPath);
		publishFile.SetTo(tmpPath);
		publishFile.Rename(publishPath, true);
		oldFilePath = GetAppDirPath();
		oldFilePath += "/tmppub";
		//oldFilePath += name;
		oldFilePath += ".";
		oldFilePath += tmpExt;
		//printf("Tmp Path: %s\nPublishPath: %s\n", tmpPath.String(), publishPath.String());
		entry.SetTo(&ref); // directory where the file is to be saved as defined by user
		entry.SetTo(&ref);
		entry.GetPath(&path);
		dirPath = path.Path();
		dirPath += "/";				
		newFilePath = dirPath;
		newFilePath += name;
		newFilePath += ".";
		newFilePath += tmpExt;
		printf("old file: %s\n", oldFilePath.String());
		printf("new file: %s\n", newFilePath.String());
		if(publishDirectory.SetTo(dirPath) == B_OK) // set publish directory to the user created directory
		{
			//printf("publishdirectory %s\n", path.Path());
			//printf("successful directory set\n");
			err = publishFile.MoveTo(&publishDirectory, NULL, true); // move publish file to publish directory
			if(err != B_OK)
			{
				if(err == B_CROSS_DEVICE_LINK)
				{
					BFile oldFile;
					BFile newFile;
					if(oldFile.SetTo(oldFilePath, B_READ_ONLY) == B_OK)
					{
						if(newFile.SetTo(newFilePath, B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE) == B_OK)
						{
							off_t length;
							char* text;
							oldFile.GetSize(&length);
							text = (char*) malloc(length);
							if(text && oldFile.Read(text, length) >= B_OK) // write text to the newfile
							{
								err = newFile.Write(text, length);
								if(err >= B_OK)
								{
									eAlert = new ErrorAlert("3.13 Editor Error: File could not be written due to: ", strerror(err));
									eAlert->Launch();		
								}
								else
								{
									removeOldFile.SetTo(oldFilePath);
									err = removeOldFile.Remove();
									if(err != B_OK)
									{
										eAlert = new ErrorAlert("3.14 Editor Error: Tmp File could not be removed due to: ", strerror(err));
										eAlert->Launch();
									}
								}
							}
							free(text);
						}
					}
				}
				else
				{
					eAlert = new ErrorAlert("3.15 Editor Error: File could not be written due to: ", strerror(err));
					eAlert->Launch();		
				}
			}
		}
		else
		{
			eAlert = new ErrorAlert("3.6 Editor Error: Directory Set Failed");
			eAlert->Launch();
			//printf("directory set failed");
		}
	}
	// clean up the temporary files...
	err = removeTmpFile.Remove();
	if(err != B_OK)
	{
		eAlert = new ErrorAlert("3.14 Editor Error: Tmp File could not be removed due to: ", strerror(err));
		eAlert->Launch();
	}
}

SqlObject::SqlObject(sqlite3_stmt* sqlStatement, const char* errorNumber, sqlite3* openDB)
{
	sqldb = openDB;
	sqlstatement = sqlStatement;
	errornumber = errorNumber;
}
SqlObject::SqlObject(sqlite3_stmt* sqlStatement, const char* errorNumber)
{
	sqldb = NULL;
	sqlstatement = sqlStatement;
	errornumber = errorNumber;
	char* sqlErrMsg;
	int	sqlValue;
	BString tmpString;
	sqlErrMsg = 0;
	BString tmpPath = GetAppDirPath(); // for testing purposes, use local one.
	//BString tmpPath = GetUserDirPath(); // for publish purposes, use real location
	if(tmpPath != "-15")
	{
		tmpPath += "/MasterPiece.db";
		sqlValue = sqlite3_open_v2(tmpPath, &sqldb, SQLITE_OPEN_READWRITE, NULL); // open db
		if(sqlite3_errcode(sqldb) == 14) // if error is SQLITE_CANTOPEN, then create db with structure
		{
			sqlValue = sqlite3_open_v2(tmpPath, &sqldb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL); 
			if(sqlite3_errcode(sqldb) == 0) // sqlite_ok
			{
				tmpString = "CREATE TABLE ideatable(ideaid integer primary key autoincrement, ideaname text, ideatext text, ismp integer, mpid integer, ordernumber integer);";
				sqlValue = sqlite3_exec(sqldb, tmpString, NULL, NULL, &sqlErrMsg);
				if(sqlValue != SQLITE_OK) // if sql was not successful
				{
					DisplayError("1.1", "OPEN", sqlErrMsg);
				}
			}
			else // some kind of failure
			{
				DisplayError("1.0", "OPEN", sqlite3_errmsg(sqldb));
			}
		}
		else if(sqlite3_errcode(sqldb) == 0) // sqlite_OK, it exists
		{
			//no error, so i will return opendb at end;
		}
		else // if error is not ok or not existing, then display error in alert
		{
			DisplayError("1.2", "OPEN", sqlite3_errmsg(sqldb));
		}
		sqlite3_free(sqlErrMsg);
	}
	else
	{
		DisplayError("1.0", "PATH", " to the User Directory not Found");
	}
}
SqlObject::~SqlObject(void)
{
}
void SqlObject::PrepareSql(const char* sqlQuery)
{
	sqlquery = sqlQuery;
	if(sqlite3_prepare_v2(sqldb, sqlquery, -1, &sqlstatement, NULL) != SQLITE_OK) // sql statement was not prepared
	{
		DisplayError(errornumber, "PREPARE", sqlite3_errmsg(sqldb));
	}
}
void SqlObject::BindValue(int bindPlace, int bindValue)
{
	bindplace = bindPlace;
	bindint = bindValue;
	if(sqlite3_bind_int(sqlstatement, bindplace, bindint) != SQLITE_OK) // sql int bind failed
	{
		DisplayError(errornumber, "BIND INT", "MISUSE");
	}
}
void SqlObject::BindValue(int bindPlace, double bindValue)
{
	bindplace = bindPlace;
	binddouble = bindValue;
	if(sqlite3_bind_double(sqlstatement, bindplace, binddouble) != SQLITE_OK) // sql double bind failed
	{
		DisplayError(errornumber, "BIND DOUBLE", "MISUSE");
	}
}
void SqlObject::BindValue(int bindPlace, int64 bindValue)
{
	bindplace = bindPlace;
	bindint64 = bindValue;
	if(sqlite3_bind_int64(sqlstatement, bindplace, bindint64) != SQLITE_OK) // sql int64 bind failed
	{
		DisplayError(errornumber, "BIND INT64", "MISUSE");
	}
}
void SqlObject::BindValue(int bindPlace, const char* bindValue)
{
	bindplace = bindPlace;
	bindstring = bindValue;
	if(sqlite3_bind_text(sqlstatement, bindplace, bindstring, -1, SQLITE_TRANSIENT) != SQLITE_OK) // sql text bind failed
	{
		DisplayError(errornumber, "BIND TEXT", "MISUSE");
	}
}
void SqlObject::BindValue(int bindPlace) // bind null
{
	bindplace = bindPlace;
	if(sqlite3_bind_null(sqlstatement, bindplace) != SQLITE_OK)
	{
		DisplayError(errornumber, "BIND NULL", "MISUSE");
	}
}
void SqlObject::BindValue(int bindPlace, const void* bindValue)
{
	bindplace = bindPlace;
	bindblob = bindValue;
	if(sqlite3_bind_blob(sqlstatement, bindplace, bindblob, -1, SQLITE_TRANSIENT) != SQLITE_OK) // sql blob bind failed
	{
		DisplayError(errornumber, "BIND BLOB", "MISUSE");
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
		if(sqlcode == SQLITE_BUSY) DisplayError(errornumber, "STEP", "BUSY");
		else if(sqlcode == SQLITE_ERROR) DisplayError(errornumber, "STEP", "ERROR");
		else if(sqlcode == SQLITE_MISUSE) DisplayError(errornumber, "STEP", "MISUSE");
		else DisplayError(errornumber, "STEP", "OTHER ISSUE");
	}
	return sqlcode;
}
void SqlObject::ClearBindings(void)
{
	if(sqlite3_clear_bindings(sqlstatement) != SQLITE_OK)
	{
		DisplayError(errornumber, "CLEAR", "ERROR");
	}
}
void SqlObject::ResetSql(void)
{
	if(sqlite3_reset(sqlstatement) != SQLITE_OK)
	{
		DisplayError(errornumber, "RESET", "ERROR");
	}
}
void SqlObject::FinalizeSql(void)
{
	if(sqlite3_finalize(sqlstatement) != SQLITE_OK)
	{
		DisplayError(errornumber, "FINALIZE", "ERROR");
	}
}
void SqlObject::CloseSql(void)
{
	sqlcode = sqlite3_close(sqldb);
	if(sqlcode != SQLITE_OK)
	{
		if(sqlcode == SQLITE_BUSY) DisplayError(errornumber, "CLOSE", "BUSY");
		else if(sqlcode == SQLITE_ERROR) DisplayError(errornumber, "CLOSE", "ERROR");
		else DisplayError(errornumber, "CLOSE", "OTHER");
	}
}
sqlite3* SqlObject::ReturnSqlDB(void)
{
	return sqldb;
}
