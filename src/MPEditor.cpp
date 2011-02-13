#include "MPEditor.h"

MPEditor::MPEditor(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID)
	:	BWindow(BRect(100, 100, 900, 700), windowTitle, B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE), launcherMessage(msg), launcherMessenger(msgr)
{
	BRect r = Bounds();
	r.bottom = r.bottom - 50;
	editorTextView = new BTextView(r, NULL, r, B_FOLLOW_ALL, B_WILL_DRAW);	
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	
	backView->SetLayout(new BGroupLayout(B_HORIZONTAL, 0.0));
	backView->AddChild(BGridLayoutBuilder()
		.Add(new EditorMenu(), 0, 0)
		.Add(new BScrollView("scroll_editor", editorTextView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 0, 1)
		.SetInsets(0, 0, 0, 0)
	);
	currentideaID = ideaID;
}
void MPEditor::MessageReceived(BMessage* msg)
{
	BRect r(Bounds());
	switch(msg->what)
	{
		case MENU_NEW_THT:
			tmpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Editor - untitled", -1);
			tmpEditor->Show();
			break;
		case MENU_EDT_THT:
			printf(" must open edit name dialog\r\n");
			break;
		case MENU_SAV_THT:
			if(currentideaID == -1)
			{
				printf("need to write sql for idea, then must open save dialog to set name\r\n");
				xPos = (r.right - r.left) / 2;
				yPos = (r.bottom - r.top) / 2;
				saveIdea = new SaveIdea(xPos, yPos, currentideaID);
				saveIdea->Show();
			}
			else
			{
				// need to open mpdb first...
				//sqlValue = sqlite3_prepare_v2(mpdb, "update ideatable set ideatext = ? where ideaid = ?", -1, &ideaStatement, NULL);
				printf("must just write sql\r\n");
			}
			break;
		case MENU_PRV_THT:
			printf("save data, export to python html one and open data in preview window or webpositive");
			break;
		case MENU_PUB_THT:
			printf("save data, open publish to window, export to python and save as name in publish window");
			break;
		case MENU_HLP_THT:
			printf("open help topic window");
			break;
		case MENU_KEY_THT:
			printf("open keyboard reference window");
			break;
		case MENU_MRK_THT:
			printf("open markup reference window");
			break;
		case MENU_ABT_THT:
			printf("open about window");
			break;
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}
bool MPEditor::QuitRequested(void)
{
	launcherMessage.MakeEmpty();
	launcherMessage.AddInt64("showLauncher", 1);
	launcherMessenger.SendMessage(&launcherMessage);
	return true;
}
/*
void MPLauncher::OpenMasterpieceDB()
{
	sqlErrMsg = 0;
	app_info info;
	be_app->GetAppInfo(&info);
	BPath path(&info.ref);
	path.GetParent(&path);
	BStrin
	g tmpPath = path.Path();
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
}
*/
