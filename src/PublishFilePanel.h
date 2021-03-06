#ifndef PUBLISHFILEPANEL_H
#define PUBLISHFILEPANEL_H

#include <FilePanel.h>
#include <Menu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <CheckBox.h>

#include "CommonFunctions.h"

#define PUBLISH_TYPE 'ptyp'

class PublishFilePanel : public BFilePanel
{
	public:
					PublishFilePanel(BMessenger* target);
		BMenuField*	publishTypeMenuField;
		BMenu*		publishTypeMenu;
		BCheckBox*	openCheckBox;
		
	private:
		BMessage*	pubMsg;
};
#endif
