
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
// 2.  Need to replace openview with OpenWindow

#include "MainWindow.h"

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,900,700),"MasterPiece",B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE)
{
	BRect r(Bounds());
	/* begin comment out group when not testing layout code */
	BGroupLayout* mainGroup = new BGroupLayout(B_VERTICAL);
	mainGrid = new BGridLayout();
	SetLayout(mainGroup);
	/* end comment out group when not testing layout code */
	//BView *mainView = new BView(Bounds(), "mainview", B_FOLLOW_ALL, B_WILL_DRAW);
	//AddChild(mainView);  // comment out when testing layout code
	rgb_color myColor = {215, 215, 215, 255};
	//mainView->SetViewColor(myColor);
	//r.bottom = 20;
	mpMenuBar = new MPMenuBar(r);
	//mainView->AddChild(mpMenuBar); // uncomment when not using layout code
	mainGroup->AddView(0, mpMenuBar); // comment out when not testing layout code
	mainGroup->SetInsets(0, 0, 0, 0);
	mainGrid->SetInsets(0, 0, 0, 0);
	mainGroup->AddItem(mainGrid);

	BRect sumRect(Bounds());
	sumRect.top = 0;
	sumView = new SummaryView(sumRect);
	//mainView->AddChild(sumView); // uncomment when not using grid layout
	//mainGrid->AddView(sumView, 0, 0);
	sumView->SetViewColor(myColor);
	//sumView->Hide();
	
	thoughtView = new ThoughtView(sumRect);
	//mainView->AddChild(thoughtView);
	//mainGrid->AddView(thoughtView, 0, 0);
	thoughtView->SetViewColor(myColor);
	//thoughtView->Hide();
	
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
				errorAlert = new ErrorAlert("1.1 SQL Error: ", sqlErrMsg);
				errorAlert->Launch();
			}
		}
		else // some kind of failure...
		{
			errorAlert = new ErrorAlert("1.0 Sql Error: ", sqlite3_errmsg(mpdb));
			errorAlert->Launch();
		}
	}
	else if(sqlite3_errcode(mpdb) == 0) // SQLITE_OK, it exists
	{
		// ladida
	}
	else // if error is not ok or not existing, then display error in alert.
	{
		errorAlert = new ErrorAlert("1.2 Sql Error: ", sqlite3_errmsg(mpdb));
		errorAlert->Launch();
		this->mpMenuBar->fileMenu->SetEnabled(false);
	}
}

void MainWindow::Draw(BRect rect)
{
	//rgb_color backColor = {215, 215, 215, 255};
}

