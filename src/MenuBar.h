#ifndef MENUBAR_H
#define MENUBAR_H

#include <MenuBar.h>
#include <MenuItem.h>

#define MENU_NEW_MSG		'new'
#define MENU_OPN_MSG		'open'

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

private:

};
#endif
