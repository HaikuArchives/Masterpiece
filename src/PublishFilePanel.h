#ifndef PUBLISHFILEPANEL_H
#define PUBLISHFILEPANEL_H

#include <FilePanel.h>
#include <Menu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Query.h>
#include <VolumeRoster.h>
#include <Volume.h>
#include <CheckBox.h>

#include "CommonFunctions.h"

#define PUBLISH_TYPE 'ptyp'
#define PUBLISH_OPEN 'popn'

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
