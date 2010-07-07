#ifndef MENUBAR_H
#define MENUBAR_H

#include <MenuBar.h>
#include <MenuItem.h>

#define MENU_NEW_MSG		'new'
#define MENU_OPN_MSG		'open'
#define MENU_THT_MSG		'mtm'
#define MENU_IMG_MSG		'mim'

class MenuBar : public BMenuBar
{
public:
							MenuBar(BRect r);
			BMenu			*fileMenu;
			BMenu			*contentMenu;
			BMenu			*layoutMenu;
			BMenuItem		*newFileMenuItem;
			BMenuItem		*openFileMenuItem;
			BMenuItem		*quitMenuItem;
			BMenuItem		*thoughtsMenuItem;
			BMenuItem		*imagesMenuItem;
private:

};
#endif
