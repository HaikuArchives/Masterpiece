/*
 *
 * Error Codes
 * 1.x - Sql Errors
 * 2.x - Launcher Errors
 * 3.x - Editor Errors
 * 4.x - Builder Errors
 * 5.x - Save Errors
 * 6.x - Edit Idea Name Errors
 *
 */

#include "MPLauncher.h"

MPLauncher::MPLauncher(void)
	:	BWindow(BRect(100, 100, 650, 400), "MasterPiece Launcher", B_TITLED_WINDOW,  B_NOT_H_RESIZABLE | B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE)
{
	// mplauncher is the main window of the application
	
	// gui control initialization
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
	// layout builder code
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

	//mpdb = OpenSqliteDB(); // open sqldb
	if(mpdb == NULL)
	{
		eAlert = new ErrorAlert("1.4. Sql Error: SQL DB was not opened properly.");
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
		case CREATE_NEW_MP: // create new mp by opening mp builder
			mpBuilder = new MPBuilder(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Builder - untitled", -1);
			mpBuilder->Show();
			this->Hide();
			break;
		case CREATE_NEW_THT: // create new thought by opening thought editor
			mpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Editor - untitled", -1);
			mpEditor->Show();
			this->Hide();
			break;
		case OPEN_EXISTING_MP: // open existing mp by selecting from listview
			selected = openMasterpieceListView->CurrentSelection(); // list item value
			if(selected < 0) // if selected nothing, open empty builder window
			{
				mpBuilder = new MPBuilder(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Builder - untitled", -1);
				mpBuilder->Show();
				this->Hide();
			}
			else // you selected an actual existing mp from listview
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
		case OPEN_EXISTING_THT: // open existing thought by selecting from listview
			selected = openThoughtListView->CurrentSelection(); // list item value
			if(selected < 0) // if selected nothing, open empty builder window
			{
				mpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Editor - untitled", -1);
				mpEditor->Show();
				this->Hide();
			}
			else // you selected an actual existing thought from listview
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
		case SHOW_LAUNCHER:  // once finished with editor or builder, call to show this launcher
			if(msg->FindInt64("showLauncher", &showLauncher) == B_OK) // message was found
			{
				if(showLauncher == 1)
				{
					PopulateLauncherListViews(); // rerun the sql and repopulate the 2 listviews with any updated values
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
					eAlert = new ErrorAlert("2.1 Launcher Error: Illegal Value was returned.");
					eAlert->Launch();
					// big error must display 
				}
			}
			else // message not found
			{
				eAlert = new ErrorAlert("2.2 Launcher Error: Message Variable was not found.");
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
	//sqlite3_free(sqlErrMsg); // free up sql
	//sqlite3_close(mpdb); // close db
	be_app->PostMessage(B_QUIT_REQUESTED); // exit application
	return true;
}
void MPLauncher::PopulateLauncherListViews(void)
{
	openMasterpieceListView->MakeEmpty();
	openThoughtListView->MakeEmpty();
	sqlObject = new SqlObject(ideaStatement, "5");
	sqlObject->PrepareSql("select ideaname, ideaid from ideatable where ismp = 1");
	while(sqlObject->StepSql() == SQLITE_ROW)
	{
		openMasterpieceListView->AddItem(new IdeaStringItem(sqlObject->ReturnText(0), sqlObject->ReturnInt(1)));
	}
	sqlObject->FinalizeSql();
	sqlObject->CloseSql();
	
	sqlObject = new SqlObject(ideaStatement, "6");
	sqlObject->PrepareSql("select ideaname, ideaid from ideatable where ismp = 0");
	while(sqlObject->StepSql() == SQLITE_ROW)
	{
		openThoughtListView->AddItem(new IdeaStringItem(sqlObject->ReturnText(0), sqlObject->ReturnInt(1)));
	}
	sqlObject->FinalizeSql();
	sqlObject->CloseSql();
	/*
	sqlObject = new SqlObject(mpdb, ideaStatement, "5");
	sqlObject->PrepareSql("select ideaname, ideaid from ideatable where ismp = 1");
	while(sqlObject->StepSql() == SQLITE_ROW)
	{
		openMasterpieceListView->AddItem(new IdeaStringItem(sqlObject->ReturnText(0), sqlObject->ReturnInt(1)));
	}
	sqlObject->FinalizeSql();
	sqlObject->PrepareSql("select ideaname, ideaid from ideatable where ismp = 0");
	while(sqlObject->StepSql() == SQLITE_ROW)
	{
		openThoughtListView->AddItem(new IdeaStringItem(sqlObject->ReturnText(0), sqlObject->ReturnInt(1)));
	}
	sqlObject->FinalizeSql();
	*/
}
