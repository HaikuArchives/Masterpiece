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
				this->Close();
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
						mpMessage.MakeEmpty();
						mpMessage.AddString("opentitle", selectResult[1]);
						mpMessage.AddInt64("openid", selected);
						mpMessenger.SendMessage(&mpMessage);
						this->Close();						
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
void OpenWindow::MouseDown(BPoint point)
{
	/*
	if(clicks == 2)
	{
		eAlert = new ErrorAlert("double click");
		eAlert->Launch();
	}
	*/
}
