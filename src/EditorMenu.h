#ifndef EDITORMENU_H
#define EDITORMENU_H

#include <MenuBar.h>
#include <MenuItem.h>
#include <GroupLayout.h>
#include <SpaceLayoutItem.h>
#include <GridLayoutBuilder.h>

#define MENU_NEW_THT	'new' 	// new thought
#define MENU_EDT_THT	'edit' 	// edit thought name
#define	MENU_SAV_THT	'save'	// save thought
#define MENU_PRV_THT	'prev'	// preview thought
#define MENU_PUB_THT	'pub'	// publish thought
#define MENU_HLP_THT	'help'	// help topics
#define	MENU_KEY_THT	'key'	// keyboard reference
#define MENU_MRK_THT	'mark'	// markup reference
#define MENU_ABT_THT	'abt'	// about editor

class EditorMenu : public BMenuBar
{
public:
						EditorMenu();			// editor menu
			BMenu*		helpMenu;				// help menu
			BMenuItem*	newThoughtMenuItem;		// new thought menu item
			BMenuItem*	editThoughtMenuItem;	// edit thought menu item
			BMenuItem*	saveThoughtMenuItem;	// save thought menu item
			BMenuItem*	previewMenuItem;		// preview menu item
			BMenuItem*	publishMenuItem;		// publish menu item
			BMenuItem*	keyboardRefMenuItem;	// keyboard reference menu item
			BMenuItem*	markupRefMenuItem;		// markup reference menu item
			BMenuItem*	helpTopicsMenuItem;		// help topics menu item
			BMenuItem*	aboutMenuItem;			// about menu item
private:

};

#endif
