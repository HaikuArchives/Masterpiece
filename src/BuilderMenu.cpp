#include "BuilderMenu.h"

BuilderMenu::BuilderMenu()
	:	BMenuBar(BRect(0, 0, 10, 20), "builderbar")
{
	// initialize menu and menuitems
	helpMenu = new BMenu("Help");
	newMasterMenuItem = new BMenuItem("New Thought", new BMessage(MENU_NEW_MP));
	editMPTitleMenuItem = new BMenuItem("Edit Masterpiece Name", new BMessage(MENU_EDT_MP));
	saveMasterMenuItem = new BMenuItem("Save Progress", new  BMessage(MENU_SAV_MP));
	previewMenuItem = new BMenuItem("Preview", new BMessage(MENU_PRV_MP));
	publishMenu = new BMenu("Publish As...");
	pdfMenuItem = new BMenuItem("PDF", new BMessage(MENU_PDF_MP), 'p', B_SHIFT_KEY);
	htmlMenuItem = new BMenuItem("HTML", new BMessage(MENU_HTM_MP), 'h', B_SHIFT_KEY);
	xmlMenuItem = new BMenuItem("XML", new BMessage(MENU_XML_MP), 'x', B_SHIFT_KEY);
	odtMenuItem = new BMenuItem("ODT", new BMessage(MENU_ODT_MP), 'o', B_SHIFT_KEY);
	keyboardRefMenuItem = new BMenuItem("Keyboard Reference", new BMessage(MENU_KEY_MP));
	helpTopicsMenuItem = new BMenuItem("Help Topics", new BMessage(MENU_HLP_MP));
	aboutMenuItem = new BMenuItem("About", new BMessage(MENU_ABT_MP));
	
	//layout helpMenu
	helpMenu->AddItem(keyboardRefMenuItem);
	helpMenu->AddItem(helpTopicsMenuItem);
	helpMenu->AddSeparatorItem();
	helpMenu->AddItem(aboutMenuItem);
	
	//layout publishMenu
	publishMenu->AddItem(odtMenuItem);
	publishMenu->AddItem(xmlMenuItem);
	publishMenu->AddItem(htmlMenuItem);
	publishMenu->AddItem(pdfMenuItem);
	
	//layout menubar
	AddItem(newMasterMenuItem);
	AddItem(editMPTitleMenuItem);
	AddItem(saveMasterMenuItem);
	AddItem(previewMenuItem);
	AddItem(publishMenu);
	AddItem(helpMenu);
	
	// set enabled/disabled triggers at initialization
}
