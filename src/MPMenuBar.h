#ifndef MENUBAR_H
#define MENUBAR_H

#include <MenuBar.h>
#include <MenuItem.h>

#define MENU_NEW_MSG		'new'					// new menu message
#define MENU_OPN_MSG		'open'					// open menu message
#define MENU_THT_MSG		'mtm'					// thought menu message
#define MENU_IMG_MSG		'mim'					// image menu message

class MenuBar : public BMenuBar
{
public:
							MenuBar(BRect r);		// MenuBar function with rect r
			BMenu			*fileMenu;				// File Menu
			BMenu			*contentMenu;			// Content Menu
			BMenu			*layoutMenu;			// Layout Menu
			BMenuItem		*newFileMenuItem;		// New Masterpiece MenuItem
			BMenuItem		*openFileMenuItem;		// Open Masterpiece MenuItem
			BMenuItem		*quitMenuItem;			// Quit Application MenuItem
			BMenuItem		*thoughtsMenuItem;		// Thoughts View MenuItem
			BMenuItem		*imagesMenuItem;		// Images View MenuItem
private:

};

#endif