void MainWindow::MessageReceived(BMessage *msg)
{
	BRect r(Bounds());	
	switch (msg->what)
	{
		case MENU_NEW_MSG:
			// 1.  need to center the modal window on the parent...
			// 2.  check to see if course is currently open
			if(!this->sumView->IsHidden()) this->sumView->Hide();
			xPos = (r.right - r.left) / 2;
			yPos = (r.bottom - r.top) / 2;
			newWin = new NewWindow(BMessage(UPDATE_NEW_MP), BMessenger(this), xPos, yPos);
			newWin->Show();
			break;
		
		case UPDATE_NEW_MP:
			if(msg->FindString("mptitle", &mptitle) == B_OK && msg->FindInt64("mpid", &mpid) == B_OK)
			{
				this->SetTitle(mptitle);
				// 1.  need to figure out how to open the summaryview using the mpid
				tmpString = mptitle;
				tmpString += " Summary";
				this->sumView->sumViewTitleString->SetText(tmpString);
				//if(this->sumView->IsHidden()) this->sumView->Show();
				mainGrid->AddView(sumView, 0, 0);
				this->mpMenuBar->contentMenu->SetEnabled(true);
				this->mpMenuBar->layoutMenu->SetEnabled(true);
				this->mpMenuBar->closeFileMenuItem->SetEnabled(true);
				// 1.  call the function to populate summaryview passing the (mpid)
			}
			break;
		
		case MENU_OPN_MSG:
			xPos = (r.right - r.left) / 2;
			yPos = (r.bottom - r.top) / 2;
			openWin = new OpenWindow(BMessage(UPDATE_OPEN_MP), BMessenger(this), xPos, yPos, "");
			openWin->Show();
			break;
		
		case UPDATE_OPEN_MP:
			if(msg->FindString("opentitle", &mptitle) == B_OK && msg->FindInt64("openid", &mpid) == B_OK)
			{
				this->SetTitle(mptitle);
				tmpString = mptitle;
				tmpString += " Summary";
				this->sumView->sumViewTitleString->SetText(tmpString);
				mainGrid->AddView(sumView, 0, 0);
				
				//if(this->sumView->IsHidden()) this->sumView->Show();
				this->mpMenuBar->contentMenu->SetEnabled(true);
				this->mpMenuBar->layoutMenu->SetEnabled(true);
				this->mpMenuBar->closeFileMenuItem->SetEnabled(true);
			}
			
			break;
			
		case MENU_CLS_MSG:
			// 1.  close course - simply clear values and hide views.
			//if(!this->sumView->IsHidden()) this->sumView->Hide();
			mainGrid->RemoveView(sumView);
			this->SetTitle("MasterPiece");
			this->mpMenuBar->closeFileMenuItem->SetEnabled(false);
			this->mpMenuBar->contentMenu->SetEnabled(false);
			this->mpMenuBar->layoutMenu->SetEnabled(false);
			
			break;

		case MENU_THT_MSG:
			tmpView = mainGrid->TargetView();
			//if(!this->sumView->IsHidden()) this->sumView->Hide();
			//if(this->thoughtView->IsHidden()) this->thoughtView->Show();
			//if(!this->sumView->IsHidden()) mainGrid->RemoveView(sumView);
			mainGrid->RemoveView(sumView);
			if(tmpView == this->sumView)
			{
				mainGrid->RemoveView(sumView);
				errorAlert = new ErrorAlert("remove view");
				errorAlert->Launch();
			}
			else
			{
				errorAlert = new ErrorAlert("don't remove view");
				errorAlert->Launch();
			}
			mainGrid->AddView(thoughtView);
			this->mpMenuBar->thoughtsMenuItem->SetEnabled(false);
			// when in a view, might want to invalidate the menu option so it can't be redone and screwed up for now
			// do something here...
			break;
		
		case MNG_LAYOUT_MSG:
			// do something here...
			break;
			
		case MENU_SUM_MSG:
			mainGrid->RemoveView(thoughtView);
			mainGrid->AddView(sumView);
			this->mpMenuBar->summaryMenuItem->SetEnabled(false);
			this->mpMenuBar->thoughtsMenuItem->SetEnabled(true);
			// do something here...
			break;

		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

void MainWindow::FrameResized(float width, float height)
{
}

bool
MainWindow::QuitRequested(void)
{
	sqlite3_free(sqlErrMsg);
	sqlite3_close(mpdb);
	
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void MainWindow::PopulateSummaryView(int mpID)
{
	//tmpString = "select thoughtID, thoughtData from ttable wher
	// place code here to populate summary view based on input id...
}
/*
#ifndef	_CARD_LAYOUT_H
#define	_CARD_LAYOUT_H

#include <AbstractLayout.h>


class BCardLayout : public BAbstractLayout {
public:
								BCardLayout();
								BCardLayout(BMessage* from);
	virtual						~BCardLayout();

			BLayoutItem*		VisibleItem() const;
			int32				VisibleIndex() const;
			void				SetVisibleItem(int32 index);
			void				SetVisibleItem(BLayoutItem* item);

	virtual	BSize				BaseMinSize();
	virtual	BSize				BaseMaxSize();
	virtual	BSize				BasePreferredSize();
	virtual	BAlignment			BaseAlignment();

	virtual	bool				HasHeightForWidth();
	virtual	void				GetHeightForWidth(float width, float* min,
									float* max, float* preferred);

	virtual	void				InvalidateLayout(bool children = false);

	virtual status_t			Archive(BMessage* into, bool deep = true) const;
	virtual status_t			AllUnarchived(const BMessage* from);
	static	BArchivable*		Instantiate(BMessage* from);

protected:
	virtual	void				DerivedLayoutItems();
	virtual	bool				ItemAdded(BLayoutItem* item, int32 atIndex);
	virtual	void				ItemRemoved(BLayoutItem* item, int32 fromIndex);

private:
			BSize				fMin;
			BSize				fMax;
			BSize				fPreferred;
			BLayoutItem*		fVisibleItem;
			bool				fMinMaxValid;

			void				_ValidateMinMax();
};

#endif	// _CARD_LAYOUT_H
*/
