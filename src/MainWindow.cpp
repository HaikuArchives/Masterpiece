#include "MainWindow.h"
#include "NewWindow.h"

#include <Application.h>
#include <Menu.h>
#include <MenuItem.h>
#include <View.h>
#include <TranslationUtils.h>
#include <TranslatorRoster.h>
#include <TranslatorFormats.h>
#include <TypeConstants.h>
#include <TranslationDefs.h>
#include <Bitmap.h>

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
	toolbarView = new BView(BRect(100, 100, 500, 270), "view", B_FOLLOW_ALL, B_WILL_DRAW);
	testImage = BTranslationUtils::GetBitmapFile("images/document-new.png", NULL);
	//toolbarView->AddChild(testImage);
	AddChild(toolbarView);
}


void
MainWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case MENU_NEW_MSG:
			// do something here...
			// 1.  need to center the modal window on the parent...
			newWindow = new NewWindow();
			newWindow->Show();
			break;
		
		case MENU_OPN_MSG:
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
