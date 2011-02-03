#include "MPLauncher.h"

MPLauncher::MPLauncher(void)
	:	BWindow(BRect(100, 100, 650, 400), "MasterPiece Launcher", B_TITLED_WINDOW,  B_NOT_H_RESIZABLE | B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE)
{
	mainGroup = new BGroupLayout(B_HORIZONTAL, 0.0);
	newThoughtButton = new BButton(BRect(10, 10, 90, 35), NULL, "Create a New...", new BMessage(CREATE_NEW_THT), B_FOLLOW_NONE, B_WILL_DRAW);
	newMasterpieceButton = new BButton(BRect(10, 10, 90, 35), NULL, "Create a New...", new BMessage(CREATE_NEW_MP), B_FOLLOW_NONE, B_WILL_DRAW);
	thoughtStringView = new BStringView(BRect(10, 10, 200, 30), NULL, "Work on a Thought");
	masterpieceStringView = new BStringView(BRect(10, 10, 200, 30), NULL, "Work on a Masterpiece");
	openMasterpieceStringView = new BStringView(BRect(10, 10, 200, 30), NULL, "Open an Existing...");
	openThoughtStringView = new BStringView(BRect(10, 10, 200, 30), NULL, "Open an Existing...");
	openThoughtListView = new BListView(BRect(10, 10, 100, 30), NULL, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW);
	openMasterpieceListView = new BListView(BRect(10, 10, 100, 30), NULL, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW);
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	
	backView->SetLayout(mainGroup);
	backView->AddChild(BGridLayoutBuilder()
		.Add(masterpieceStringView, 0, 0)
		.Add(BSpaceLayoutItem::CreateGlue(), 1, 0)
		.Add(thoughtStringView, 2, 0)
		.Add(BSpaceLayoutItem::CreateGlue(), 3, 0)
		.Add(newMasterpieceButton, 0, 1)
		.Add(BSpaceLayoutItem::CreateGlue(), 1, 0)
		.Add(newThoughtButton, 2, 1)
		.Add(openMasterpieceStringView, 0, 2)
		.Add(BSpaceLayoutItem::CreateGlue(), 1, 2)
		.Add(openThoughtStringView, 2, 2)
		.Add(BSpaceLayoutItem::CreateGlue(), 3, 2)
		.Add(new BScrollView("scroll_masterpiecelist", openMasterpieceListView,  B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 0, 3, 2, 3)
		.Add(new BScrollView("scroll_thoughtlist", openThoughtListView,  B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 2, 3, 2, 3)
		.SetInsets(5, 5, 5, 2)
	);
	//openMasterpieceListView->SetInvocationMessage(new BMessage(OPEN_EXISTING_MP));
	//openThoughtListView->SetInvocationMessage(new BMessage(OPEN_EXISTING_THT));
	openMasterpieceListView->MakeEmpty();
	openThoughtListView->MakeEmpty();
	
	OpenMasterpieceDB();
}
void MPLauncher::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case CREATE_NEW_MP:
			mpBuilder = new MPBuilder(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Builder - untitled");
			mpBuilder->Show();
			this->Hide();
			// do something here
			break;
		case CREATE_NEW_THT:
			mpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Editor - untitled");
			mpEditor->Show();
			this->Hide();
			// do something here
			break;
		case OPEN_EXISTING_MP:
			selected = openMasterpieceListView->CurrentSelection(); // list item value
			if(selected < 0)
			{
				eAlert = new ErrorAlert("No MP to select");
				eAlert->Launch();
			}
			else
			{
				mpBuilder = new MPBuilder(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Builder - untitled");
				mpBuilder->Show();
				this->Hide();
			}
			break;
		case OPEN_EXISTING_THT:
			mpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Editor - untitled");
			mpEditor->Show();
			this->Hide();
			// do something here
			break;
		case SHOW_LAUNCHER:
			// do something here
			if(msg->FindInt64("showLauncher", &showLauncher) == B_OK)
			{
				if(showLauncher == 1)
				{
					this->Show();
				}
				else if(showLauncher == 0)
				{
					this->Hide();
				}
				else
				{
					eAlert = new ErrorAlert("Message is not 0 or 1");
					eAlert->Launch();
					// big error must display 
				}
			}
			else
			{
				eAlert = new ErrorAlert("Message is not right");
				eAlert->Launch();
			}
			break;
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}		
	}
}
bool MPLauncher::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
void MPLauncher::OpenMasterpieceDB()
{
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
				// nothing to select, need to remove invocationmessage...
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
		// determine if mp's and or thoughts exist, then set invocation message as necessary
		tmpString = "select ideaname, ideaid from ideatable where ismp = 1";
		sqlValue = sqlite3_get_table(mpdb, tmpString, &selectResult, &nrow, &ncol, &sqlErrMsg);
		if(sqlValue == SQLITE_OK) // sql query was successful
		{
			for(int i = 0; i < nrow; i++)
			{
				tmpString = selectResult[(i*ncol) + 2];
				tmpString += ". ";
				tmpString += selectResult[(i*ncol) + 3];
				this->openMasterpieceListView->AddItem(new BStringItem(tmpString));
			}
		}
		else // sql select failed
		{
			eAlert = new ErrorAlert("No Masterpiece Exist. Please Create One First.");
			eAlert->Launch();
		}
		openMasterpieceListView->SetInvocationMessage(new BMessage(OPEN_EXISTING_MP));
		openThoughtListView->SetInvocationMessage(new BMessage(OPEN_EXISTING_THT));
		// populate mpview with mp's
	}
	else // if error is not ok or not existing, then display error in alert
	{
		eAlert = new ErrorAlert("1.2 Sql Error: ", sqlite3_errmsg(mpdb));
		eAlert->Launch();
	}
}