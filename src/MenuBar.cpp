#include "MenuBar.h"

MenuBar::MenuBar(BRect r)
	:	BMenuBar(r, "menubar")
{
	fileMenu = new BMenu("File");
	contentMenu = new BMenu("Content");
	layoutMenu = new BMenu("Layout");
	newFileMenuItem = new BMenuItem("New MasterPiece", new BMessage(MENU_NEW_MSG));
	openFileMenuItem = new BMenuItem("Open Existing", new BMessage(MENU_OPN_MSG));
	quitMenuItem = new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED));
	
	fileMenu->AddItem(newFileMenuItem);
	fileMenu->AddItem(openFileMenuItem);
	fileMenu->AddSeparatorItem();
	fileMenu->AddItem(quitMenuItem);
	
	AddItem(fileMenu);
	AddItem(contentMenu);
	AddItem(layoutMenu);
		
}

