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
	thoughtsMenuItem = new BMenuItem("Thoughts", new BMessage(MENU_THT_MSG));
	imagesMenuItem = new BMenuItem("Images", new BMessage(MENU_IMG_MSG));	
	
	fileMenu->AddItem(newFileMenuItem);
	fileMenu->AddItem(openFileMenuItem);
	fileMenu->AddSeparatorItem();
	fileMenu->AddItem(quitMenuItem);
	
	contentMenu->AddItem(thoughtsMenuItem);
	contentMenu->AddItem(imagesMenuItem);
	
	AddItem(fileMenu);
	AddItem(contentMenu);
	AddItem(layoutMenu);
	contentMenu->SetEnabled(false);
	layoutMenu->SetEnabled(false);
		
}

