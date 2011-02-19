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
	
	mpdb = OpenSqliteDB();
	if(mpdb == NULL)
	{
		eAlert = new ErrorAlert("sql db was not opened properly.");
		eAlert->Launch();
	}
	else  // populate listview's here...
	{
		PopulateLauncherListViews();
		openMasterpieceListView->SetInvocationMessage(new BMessage(OPEN_EXISTING_MP));
		openThoughtListView->SetInvocationMessage(new BMessage(OPEN_EXISTING_THT));
	}
}
void MPLauncher::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case CREATE_NEW_MP:
			mpBuilder = new MPBuilder(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Builder - untitled", -1);
			mpBuilder->Show();
			this->Hide();
			break;
		case CREATE_NEW_THT:
			mpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Editor - untitled", -1);
			mpEditor->Show();
			this->Hide();
			break;
		case OPEN_EXISTING_MP:
			selected = openMasterpieceListView->CurrentSelection(); // list item value
			if(selected < 0)
			{
				mpBuilder = new MPBuilder(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Builder - untitled", -1);
				mpBuilder->Show();
				this->Hide();
			}
			else
			{
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(openMasterpieceListView->ItemAt(selected));
				BString tmpText;
				tmpText = "MasterPiece Builder - ";
				tmpText += item->Text();
				mpBuilder = new MPBuilder(BMessage(SHOW_LAUNCHER), BMessenger(this), tmpText, item->ReturnID());
				mpBuilder->Show();
				this->Hide();
			}
			break;
		case OPEN_EXISTING_THT:
			selected = openThoughtListView->CurrentSelection(); // list item value
			if(selected < 0)
			{
				mpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Editor - untitled", -1);
				mpEditor->Show();
				this->Hide();
			}
			else
			{
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(openThoughtListView->ItemAt(selected));
				BString tmpText;
				tmpText = "MasterPiece Editor - ";
				tmpText += item->Text();
				mpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), tmpText, item->ReturnID());
				mpEditor->Show();
				this->Hide();
			}
			break;
		case SHOW_LAUNCHER:
			if(msg->FindInt64("showLauncher", &showLauncher) == B_OK)
			{
				if(showLauncher == 1)
				{
					// need to rerun the sql and repopulate the 2 listviews
					PopulateLauncherListViews();
					if(this->IsHidden())
					{
						this->Show();
					}
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
	sqlite3_free(sqlErrMsg);
	sqlite3_close(mpdb);
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
void MPLauncher::PopulateLauncherListViews(void)
{
	openMasterpieceListView->MakeEmpty();
	openThoughtListView->MakeEmpty();
	
	sqlValue = sqlite3_prepare_v2(mpdb, "select ideaname, ideaid from ideatable where ismp = 1", -1, &ideaStatement, NULL);
	if(sqlValue == SQLITE_OK) // sql query was successful
	{
		while(sqlite3_step(ideaStatement) == SQLITE_ROW)
		{
			tmpString = sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 0));
			openMasterpieceListView->AddItem(new IdeaStringItem(tmpString, sqlite3_column_int(ideaStatement, 1)));
		}
	}
	else // sql select failed
	{
		eAlert = new ErrorAlert("No Masterpiece Exist. Please Create One First.");
		eAlert->Launch();
	}
	sqlite3_finalize(ideaStatement);
	sqlValue = sqlite3_prepare_v2(mpdb, "select ideaname, ideaid from ideatable where ismp = 0", -1, &ideaStatement, NULL);
	if(sqlValue == SQLITE_OK) // sql statement was prepared
	{
		while(sqlite3_step(ideaStatement) == SQLITE_ROW)
		{
			tmpString = sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 0));
			openThoughtListView->AddItem(new IdeaStringItem(tmpString, sqlite3_column_int(ideaStatement, 1)));
		}
	}
	else
	{
		eAlert = new ErrorAlert("No Thoughts Exist.  Please Create One First.");
		eAlert->Launch();
	}
	sqlite3_finalize(ideaStatement);
}
