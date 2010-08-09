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
	sqlValue = sqlite3_open("./MasterPiece.db", &mpdb);
	fprintf(stdout, "open: %s\n", sqlite3_errmsg(mpdb));

	/*
	* NEED TO ABSTRACT THE THOUGHT STUFF TO A TAB
	BRect tr = Bounds();
	tr.top = 20;
	contentTabView = new BTabView(tr, "tab_view");
	contentTabView->SetViewColor(myColor);
	tr = contentTabView->Bounds();
	tr.InsetBy(5,5);
	tr.bottom -= contentTabView->TabHeight();
	tmpTab = new BTab();
	contentTabView->AddTab(new BView(tr, 0, B_FOLLOW_ALL, B_WILL_DRAW), tmpTab);
	tmpTab->SetLabel("Thoughts");
	tmpTab = new BTab();
	contentTabView->AddTab(new BView(tr, 0, B_FOLLOW_ALL, B_WILL_DRAW), tmpTab);
	tmpTab->SetLabel("Images");
	AddChild(contentTabView);
	contentTabView->Hide();
	*/
	// Check that the MasterPiece directory exists and create it if it doesn't
	// possibly check if user want's to create it or select a new one...
	homeEntry = BEntry("/boot/home/MasterPiece", false);
	if(!homeEntry.Exists()) // does not exist
	{
		// create MasterPiece directory...
	}
	else
	{
		homeDir = new BDirectory("/boot/home/MasterPiece");
	}
}

static int ReturnCount(void *NotUsed, int resultCount, char **colResults, char **colName)
{
	int i;
	for(i=0;i<resultCount;i++)
	{
		fprintf(stdout, "count: %d, %s = %s\n", resultCount, colName[i], colResults[i]);
	}
	return 0;
} 

void MainWindow::MessageReceived(BMessage *msg)
{
	
	switch (msg->what)
	{
		case MENU_NEW_MSG:
			// 1.  need to center the modal window on the parent...
			if(!this->sumView->IsHidden()) this->sumView->Hide();
			if(!this->openView->IsHidden()) this->openView->Hide();
			if(this->fullView->IsHidden()) this->fullView->Show();
			break;
		
		case MENU_OPN_MSG:
			if(!this->sumView->IsHidden()) this->sumView->Hide();
			if(!this->fullView->IsHidden()) this->fullView->Hide();
			this->openView->openListView->MakeEmpty();
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
			if(this->openView->IsHidden()) this->openView->Show();
			
			break;
		
		case ADD_NEW_COURSE:
			tmpString = "select masterPieceName from mptable where masterpieceName = '";
			tmpString += this->fullView->titleText->Text();
			tmpString += "';";
			/*
			tmpString = "insert into mptable (masterpieceName) values('";
			tmpString += this->fullView->titleText->Text();
			tmpString += "');";
			*/
			//sqlValue = sqlite3_exec(mpdb, tmpString, ReturnCount, NULL, &sqlErrMsg);
			sqlValue = sqlite3_get_table(mpdb, tmpString, &selectResult, &nrow, &ncol, &sqlErrMsg);
			if(sqlValue == SQLITE_OK)
			{
				if(nrow >= 1)
				{
					fprintf(stdout, "count: %d, %s = %s\n", nrow, selectResult[0], selectResult[1]);
				}
			}
			fprintf(stdout, "count check errors: %s\n", sqlErrMsg);
			// frpintf(stdout, "count value check: %d\n", 
			// fprintf(stdout, "insert: %s", sqlErrMsg);
			// possibly check result before doing the following:
			this->SetTitle(this->fullView->titleText->Text());
			if(!this->fullView->IsHidden()) this->fullView->Hide();
			if(!this->openView->IsHidden()) this->openView->Hide();
			if(this->sumView->IsHidden()) this->sumView->Show();
			this->mpMenuBar->contentMenu->SetEnabled(true);
			this->mpMenuBar->layoutMenu->SetEnabled(true);			
			
			/*
			returnValue = homeDir->CreateDirectory(this->fullView->titleText->Text(), homeDir);
			if(returnValue == B_FILE_EXISTS)
			{
				tmpString = "Do you want to open the course ";
				tmpString += this->fullView->titleText->Text();
				tmpString += "?";
			}
			if(returnValue != B_OK)
			{
				debugAlert = new BAlert("Debug Value", tmpString, "Yes", "No", 0, B_WIDTH_AS_USUAL, B_INFO_ALERT);
				debugAlert->MoveTo(350, 250);
				debugAlert->SetShortcut(1, B_ESCAPE);
				int alertReturn = debugAlert->Go();
				if(alertReturn == 0) // Yes
				{
					this->SetTitle(this->fullView->titleText->Text());
					if(!this->fullView->IsHidden()) this->fullView->Hide();
				}
				else if(alertReturn == 1) // No
				{
					this->fullView->titleText->SetText("");
				}
			}
			else
			{
				this->SetTitle(this->fullView->titleText->Text());
				if(!this->fullView->IsHidden()) this->fullView->Hide();
				if(!this->openView->IsHidden()) this->openView->Hide();
				if(this->sumView->IsHidden()) this->sumView->Show();
				this->mpMenuBar->contentMenu->SetEnabled(true);
				this->mpMenuBar->layoutMenu->SetEnabled(true);
			}
			// do something here...
			*/
			this->fullView->titleText->SetText("");
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
