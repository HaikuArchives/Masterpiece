#include "MainWindow.h"

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,900,700),"MasterPiece",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE)
{
	BRect r(Bounds());
	r.bottom = 20;
	mpMenuBar = new MenuBar(r);
	AddChild(mpMenuBar);
	rgb_color myColor = {215, 215, 215, 255};
	
	fullView = new NewMasterView();
	AddChild(fullView);
	fullView->SetViewColor(myColor);
	fullView->Hide();
	
	openView = new OpenMasterView();
	AddChild(openView);
	openView->SetViewColor(myColor);
	openView->Hide();
	BRect sumRect(Bounds());
	sumRect.top = 20;
	sumView = new SummaryView(sumRect);
	AddChild(sumView);
	sumView->SetViewColor(myColor);
	sumView->Hide();
	sqlErrMsg = 0;
	
	app_info info;
	be_app->GetAppInfo(&info);
	BPath path(&info.ref);
	path.GetParent(&path);
	fprintf(stdout, path.Path());
	tmpString = path.Path();
	tmpString += "/MasterPiece.db";
	sqlValue = sqlite3_open_v2(tmpString, &mpdb, SQLITE_OPEN_READWRITE, NULL); // open masterpiece.db
	if(sqlite3_errcode(mpdb) != 0) // if error is not ok, then display error in alert.
	{
		errorAlert = new ErrorAlert("1.1 Sql Error: ", sqlite3_errmsg(mpdb));
		errorAlert->Launch();
		this->mpMenuBar->fileMenu->SetEnabled(false);
		
	}
}

void MainWindow::MessageReceived(BMessage *msg)
{
	
	switch (msg->what)
	{
		case MENU_NEW_MSG:
			// 1.  need to center the modal window on the parent...
			// 2.  check to see if course is currently open
			if(!this->sumView->IsHidden()) this->sumView->Hide();
			if(!this->openView->IsHidden()) this->openView->Hide();
			if(this->fullView->IsHidden()) this->fullView->Show();
			break;
		
		case MENU_OPN_MSG:
			if(!this->sumView->IsHidden()) this->sumView->Hide();
			if(!this->fullView->IsHidden()) this->fullView->Hide();
			this->openView->openListView->MakeEmpty();
			/*
			homeDir->Rewind();
			while(homeDir->GetNextEntry(&entry) == B_OK)
			{
				char name[B_FILE_NAME_LENGTH];
				entry.GetName(name);
				if(entry.IsDirectory())
				{
					this->openView->openListView->AddItem(new BStringItem(name));
				}
			}
			*/
			if(this->openView->IsHidden()) this->openView->Show();
			
			break;
		
		case ADD_NEW_COURSE:
			tmpString = "select masterpieceName from mptable where masterpieceName = '";
			tmpString += this->fullView->titleText->Text();
			tmpString += "';";
			sqlValue = sqlite3_get_table(mpdb, tmpString, &selectResult, &nrow, &ncol, &sqlErrMsg);
			if(sqlValue == SQLITE_OK) // if sql was successful
			{
				if(nrow >= 1) // course already exists
				{
					fprintf(stdout, "count: %d, %s = %s\n", nrow, selectResult[0], selectResult[1]);
					sqlite3_free_table(selectResult);
					tmpString = "The MasterPiece: \"";
					tmpString += this->fullView->titleText->Text();
					tmpString += "\" already exists.  Do you want to Open the existing, Create a new one or cancel?";
					userAlert = new BAlert("MasterPiece Exists", tmpString, "Open", "Create", "Cancel", B_WIDTH_AS_USUAL, B_INFO_ALERT);
					userAlert->MoveTo(350, 250);
					userAlert->SetShortcut(2, B_ESCAPE);
					int alertReturn = userAlert->Go();
					if(alertReturn == 0) // Open
					{
						fprintf(stdout, "open course\n");
						this->SetTitle(this->fullView->titleText->Text());
						if(!this->fullView->IsHidden()) this->fullView->Hide();
						if(!this->openView->IsHidden()) this->openView->Hide();
						if(this->sumView->IsHidden()) this->sumView->Show();
						this->mpMenuBar->contentMenu->SetEnabled(true);
						this->mpMenuBar->layoutMenu->SetEnabled(true);
						// 1. need to load summary view with the existing course information
					}
					else if(alertReturn == 1) // Create
					{
						fprintf(stdout, "create course\n");
						tmpString = "insert into mptable (masterpieceName) values('";
						tmpString += this->fullView->titleText->Text();
						tmpString += "');";
						sqlValue = sqlite3_exec(mpdb, tmpString, NULL, NULL, &sqlErrMsg);
						if(sqlValue == SQLITE_OK) // insert was successful
						{
							this->SetTitle(this->fullView->titleText->Text());
							if(!this->fullView->IsHidden()) this->fullView->Hide();
							if(!this->openView->IsHidden()) this->openView->Hide();
							if(this->sumView->IsHidden()) this->sumView->Show();
							this->mpMenuBar->contentMenu->SetEnabled(true);
							this->mpMenuBar->layoutMenu->SetEnabled(true);
							// load empty summary view information for this new course
						}
						else
						{
							errorAlert = new ErrorAlert("Error 2.1 MasterPiece was not created successfully. Please Try Again.");
							errorAlert->Launch();
						}
					}
					else if(alertReturn == 2) // Cancel
					{
						fprintf(stdout, "cancel course\n");
					}
				}
			}
			else // sql not succesful, display error
			{
				errorAlert = new ErrorAlert("1.2 Sql Error: ", sqlErrMsg);
				errorAlert->Launch();
			}
			this->fullView->titleText->SetText(""); // reset new course title to blank when done
			break;
		
		case CANCEL_NEW_COURSE:		
			if(!this->fullView->IsHidden()) this->fullView->Hide();
			this->fullView->titleText->SetText("");
			// do soemthing here...
			break;
			
		case MENU_THT_MSG:
			if(this->sumView->IsHidden()) this->sumView->Show();
			// do something here...
			break;
			
		case MNG_LAYOUT_MSG:
			// do something here...
			break;
		case CANCEL_OPEN_COURSE:
			if(!this->openView->IsHidden())this->openView->Hide();
			// do something here...
			break;
		case OPEN_EXISTING_COURSE:
			// do something here...
			int32 selected;
			selected = this->openView->openListView->CurrentSelection();
			if(selected < 0)
			{
				// error here
			}
			BStringItem *item;
			item = dynamic_cast<BStringItem*>(this->openView->openListView->ItemAt(selected));
			if(item)
			{
				this->SetTitle(item->Text());
				if(!this->openView->IsHidden()) this->openView->Hide();
				if(!this->fullView->IsHidden()) this->fullView->Hide();
				if(this->sumView->IsHidden()) this->sumView->Show();
				this->mpMenuBar->contentMenu->SetEnabled(true);
				this->mpMenuBar->layoutMenu->SetEnabled(true);
			}
			break;
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


bool
MainWindow::QuitRequested(void)
{
	sqlite3_free(sqlErrMsg);
	sqlite3_close(mpdb);
	
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
