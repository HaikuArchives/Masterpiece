#include "MainWindow.h"
#include "PictureView.h"

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

#define TEXT_INSET 3.0

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,500,400),"MasterPiece",B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE)
{
	BRect r(Bounds());
	r.bottom = 20;
	fMenuBar = new BMenuBar(r,"menubar");
	BMenu* fileMenu = new BMenu("File");
	newFileMenuItem = new BMenuItem("New MasterPiece", new BMessage(MENU_NEW_MSG));
	openFileMenuItem = new BMenuItem("Open Existing", new BMessage(MENU_OPN_MSG));
	quitMenuItem = new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED));

	fileMenu->AddItem(newFileMenuItem);
	fileMenu->AddItem(openFileMenuItem);
	fileMenu->AddSeparatorItem();
	fileMenu->AddItem(quitMenuItem);

	fMenuBar->AddItem(fileMenu);
	
	AddChild(fMenuBar);
	/*
	 * Adding Picture's Test...
	 *
	PictureView *picView = new PictureView();
	toolbarView = new BView(BRect(100, 100, 500, 270), "view", B_FOLLOW_ALL, B_WILL_DRAW);
	testImage = BTranslationUtils::GetBitmapFile("images/document-new.png", NULL);
	//toolbarView->BeginPicture(new BPicture);
	//toolbarView->DrawBitmap(testImage);
	//onImage = toolbarView->EndPicture();
	//toolbarView->AddChild(testImage);
	//AddChild(toolbarView);
	AddChild(picView);
	picView->MoveTo((Bounds().Width() - picView->Bounds().Width()) / 2.0,
					(Bounds().Height() - picView->Bounds().Height()) / 2.0);
	*/
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
	fullView->Hide();
	
	BRect tr = Bounds();
	tr.top = 20;
	mpTabView = new BTabView(tr, "tab_view");
	mpTabView->SetViewColor(myColor);
	tr = mpTabView->Bounds();
	tr.InsetBy(5,5);
	tr.bottom -= mpTabView->TabHeight();
	tmpTab = new BTab();
	mpTabView->AddTab(new BView(tr, 0, B_FOLLOW_ALL, B_WILL_DRAW), tmpTab);
	tmpTab->SetLabel("Thoughts");
	tmpTab = new BTab();
	mpTabView->AddTab(new BView(tr, 0, B_FOLLOW_ALL, B_WILL_DRAW), tmpTab);
	tmpTab->SetLabel("Layout");
	AddChild(mpTabView);
	mpTabView->Hide();
	
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case MENU_NEW_MSG:
			// do something here...
			// 1.  need to center the modal window on the parent...
			//newWindow = new NewWindow();
			//newWindow->Show();
			this->fullView->Show();
			break;
		
		case MENU_OPN_MSG:
			// do something here...
			break;
		
		case ADD_NEW_COURSE:
			//sprintf(comboText, "%s - %s", this->Title(), titleText->Text());
			//comboText = this->Title();
			//strcat((char*)comboText, titleText->Text());
			//this->SetTitle(comboText);
			this->SetTitle(titleText->Text());
			titleText->SetText("");
			this->fullView->Hide();
			// do something here...
			// 1. Also need to create a folder in the file system, or simply an entry in the DB.
			// 2. Also need to show the correct tabset of views...
			this->mpTabView->Show();
			break;
		
		case CANCEL_NEW_COURSE:
			this->fullView->Hide();
			titleText->SetText("");
			// do soemthing here...
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
