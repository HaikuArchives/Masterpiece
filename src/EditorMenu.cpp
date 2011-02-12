#include "EditorMenu.h"

EditorMenu::EditorMenu()
	:	BMenuBar(BRect(0, 0, 10, 10), "editorbar")
{
	// initialize menu and menuitems
	helpMenu = new BMenu("Help");
	newThoughtMenuItem = new BMenuItem("New Thought", new BMessage(MENU_NEW_THT));
	editThoughtMenuItem = new BMenuItem("Edit Thought Name", new BMessage(MENU_EDT_THT));
	saveThoughtMenuItem = new BMenuItem("Save Progress", new  BMessage(MENU_SAV_THT));
	previewMenuItem = new BMenuItem("Preview", new BMessage(MENU_PRV_THT));
	publishMenuItem = new BMenuItem("Publish", new BMessage(MENU_PUB_THT));
	keyboardRefMenuItem = new BMenuItem("Keyboard Reference", new BMessage(MENU_KEY_THT));
	markupRefMenuItem = new BMenuItem("Markup Reference", new BMessage(MENU_MRK_THT));
	helpTopicsMenuItem = new BMenuItem("Help Topics", new BMessage(MENU_HLP_THT));
	aboutMenuItem = new BMenuItem("About", new BMessage(MENU_ABT_THT));
	
	//layout helpMenu
	helpMenu->AddItem(markupRefMenuItem);
	helpMenu->AddItem(keyboardRefMenuItem);
	helpMenu->AddItem(helpTopicsMenuItem);
	helpMenu->AddSeparatorItem();
	helpMenu->AddItem(aboutMenuItem);
	
	//layout menubar
	AddItem(newThoughtMenuItem);
	AddItem(editThoughtMenuItem);
	AddItem(saveThoughtMenuItem);
	AddItem(previewMenuItem);
	AddItem(publishMenuItem);
	AddItem(helpMenu);
	
	// set enabled/disabled triggers at initialization
}
