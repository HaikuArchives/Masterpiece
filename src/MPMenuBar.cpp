#include "MPMenuBar.h"

MPMenuBar::MPMenuBar(BRect r)
	:	BMenuBar(r, "menubar")
{
	// initialize menu's and menuitems
	fileMenu = new BMenu("File");
	contentMenu = new BMenu("Content");
	layoutMenu = new BMenu("Layout");
	helpMenu = new BMenu("Help");
	newFileMenuItem = new BMenuItem("New MasterPiece", new BMessage(MENU_NEW_MSG));
	openFileMenuItem = new BMenuItem("Open Existing", new BMessage(MENU_OPN_MSG));
	closeFileMenuItem = new BMenuItem("Close Current", new BMessage(MENU_CLS_MSG));
	quitMenuItem = new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED));
	summaryMenuItem = new BMenuItem("Summary", new BMessage(MENU_SUM_MSG));
	thoughtsMenuItem = new BMenuItem("Thoughts", new BMessage(MENU_THT_MSG));
	imagesMenuItem = new BMenuItem("Images", new BMessage(MENU_IMG_MSG));	
	markupHelpMenuItem = new BMenuItem("Markup Help", new BMessage(MENU_MRK_MSG));
	
	// layout FileMenu
	fileMenu->AddItem(newFileMenuItem);
	fileMenu->AddItem(openFileMenuItem);
	fileMenu->AddItem(closeFileMenuItem);
	fileMenu->AddSeparatorItem();
	fileMenu->AddItem(quitMenuItem);
	// Layout ContentMenu
	contentMenu->AddItem(summaryMenuItem);
	contentMenu->AddSeparatorItem();
	contentMenu->AddItem(thoughtsMenuItem);
	contentMenu->AddItem(imagesMenuItem);
	// Layout HelpMenu
	helpMenu->AddItem(markupHelpMenuItem);
	// Layout MainMenuBar
	AddItem(fileMenu);
	AddItem(contentMenu);
	AddItem(layoutMenu);
	AddItem(helpMenu);
	// Set Enabled/Disabled Triggers at Initialization
	contentMenu->SetEnabled(false);
	layoutMenu->SetEnabled(false);
	closeFileMenuItem->SetEnabled(false);
		
}

