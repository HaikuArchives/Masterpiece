
/*
 *
 * Error Codes
 * 1.x - Sql Errors
 * 2.x - New MP
 * 3.x - Open MP
 *
 */

// 1.  Need to check and see if an mp is already open, prior to creating a new one,
// 	   so i can reload what was there and what view was there...

#include "MainWindow.h"

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,900,700),"MasterPiece",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE)
{
	BRect r(Bounds());
	r.bottom = 20;
	mpMenuBar = new MPMenuBar(r);
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
	BString tmpPath = path.Path();
	tmpPath += "/MasterPiece.db";
	sqlValue = sqlite3_open_v2(tmpPath, &mpdb, SQLITE_OPEN_READWRITE, NULL); // open masterpiece.db
//	sqlValue = sqlite3_open(tmpPath, &mpdb); // open masterpiece.db
	if(sqlite3_errcode(mpdb) == 14) // if error is SQLITE_CANTOPEN, then create db with structure.
	{
		// create db with structure here
	}
	else if(sqlite3_errcode(mpdb) == 0) // SQLITE_OK, it exists
	{
		// ladida
	}
	else // if error is not ok or not existing, then display error in alert.
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
			tmpString = "select masterpieceID, masterpieceName from mptable";
			sqlValue = sqlite3_get_table(mpdb, tmpString, &selectResult, &nrow, &ncol, &sqlErrMsg);
			if(sqlValue == SQLITE_OK) // if sql was successful
			{
				for(int i = 0; i < nrow; i++)
				{
					tmpString = selectResult[(i*ncol)+2];
					tmpString += ".  ";
					tmpString += selectResult[(i*ncol)+3];
					this->openView->openListView->AddItem(new BStringItem(tmpString));
				}
			}
			else // sql select was not successful.
			{
				errorAlert = new ErrorAlert("1.3 SQL Error: ", sqlErrMsg);
				errorAlert->Launch();
			}
			sqlite3_free_table(selectResult); // free table either way
			if(this->openView->IsHidden()) this->openView->Show();
			
			break;
		
		case ADD_NEW_COURSE:
			if(strlen(this->fullView->titleText->Text()) == 0) // mp title is empty
			{
				errorAlert = new ErrorAlert("2.2 MasterPiece Name Cannot Be Blank.  Please Try Again.");
				errorAlert->Launch();
			}
			else // mp title has length
			{
				tmpString = "select masterpieceName from mptable where masterpieceName = '";
				tmpString += this->fullView->titleText->Text();
				tmpString += "';";
				sqlValue = sqlite3_get_table(mpdb, tmpString, &selectResult, &nrow, &ncol, &sqlErrMsg);
				if(sqlValue == SQLITE_OK) // if sql was successful
				{
					if(nrow >= 1) // course already exists
					{
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
							tmpString = this->fullView->titleText->Text();
							tmpString += " Summary";
							this->sumView->sumViewTitleString->SetText(tmpString);
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
							tmpString = "insert into mptable (masterpieceName) values('";
							tmpString += this->fullView->titleText->Text();
							tmpString += "');";
							sqlValue = sqlite3_exec(mpdb, tmpString, NULL, NULL, &sqlErrMsg);
							if(sqlValue == SQLITE_OK) // insert was successful
							{
								this->SetTitle(this->fullView->titleText->Text());
								tmpString = this->fullView->titleText->Text();
								tmpString += " Summary";
								this->sumView->sumViewTitleString->SetText(tmpString);						
								if(!this->fullView->IsHidden()) this->fullView->Hide();
								if(!this->openView->IsHidden()) this->openView->Hide();
								if(this->sumView->IsHidden()) this->sumView->Show();
								this->mpMenuBar->contentMenu->SetEnabled(true);
								this->mpMenuBar->layoutMenu->SetEnabled(true);
								// load empty summary view information for this new course
							}
							else // insert failed
							{
								errorAlert = new ErrorAlert("Error 2.1 MasterPiece was not created successfully. Please Try Again.");
								errorAlert->Launch();
							}
						}
						else if(alertReturn == 2) // Cancel
						{
						}
					}
					else // course does not exist, add course
					{
						tmpString = "insert into mptable (masterpieceName) values('";
						tmpString += this->fullView->titleText->Text();
						tmpString += "');";
						sqlValue = sqlite3_exec(mpdb, tmpString, NULL, NULL, &sqlErrMsg);
						if(sqlValue == SQLITE_OK) // insert was successful
						{
							this->SetTitle(this->fullView->titleText->Text());
							tmpString = this->fullView->titleText->Text();
							tmpString += " Summary";
							this->sumView->sumViewTitleString->SetText(tmpString);						
							if(!this->fullView->IsHidden()) this->fullView->Hide();
							if(!this->openView->IsHidden()) this->openView->Hide();
							if(this->sumView->IsHidden()) this->sumView->Show();
							this->mpMenuBar->contentMenu->SetEnabled(true);
							this->mpMenuBar->layoutMenu->SetEnabled(true);
							// load empty summary view information for this new course
						}
						else // insert failed
						{
							errorAlert = new ErrorAlert("Error 2.1 MasterPiece was not created successfully. Please Try Again.");
							errorAlert->Launch();
						}
					}
				}
				else // sql not succesful, display error
				{
					errorAlert = new ErrorAlert("1.2 Sql Error: ", sqlErrMsg);
					errorAlert->Launch();
				}
				this->fullView->titleText->SetText(""); // reset new course title to blank when done regardless of operation
			}

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
			int selected;
			selected = this->openView->openListView->CurrentSelection() + 1; // list item value + 1
			if(selected < 0)
			{
				errorAlert = new ErrorAlert("3.1  MasterPiece was not found.  Please Try Again");
				errorAlert->Launch();
				// ensure the program stays on the open view, if you select nothing to try again...
			}
			BStringItem *item;
			item = dynamic_cast<BStringItem*>(this->openView->openListView->ItemAt(selected - 1));
			if(item)
			{
				tmpString = "select masterpieceName from mptable where masterpieceID = ";
				tmpString << selected;
				sqlValue = sqlite3_get_table(mpdb, tmpString, &selectResult, &nrow, &ncol, &sqlErrMsg);
				if(sqlValue == SQLITE_OK) // if sql was successful
				{
					if(nrow == 1) // 1 id was returned.
					{
						this->SetTitle(selectResult[1]);
						tmpString = selectResult[1];
						tmpString += " Summary";
						this->sumView->sumViewTitleString->SetText(tmpString);						
						sqlite3_free_table(selectResult);
						if(!this->openView->IsHidden()) this->openView->Hide();
						if(!this->fullView->IsHidden()) this->fullView->Hide();
						if(this->sumView->IsHidden()) this->sumView->Show();
						this->mpMenuBar->contentMenu->SetEnabled(true);
						this->mpMenuBar->layoutMenu->SetEnabled(true);
						
					}
					else // wrong resultset was returned...
					{
						errorAlert = new ErrorAlert("3.2 MasterPiece could not be opened.  Please Try Again");
						errorAlert->Launch();
					}
				}
				else // sql wasn't successful
				{
					errorAlert = new ErrorAlert("1.4 Sql Error: ", sqlErrMsg);
					errorAlert->Launch();
				}
			}
			else // selected wasn't an item
			{
				errorAlert = new ErrorAlert("3.3 MasterPiece does not exist.  Please Try Again");
				errorAlert->Launch();
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
