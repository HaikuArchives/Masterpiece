#include "NewWindow.h"

NewWindow::NewWindow(const BMessage &msg, const BMessenger &msgr, float mainX, float mainY)
	:	BWindow(BRect(20, 20, 200, 100), "Enter Title", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE), mpMessage(msg), mpMessenger(msgr)
{
	/*
	BRect textFrame(0, 0, 170, 40);
	titleText = new MultiLineTextControl(textFrame, NULL, NULL, false, "", NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	newButton = new BButton(BRect(190, 50, 270, 75), NULL, "Add", new BMessage(ADD_NEW_MP), B_FOLLOW_NONE, B_WILL_DRAW);
	cancelButton = new BButton(BRect(100, 50, 180, 75), NULL, "Cancel", new BMessage(CANCEL_NEW_MP), B_FOLLOW_NONE, B_WILL_DRAW);
	mainGrid = new BGridLayout();
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	backView->SetLayout(mainGrid);
	mainGrid->SetInsets(2, 2, 2, 2);
	mainGrid->AddView(titleText, 0, 0, 2, 1);
	mainGrid->AddView(cancelButton, 0, 1);
	mainGrid->AddView(newButton, 1, 1);
	MoveTo(mainX, mainY);
	
	OpenMasterPieceDB();
	*/
}
void NewWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what)
	{
		case CANCEL_NEW_MP:
			if(!this->IsHidden())
			{
				//this->titleText->SetText("");
				this->Hide();
			}
			break;
		
		case ADD_NEW_MP:
			/*
			if(strlen(this->titleText->Text()) == 0) // mp title is empty
			{
				eAlert = new ErrorAlert("2.2 MasterPiece Name Cannot Be Blank.  Please Try Again.");
				eAlert->Launch();
			}
			else // mp title has length
			{
				AddNewMasterPiece();
			}
			*/
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

void NewWindow::OpenMasterPieceDB()
{
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
		// ladida
	}
	else // if error is not ok or not existing, then display error in alert.
	{
		eAlert = new ErrorAlert("1.2 Sql Error: ", sqlite3_errmsg(mpdb));
		eAlert->Launch();
	}
}

void NewWindow::AddNewMasterPiece()
{
	/*
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
								BRect r(Frame());
								xPos = r.left;
								yPos = r.top;
								openWin = new OpenWindow(BMessage(UPDATE_OPEN_MP), BMessenger(this), xPos, yPos, this->titleText->Text());
								openWin->Show();
								this->titleText->SetText("");
								this->Close();
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
				*/
}
