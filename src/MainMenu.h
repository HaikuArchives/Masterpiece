#ifndef MAINMENU_H
#define MAINMENU_H

#include <MenuBar.h>
#include <MenuItem.h>
#include <String.h>

#define MENU_NEW_THT	'new' 	// new thought
#define MENU_EDT_THT	'edit' 	// edit thought name
#define	MENU_SAV_THT	'save'	// save thought
#define MENU_PRV_THT	'prev'	// preview thought
#define MENU_PUB_THT	'pub'	// publish thought
#define MENU_HLP_THT	'help'	// help topics
#define	MENU_KEY_THT	'key'	// keyboard reference
#define MENU_QKS_THT	'qks'	// quick start reference
#define MENU_RST_THT	'rst'	// rst guide
#define MENU_DRT_THT	'drt'	// directives reference
#define MENU_QKR_THT	'qkr'	// quick reference
#define MENU_QKE_THT	'qke'	// quick start example
#define MENU_CHE_THT	'che'	// cheat sheet example
#define MENU_CHT_THT	'cht'	// cheat sheet reference
#define MENU_ABT_THT	'abt'	// about editor
#define MENU_MPH_THT	'mph'	// masterpiece tutorial
#define CLEAR_STATUS	'clst'  // send message to clear statusbar

class MainMenu : public BMenuBar
{
public:
						MainMenu(BRect rect, BString editText, const BMessage &msg, const BMessenger &msgr);	// editor menu
			BMenu*		helpMenu;				// help menu
			BMenu*		topicMenu;				// help topics sub menu
			BMenu*		exampleMenu;			// example files
			BMenuItem*	newThoughtMenuItem;		// new thought menu item
			BMenuItem*	editThoughtMenuItem;	// edit thought menu item
			BMenuItem*	saveThoughtMenuItem;	// save thought menu item
			BMenuItem*	previewMenuItem;		// preview menu item
			BMenuItem*	publishMenuItem;		// publish menu item
			BMenuItem*	quickStartMenuItem;		// quick start menu item
			BMenuItem*	cheatMenuItem;			// cheatsheat menu item
			BMenuItem*	guideMenuItem;			// full guide menu item
			BMenuItem*	directMenuItem;			// directives menu item
			BMenuItem*	quickExMenuItem;		// quick start example menu item
			BMenuItem*	cheatExMenuItem;		// cheat sheet example menu item
			BMenuItem*	quickRefMenuItem;		// restructured quick ref menu item
			BMenuItem*	keyboardRefMenuItem;	// keyboard reference menu item
			BMenuItem*	aboutMenuItem;			// about menu item
			BMenuItem*	mphelpMenuItem;			// masterpiece tutorial menu item
			void		MouseDown(BPoint point);
			void		KeyDown(const char* bytes, int32 numBytes);
private:
			BMessage	statusMessage; 
			BMessenger	statusMessenger;
};

#endif
