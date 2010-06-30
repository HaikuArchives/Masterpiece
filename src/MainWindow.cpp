#include "MainWindow.h"

#include <Application.h>
#include <Menu.h>
#include <MenuItem.h>
#include <View.h>
#include <TranslationUtils.h>
#include <TranslatorFormats.h>
#include <Bitmap.h>
#include <StringView.h>
#include <Button.h>
#include <stdio.h>
#include <String.h>
#include <Directory.h>

#define TEXT_INSET 3.0

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,900,700),"MasterPiece",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE)
{
	BRect r(Bounds());
	r.bottom = 20;
	fMenuBar = new BMenuBar(r,"menubar");
	fileMenu = new BMenu("File");
	manageMenu = new BMenu("Manage");
	newFileMenuItem = new BMenuItem("New MasterPiece", new BMessage(MENU_NEW_MSG));
	openFileMenuItem = new BMenuItem("Open Existing", new BMessage(MENU_OPN_MSG));
	quitMenuItem = new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED));
	
	contentMenuItem = new BMenuItem("Content", new BMessage(MNG_CONTENT_MSG));
	layoutMenuItem = new BMenuItem("Layout", new BMessage(MNG_LAYOUT_MSG));

	fileMenu->AddItem(newFileMenuItem);
	fileMenu->AddItem(openFileMenuItem);
	fileMenu->AddSeparatorItem();
	fileMenu->AddItem(quitMenuItem);
	
	manageMenu->AddItem(contentMenuItem);
	manageMenu->AddItem(layoutMenuItem);

	fMenuBar->AddItem(fileMenu);
	fMenuBar->AddItem(manageMenu);
	manageMenu->SetEnabled(false);
	
	AddChild(fMenuBar);
	BRect viewFrame(110, 10, 280, 30);
	BRect textFrame;
	textFrame.left = TEXT_INSET;
	textFrame.right = viewFrame.right - viewFrame.left - TEXT_INSET;
	textFrame.top = TEXT_INSET;
	textFrame.bottom = viewFrame.bottom - viewFrame.top - TEXT_INSET;
	fullView = new BView(BRect(30, 100, 330, 200), 0, B_FOLLOW_ALL, B_WILL_DRAW);
	BStringView *titleString = new BStringView(BRect(10, 10, 100, 20), NULL, "Enter Title:");
	titleText = new BTextView(viewFrame, "textTitle", textFrame, B_FOLLOW_NONE, B_WILL_DRAW);
	BButton *addButton = new BButton(BRect(190, 50, 270, 75), NULL, "Add", new BMessage(ADD_NEW_COURSE), B_FOLLOW_NONE, B_WILL_DRAW);
	BButton *cancelButton = new BButton(BRect(100, 50, 180, 75), NULL, "Cancel", new BMessage(CANCEL_NEW_COURSE), B_FOLLOW_NONE, B_WILL_DRAW);
	fullView->AddChild(titleString);
	fullView->AddChild(addButton);
	fullView->AddChild(cancelButton);
	fullView->AddChild(titleText);
	AddChild(fullView);
	viewFrame.InsetBy(-2.0, -2.0);
	rgb_color myColor = {215, 215, 215, 255};
	fullView->SetViewColor(myColor);
	titleString->SetViewColor(myColor);
	addButton->SetLowColor(myColor);
	cancelButton->SetLowColor(myColor);
	fullView->Hide();
	
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

	switch (msg->what)
	{
		case MENU_NEW_MSG:
			// do something here...
			// 1.  need to center the modal window on the parent...
			this->contentTabView->Hide();
			this->fullView->Show();
			break;
		
		case MENU_OPN_MSG:
			// do something here...
			break;
		
		case ADD_NEW_COURSE:
			homeEntry = BEntry("/boot/home/MasterPiece", false);
			homeDir = new BDirectory("/boot/home/MasterPiece");
			if(!homeEntry.Exists()) // does not exist
			{
				// create MasterPiece directory...
			}
			else // does exist
			{
				
				// directory exists, must not create the course...
				BString tmpString;
				int returnValue = homeDir->CreateDirectory(titleText->Text(), homeDir);
				if(returnValue == B_FILE_EXISTS)
				{
					tmpString = "Do you want to open the course ";
					tmpString += titleText->Text();
					tmpString += "?";
				}
				debugAlert = new BAlert("Debug Value", tmpString, "Yes", "No", 0, B_WIDTH_AS_USUAL, B_INFO_ALERT);
				debugAlert->SetShortcut(1, B_ESCAPE);
				if(returnValue != B_OK)
				{
					int alertReturn = debugAlert->Go();
					if(alertReturn == 0) // Yes
					{
						this->SetTitle(titleText->Text());
						this->fullView->Hide();
						this->manageMenu->SetEnabled(true);
						this->contentTabView->Show();
					}
					else if(alertReturn == 1) // No
					{
						titleText->SetText("");
					}
				}
				else
				{
					this->SetTitle(titleText->Text()); // move into proper if statement
					this->fullView->Hide();
					this->manageMenu->SetEnabled(true);
					this->contentTabView->Show();
				}
			}
			// do something here...
			// 1. Also need to create a folder in the file system, or simply an entry in the DB.
			// 2. Also need to show the correct tabset of views...
			titleText->SetText("");
			break;
		
		case CANCEL_NEW_COURSE:
			this->fullView->Hide();
			titleText->SetText("");
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
