#include "BuilderMenu.h"

BuilderMenu::BuilderMenu()
	:	BMenuBar(BRect(0, 0, 10, 20), "builderbar")
{
	// initialize menu and menuitems
	helpMenu = new BMenu("Help");
	newMasterMenuItem = new BMenuItem("New Thought", new BMessage(MENU_NEW_MP));
	editMPTitleMenuItem = new BMenuItem("Edit Thought Name", new BMessage(MENU_EDT_MP));
	saveMasterMenuItem = new BMenuItem("Save Progress", new  BMessage(MENU_SAV_MP));
	previewMenuItem = new BMenuItem("Preview", new BMessage(MENU_PRV_MP));
	publishMenuItem = new BMenuItem("Publish", new BMessage(MENU_PUB_MP));
	keyboardRefMenuItem = new BMenuItem("Keyboard Reference", new BMessage(MENU_KEY_MP));
	helpTopicsMenuItem = new BMenuItem("Help Topics", new BMessage(MENU_HLP_MP));
	aboutMenuItem = new BMenuItem("About", new BMessage(MENU_ABT_MP));
	
	//layout helpMenu
	helpMenu->AddItem(keyboardRefMenuItem);
	helpMenu->AddItem(helpTopicsMenuItem);
	helpMenu->AddSeparatorItem();
	helpMenu->AddItem(aboutMenuItem);
	
	//layout menubar
	AddItem(newMasterMenuItem);
	AddItem(editMPTitleMenuItem);
	AddItem(saveMasterMenuItem);
	AddItem(previewMenuItem);
	AddItem(publishMenuItem);
	AddItem(helpMenu);
	
	// set enabled/disabled triggers at initialization
}
