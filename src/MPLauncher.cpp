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
	:	BWindow(BRect(100, 100, 670, 400), "MasterPiece Launcher", B_TITLED_WINDOW,  B_NOT_H_RESIZABLE | B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE)
{
	// mplauncher is the main window of the application
	
	// gui control initialization
	AddShortcut('m', 0, new BMessage(CREATE_NEW_MP));
	AddShortcut('t', 0, new BMessage(CREATE_NEW_THT));
	AddShortcut('d', 0, new BMessage(START_DELETE));
	mainGroup = new BGroupLayout(B_HORIZONTAL, 0.0);
	newThoughtButton = new BButton(BRect(10, 10, 90, 35), NULL, "Create a New...", new BMessage(CREATE_NEW_THT), B_FOLLOW_NONE, B_WILL_DRAW | B_NAVIGABLE);
	newMasterpieceButton = new BButton(BRect(0, 10, 80, 35), NULL, "Create a New...", new BMessage(CREATE_NEW_MP), B_FOLLOW_NONE, B_WILL_DRAW | B_NAVIGABLE);
	delThoughtButton = new BButton(BRect(10, 10, 90, 35), NULL, "Delete Selected...", new BMessage(DELETE_LAUNCHER_THT), B_FOLLOW_NONE, B_WILL_DRAW | B_NAVIGABLE);
	delMasterpieceButton = new BButton(BRect(10, 10, 90, 35), NULL, "Delete Selected...", new BMessage(DELETE_MP), B_FOLLOW_NONE, B_WILL_DRAW | B_NAVIGABLE);
	delThoughtButton->SetEnabled(false);
	delMasterpieceButton->SetEnabled(false);
	thoughtStringView = new BStringView(BRect(10, 10, 200, 30), NULL, "Work on a Thought");
	masterpieceStringView = new BStringView(BRect(10, 10, 200, 30), NULL, "Work on a Masterpiece");
	openMasterpieceStringView = new BStringView(BRect(10, 10, 200, 30), NULL, "Open an Existing...");
	openThoughtStringView = new BStringView(BRect(10, 10, 200, 30), NULL, "Open an Existing...");
	openThoughtListView = new BListView(BRect(10, 10, 100, 30), NULL, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);
	openMasterpieceListView = new BListView(BRect(10, 10, 100, 30), NULL, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);
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
		.Add(BSpaceLayoutItem::CreateGlue(), 4, 0, 2, 1)
		.Add(newMasterpieceButton, 0, 1)
		.Add(delMasterpieceButton, 1, 1)
		.Add(BSpaceLayoutItem::CreateGlue(), 1, 0)
		.Add(newThoughtButton, 2, 1)
		.Add(delThoughtButton, 3, 1)
		.Add(openMasterpieceStringView, 0, 2)
		.Add(BSpaceLayoutItem::CreateGlue(), 1, 2)
		.Add(openThoughtStringView, 2, 2)
		.Add(BSpaceLayoutItem::CreateGlue(), 3, 2)
		.Add(new BScrollView("scroll_masterpiecelist", openMasterpieceListView,  B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 0, 3, 2, 3)
		.Add(new BScrollView("scroll_thoughtlist", openThoughtListView,  B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 2, 3, 2, 3)
		.SetInsets(5, 5, 5, 2)
	);
	PopulateLauncherListViews(); // populate listview's here
	openMasterpieceListView->SetSelectionMessage(new BMessage(SELECT_EXIST_MP)); // single click action
	openThoughtListView->SetSelectionMessage(new BMessage(SELECT_EXIST_THT)); // single click action
	openMasterpieceListView->SetInvocationMessage(new BMessage(OPEN_EXISTING_MP)); // double click action
	openThoughtListView->SetInvocationMessage(new BMessage(OPEN_EXISTING_THT)); // double click action
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
		case SELECT_EXIST_MP:
			if(openMasterpieceListView->CurrentSelection() >= 0)
			{
				openThoughtListView->DeselectAll();
				delMasterpieceButton->SetEnabled(true);
				delThoughtButton->SetEnabled(false);
			}
			else
			{
				delMasterpieceButton->SetEnabled(false);
				//delThoughtButton->SetEnabled(false);
			}
			break;
		case SELECT_EXIST_THT:
			if(openThoughtListView->CurrentSelection() >= 0)
			{
				openMasterpieceListView->DeselectAll();
				delThoughtButton->SetEnabled(true);
				delMasterpieceButton->SetEnabled(false);
			}
			else
			{
				//delMasterpieceButton->SetEnabled(false);
				delThoughtButton->SetEnabled(false);
			}
			break;
		case DELETE_LAUNCHER_THT:
			IdeaStringItem* item;
			item = dynamic_cast<IdeaStringItem*>(openThoughtListView->ItemAt(openThoughtListView->CurrentSelection()));
			sqlObject = new SqlObject(ideaStatement, "7"); // open sqldb
			sqlObject->PrepareSql("delete from ideatable where ideaid = ?"); // prepare sql
			sqlObject->BindValue(1, item->ReturnID());
			sqlObject->StepSql();
			sqlObject->FinalizeSql();
			sqlObject->CloseSql();
			PopulateLauncherListViews();
			break;
		case DELETE_MP:
			IdeaStringItem* mpitem;
			mpitem = dynamic_cast<IdeaStringItem*>(openMasterpieceListView->ItemAt(openMasterpieceListView->CurrentSelection()));
			sqlObject = new SqlObject(ideaStatement, "7"); // open sqldb
			sqlObject->PrepareSql("delete from ideatable where ideaid = ?"); // prepare sql
			sqlObject->BindValue(1, mpitem->ReturnID());
			sqlObject->StepSql();
			sqlObject->FinalizeSql();
			sqlObject->CloseSql();		
			PopulateLauncherListViews();
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
		case START_DELETE:
			if(delThoughtButton->IsEnabled() == true)
			{
				PostMessage(DELETE_LAUNCHER_THT, this);
			}
			if(delMasterpieceButton->IsEnabled() == true)
			{
				PostMessage(DELETE_MP, this);
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
	be_app->PostMessage(B_QUIT_REQUESTED); // exit application
	return true;
}
void MPLauncher::PopulateLauncherListViews(void)
{
	openMasterpieceListView->MakeEmpty();
	openThoughtListView->MakeEmpty();
	sqlObject = new SqlObject(ideaStatement, "5"); // open sqldb
	sqlObject->PrepareSql("select ideaname, ideaid from ideatable where ismp = 1"); // prepare sql
	while(sqlObject->StepSql() == SQLITE_ROW) // step through the sql
	{
		openMasterpieceListView->AddItem(new IdeaStringItem(sqlObject->ReturnText(0), sqlObject->ReturnInt(1))); // populate listview
	}
	sqlObject->FinalizeSql(); // finalize sql
	sqlObject->CloseSql(); // close sql
	
	sqlObject = new SqlObject(ideaStatement, "6"); // open sqldb
	sqlObject->PrepareSql("select ideaname, ideaid from ideatable where ismp = 0"); // prepare sql
	while(sqlObject->StepSql() == SQLITE_ROW) // step through the sql
	{
		openThoughtListView->AddItem(new IdeaStringItem(sqlObject->ReturnText(0), sqlObject->ReturnInt(1))); // populate listview
	}
	sqlObject->FinalizeSql(); // finalize sql
	sqlObject->CloseSql(); // close sql
}
