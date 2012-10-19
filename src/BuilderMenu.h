#ifndef BUILDERMENU_H
#define BUILDERMENU_H

#include <MenuBar.h>
#include <MenuItem.h>

#define MENU_NEW_MP		'new' 	// new thought
#define MENU_EDT_MP		'edit' 	// edit mp name
#define	MENU_SAV_MP		'save'	// save mp
#define MENU_PRV_MP		'prev'	// preview mp
#define MENU_PUB_MP 	'pub'	// publish mp
#define MENU_HLP_MP		'help'	// help topics
#define	MENU_KEY_MP		'key'	// keyboard reference
#define MENU_ABT_MP		'abt'	// about editor
#define CLEAR_STATUS	'clst'  // send message to clear statusbar

class BuilderMenu : public BMenuBar
{
public:
						BuilderMenu(BRect rect, const BMessage &msg, const BMessenger &msgr);// editor menu
			BMenu*		helpMenu;				// help menu
			//BMenu*		publishMenu;			// publish as menu
			BMenu*		settingsMenu;			// settings
			BMenuItem*	newMasterMenuItem;		// new thought menu item
			BMenuItem*	editMPTitleMenuItem;	// edit thought menu item
			BMenuItem*	saveMasterMenuItem;		// save thought menu item
			BMenuItem*	previewMenuItem;		// preview menu item
			BMenuItem*	publishMenuItem;		// publish menu item
			BMenuItem*	importMenuItem;			// import menu item
			BMenuItem*	keyboardRefMenuItem;	// keyboard reference menu item
			BMenuItem*	helpTopicsMenuItem;		// help topics menu item
			BMenuItem*	aboutMenuItem;			// about menu item
			void		MouseDown(BPoint point);
			void		KeyDown(const char* bytes, int32 numBytes);
private:
			BMessage	statusMessage;
			BMessenger	statusMessenger;

};

#endif
