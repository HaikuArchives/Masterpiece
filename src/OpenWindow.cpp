#include "OpenWindow.h"

OpenWindow::OpenWindow(const BMessage &msg, const BMessenger &msgr, float mainX, float mainY, const BString commonName)
	:	BWindow(BRect(30, 100, 285, 300), "Open Existing MasterPiece", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE), mpMessage(msg), mpMessenger(msgr)
{
	openListView = new BListView(BRect(10, 10, 400, 150), "mpList", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW);
	openButton = new BButton(BRect(10, 370, 90, 395), NULL, "Open", new BMessage(OPEN_EXISTING_MP), B_FOLLOW_NONE, B_WILL_DRAW);
	cancelButton = new BButton(BRect(10, 370, 90, 395), NULL, "Cancel", new BMessage(CANCEL_OPEN_MP), B_FOLLOW_NONE, B_WILL_DRAW);
	BGridLayout* mainGrid = new BGridLayout();
	SetLayout(mainGrid);
	mainGrid->SetInsets(2, 2, 2, 2);
	mainGrid->AddView(new BScrollView("scroll_mplist", openListView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 0, 0, 3, 1);
	mainGrid->AddView(cancelButton, 1, 1);
	mainGrid->AddView(openButton, 2, 1);
	MoveTo(mainX, mainY);
	sqlErrMsg = 0;
	app_info info;
	be_app->GetAppInfo(&info);
	BPath path(&info.ref);
	path.GetParent(&path);
	BString tmpPath = path.Path();
	tmpPath += "/MasterPiece.db";
	sqlValue = sqlite3_open_v2(tmpPath, &mpdb, SQLITE_OPEN_READWRITE, NULL); // open masterpiece.db
	if(sqlite3_errcode(mpdb) == 14) // if error is SQLITE_CANTOPEN, then create db with structure.
	{
		sqlValue = sqlite3_open_v2(tmpPath, &mpdb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL); // create masterpiece.db
		if(sqlite3_errcode(mpdb) == 0) // sqlite_ok
		{
			tmpString = "create table mptable(mpid integer primary key autoincrement, mpname text);";
			tmpString += " create table ideatable(ideaid integer primary key autoincrement, ideadata blob, ideatype integer, mpid integer, ordernumber integer);";
			tmpString += " create table itypetable(itypeid integer primary key autoincrement, itypename text, itypedescription text);";
			sqlValue = sqlite3_exec(mpdb, tmpString, NULL, NULL, &sqlErrMsg);
			if(sqlValue == SQLITE_OK) // if sql was successful
			{
				// ladida...
			}
			else // sql not successful
			{
				eAlert = new ErrorAlert("1.1 SQL Error: ", sqlErrMsg);
				eAlert->Launch();
			}
		}
		else // some kind of failure...
		{
			eAlert = new ErrorAlert("1.0 Sql Error: ", sqlite3_errmsg(mpdb));
			eAlert->Launch();
		}
	}
	else if(sqlite3_errcode(mpdb) == 0) // SQLITE_OK, it exists
	{
		this->openListView->MakeEmpty();
		if(commonName == "")
		{
			tmpString = "select mpid, mpname from mptable";
		}
		else
		{
			tmpString = "select mpid, mpname from mptable where mpname = '";
			tmpString += commonName;
			tmpString += "'";
		}
		sqlValue = sqlite3_get_table(mpdb, tmpString, &selectResult, &nrow, &ncol, &sqlErrMsg);
		if(sqlValue == SQLITE_OK) // if sql query was successful
		{
			for(int i = 0; i < nrow; i++)
			{
				tmpString = selectResult[(i*ncol) + 2];
				tmpString += ". ";
				tmpString += selectResult[(i*ncol)+3];
				this->openListView->AddItem(new BStringItem(tmpString));
			}
		}
		else // sql select failed
		{
			eAlert = new ErrorAlert("No Masterpiece Exist.  Please Create One First.\r\n1.4 SQL Error: ", sqlErrMsg);
			eAlert->Launch();
		}
		sqlite3_free_table(selectResult); // free table either way
	}
	else // if error is not ok or not existing, then display error in alert.
	{
		eAlert = new ErrorAlert("1.2 Sql Error: ", sqlite3_errmsg(mpdb));
		eAlert->Launch();
	}
}
void OpenWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case CANCEL_OPEN_MP:
			if(!this->IsHidden())
			{
				this->Hide();
			}
			break;
		case OPEN_EXISTING_MP:
			int selected;
			selected = this->openListView->CurrentSelection() + 1; // list item value + 1
			if(selected < 0)
			{
				eAlert = new ErrorAlert("3.1 No Existing Masterpiece was found.  Please Try Again");
				eAlert->Launch();
			}
			BStringItem *item;
			item = dynamic_cast<BStringItem*>(this->openListView->ItemAt(selected - 1));
			if(item)
			{
				tmpString = "select mpname from mptable where mpid = ";
				tmpString << selected;
				sqlValue = sqlite3_get_table(mpdb, tmpString, &selectResult, &nrow, &ncol, &sqlErrMsg);
				if(sqlValue == SQLITE_OK) // sql was successful
				{
					if(nrow == 1) // 1 id was returned.
					{
						// commit message sending information to mainwindow where it can update_open_mp...
						/*
						this->SetTitle(selectResult[1]);
						tmpString = selectResult[1];
						tmpString += " Summary";
						this->sumView->sumViewTitleString->SetText(tmpString);						
						sqlite3_free_table(selectResult);
						if(!this->openView->IsHidden()) this->openView->Hide();
						if(this->sumView->IsHidden()) this->sumView->Show();
						this->mpMenuBar->contentMenu->SetEnabled(true);
						this->mpMenuBar->layoutMenu->SetEnabled(true);
						this->mpMenuBar->closeFileMenuItem->SetEnabled(true);
						*/
						
					}
					else // wrong resultset was returned...
					{
						eAlert = new ErrorAlert("3.2 MasterPiece could not be opened.  Please Try Again");
						eAlert->Launch();
					}
				}
				else // sql wasn't successful
				{
					eAlert = new ErrorAlert("1.5 Sql Error: ", sqlErrMsg);
					eAlert->Launch();
				}
				sqlite3_free_table(selectResult);
			}
			break;
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}
/* Main Window Code Reference
*
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
				tmpString = "select mpname from mptable where mpid = ";
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
						if(this->sumView->IsHidden()) this->sumView->Show();
						this->mpMenuBar->contentMenu->SetEnabled(true);
						this->mpMenuBar->layoutMenu->SetEnabled(true);
						this->mpMenuBar->closeFileMenuItem->SetEnabled(true);
						
					}
					else // wrong resultset was returned...
					{
						errorAlert = new ErrorAlert("3.2 MasterPiece could not be opened.  Please Try Again");
						errorAlert->Launch();
					}
				}
				else // sql wasn't successful
				{
					errorAlert = new ErrorAlert("1.5 Sql Error: ", sqlErrMsg);
					errorAlert->Launch();
				}
			}
			else // selected wasn't an item
			{
				errorAlert = new ErrorAlert("3.3 MasterPiece does not exist.  Please Try Again");
				errorAlert->Launch();
			}

*/
/* New Window Code Reference
*
void NewWindow::MessageReceived(BMessage *msg)
{
		
		case ADD_NEW_MP:
			if(strlen(this->titleText->Text()) == 0) // mp title is empty
			{
				eAlert = new ErrorAlert("2.2 MasterPiece Name Cannot Be Blank.  Please Try Again.");
				eAlert->Launch();
			}
			else // mp title has length
			{
				tmpString = "select mpid from mptable where mpname = '";
				tmpString += this->titleText->Text();
				tmpString += "';";
				sqlValue = sqlite3_get_table(mpdb, tmpString, &selectResult, &nrow, &ncol, &sqlErrMsg);
				if(sqlValue == SQLITE_OK) // if sql was successful
				{
					if(nrow >= 1) // course already exists
					{
						sqlite3_free_table(selectResult);
						tmpString = "The MasterPiece: \"";
						tmpString += this->titleText->Text();
						tmpString += "\" already exists. Do you want to Open the existing, Create a new one or Cancel?";
						userAlert = new BAlert("MasterPiece Exists", tmpString, "Open", "Create", "Cancel", B_WIDTH_AS_USUAL, B_INFO_ALERT);
						userAlert->MoveTo(350, 250);
						userAlert->SetShortcut(2, B_ESCAPE);
						int alertReturn = userAlert->Go();
						if(alertReturn == 0) // Open
						{
							if(nrow == 1) // only 1 exists so send message to open that one
							{
								mpMessage.MakeEmpty();
								mpMessage.AddString("mptitle", this->titleText->Text());
								mpMessage.AddInt64("mpid", (int)selectResult[2]);
								mpMessenger.SendMessage(&mpMessage);
								this->titleText->SetText("");
								this->Close();
							}
							else if(nrow > 1) // multiple mp's exist, send message to open the open window with these values only
							{
								// close this window
							}
							else // something went horribly wrong and need to fix immediately
							{
							}
						}
						else if(alertReturn == 1) // Create
						{
							tmpString = "insert into mptable (mpname) values('";
							tmpString += this->titleText->Text();
							tmpString += "');";
							sqlValue = sqlite3_exec(mpdb, tmpString, NULL, NULL, &sqlErrMsg);
							if(sqlValue == SQLITE_OK) // insert was successful
							{
								mpMessage.MakeEmpty();
								mpMessage.AddString("mptitle", this->titleText->Text());
								mpMessage.AddInt64("mpid", sqlite3_last_insert_rowid(mpdb));
								mpMessenger.SendMessage(&mpMessage);
								this->titleText->SetText(""); // reset new course title to blank when done
								this->Close();
							}
							else // insert failed
							{
								eAlert = new ErrorAlert("Error 2.1 MasterPiece was not created successfully. Please Try Again.");
								eAlert->Launch();
							}					
						}
						else if(alertReturn == 2) // Cancel
						{
						}
						else // Major Error - this feature should never occur
						{
							eAlert = new ErrorAlert("Error 2.3 Illegal Option Selected.  Please Stop Using Program and Report to the Developer!");
							eAlert->Launch();
						}
					}
					else // course does not exist, add course
					{
						tmpString = "insert into mptable (mpname) values('";
						tmpString += this->titleText->Text();
						tmpString += "');";
						sqlValue = sqlite3_exec(mpdb, tmpString, NULL, NULL, &sqlErrMsg);
						if(sqlValue == SQLITE_OK) // insert was successful
						{
							mpMessage.MakeEmpty();
							mpMessage.AddString("mptitle", this->titleText->Text());
							mpMessage.AddInt64("mpid", sqlite3_last_insert_rowid(mpdb));
							mpMessenger.SendMessage(&mpMessage);
							this->titleText->SetText(""); // reset new course title to blank when done
							this->Close();
						}
						else // insert failed
						{
							eAlert = new ErrorAlert("Error 2.1 MasterPiece was not created successfully. Please Try Again.");
							eAlert->Launch();
						}					
					}
				}
				else // sql not successful
				{
					eAlert = new ErrorAlert("1.3 Sql Error: ", sqlErrMsg);
					eAlert->Launch();
				}
			}
			break;
			
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}
void NewWindow::Draw(BRect rect)
{
}
*
*/
