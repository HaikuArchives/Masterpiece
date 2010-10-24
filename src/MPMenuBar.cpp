#include "MPMenuBar.h"

MPMenuBar::MPMenuBar(BRect r)
	:	BMenuBar(r, "menubar")
{
	// initialize menu's and menuitems
	fileMenu = new BMenu("File");
	contentMenu = new BMenu("Content");
	layoutMenu = new BMenu("Layout");
	newFileMenuItem = new BMenuItem("New MasterPiece", new BMessage(MENU_NEW_MSG));
	openFileMenuItem = new BMenuItem("Open Existing", new BMessage(MENU_OPN_MSG));
	closeFileMenuItem = new BMenuItem("Close Current", new BMessage(MENU_CLS_MSG));
	quitMenuItem = new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED));
	thoughtsMenuItem = new BMenuItem("Thoughts", new BMessage(MENU_THT_MSG));
	imagesMenuItem = new BMenuItem("Images", new BMessage(MENU_IMG_MSG));	
	
	// layout FileMenu
	fileMenu->AddItem(newFileMenuItem);
	fileMenu->AddItem(openFileMenuItem);
	fileMenu->AddItem(closeFileMenuItem);
	fileMenu->AddSeparatorItem();
	fileMenu->AddItem(quitMenuItem);
	// Layout ContentMenu
	contentMenu->AddItem(thoughtsMenuItem);
	contentMenu->AddItem(imagesMenuItem);
	// Layout MainMenuBar
	AddItem(fileMenu);
	AddItem(contentMenu);
	AddItem(layoutMenu);
	// Set Enabled/Disabled Triggers at Initialization
	contentMenu->SetEnabled(false);
	layoutMenu->SetEnabled(false);
	closeFileMenuItem->SetEnabled(false);
		
}

