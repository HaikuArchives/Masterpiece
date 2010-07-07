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
}


void
MainWindow::MessageReceived(BMessage *msg)
{

	homeDir = new BDirectory("/boot/home/MasterPiece");

	switch (msg->what)
	{
		case MENU_NEW_MSG:
			// do something here...
			// 1.  need to center the modal window on the parent...
			this->contentTabView->Hide();
			this->openView->Hide();
			this->fullView->Show();
			break;
		
		case MENU_OPN_MSG:
			// do something here...
			this->contentTabView->Hide();
			this->fullView->Hide();
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
			this->openView->Show();
			break;
		
		case ADD_NEW_COURSE:
			homeEntry = BEntry("/boot/home/MasterPiece", false);
			if(!homeEntry.Exists()) // does not exist
			{
				// create MasterPiece directory...
			}
			else // does exist
			{
				
				// directory exists, must not create the course...
				BString tmpString;
				int returnValue = homeDir->CreateDirectory(this->fullView->titleText->Text(), homeDir);
				if(returnValue == B_FILE_EXISTS)
				{
					tmpString = "Do you want to open the course ";
					tmpString += this->fullView->titleText->Text();
					tmpString += "?";
				}
				debugAlert = new BAlert("Debug Value", tmpString, "Yes", "No", 0, B_WIDTH_AS_USUAL, B_INFO_ALERT);
				debugAlert->MoveTo(350, 250);
				debugAlert->SetShortcut(1, B_ESCAPE);
				if(returnValue != B_OK)
				{
					int alertReturn = debugAlert->Go();
					if(alertReturn == 0) // Yes
					{
						this->SetTitle(this->fullView->titleText->Text());
						this->fullView->Hide();
						//this->manageMenu->SetEnabled(true);
						this->contentTabView->Show();
					}
					else if(alertReturn == 1) // No
					{
						this->fullView->titleText->SetText("");
					}
				}
				else
				{
					this->SetTitle(this->fullView->titleText->Text()); // move into proper if statement
					this->fullView->Hide();
					//this->manageMenu->SetEnabled(true);
					this->contentTabView->Show();
				}
			}
			// do something here...
			// 1. Also need to create a folder in the file system, or simply an entry in the DB.
			// 2. Also need to show the correct tabset of views...
			this->fullView->titleText->SetText("");
			break;
		
		case CANCEL_NEW_COURSE:
			this->fullView->Hide();
			this->fullView->titleText->SetText("");
			// do soemthing here...
			break;
			
		case MNG_CONTENT_MSG:
			this->contentTabView->Show();
			// do something here...
			break;
			
		case MNG_LAYOUT_MSG:
			this->contentTabView->Hide();
			// do something here...
			break;
		case CANCEL_OPEN_COURSE:
			this->openView->Hide();
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
				this->openView->Hide();
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
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
