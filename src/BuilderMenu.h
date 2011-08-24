#ifndef BUILDERMENU_H
#define BUILDERMENU_H

#include <MenuBar.h>
#include <MenuItem.h>

#define MENU_NEW_MP	'new' 	// new thought
#define MENU_EDT_MP	'edit' 	// edit mp name
#define	MENU_SAV_MP	'save'	// save mp
#define MENU_PRV_MP	'prev'	// preview mp
#define MENU_PUB_MP 'pub'	// publish mp
/*
#define MENU_PDF_MP	'pdf'	// publish pdf mp
#define MENU_HTM_MP	'html'	// publish html mp
#define MENU_XML_MP	'xml'	// publish xml mp
#define MENU_ODT_MP	'odt' 	// publish odt mp
#define MENU_TEX_MP	'tex'	// publish latex mp
*/
#define MENU_HLP_MP	'help'	// help topics
#define	MENU_KEY_MP	'key'	// keyboard reference
#define MENU_ABT_MP	'abt'	// about editor

class BuilderMenu : public BMenuBar
{
public:
						BuilderMenu();			// editor menu
			BMenu*		helpMenu;				// help menu
			BMenu*		publishMenu;			// publish as menu
			BMenuItem*	newMasterMenuItem;		// new thought menu item
			BMenuItem*	editMPTitleMenuItem;	// edit thought menu item
			BMenuItem*	saveMasterMenuItem;		// save thought menu item
			BMenuItem*	previewMenuItem;		// preview menu item
			BMenuItem*	publishMenuItem;		// publish menu item
			/*
			BMenuItem*	pdfMenuItem;			// publish pdf menu item
			BMenuItem*	htmMenuItem;			// publish html menu item
			BMenuItem*	xmlMenuItem;			// publish xml menu item
			BMenuItem*	odtMenuItem;			// publish odt menu item
			BMenuItem*	texMenuItem;			// publish latex menu item
			*/
			BMenuItem*	keyboardRefMenuItem;	// keyboard reference menu item
			BMenuItem*	helpTopicsMenuItem;		// help topics menu item
			BMenuItem*	aboutMenuItem;			// about menu item
private:

};

#endif
