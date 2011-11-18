#ifndef PUBLISHFILEPANEL_H
#define PUBLISHFILEPANEL_H
#include <Message.h>
#include <Messenger.h>
#include <FilePanel.h>
#include <Button.h>
#include <Window.h>
#include <View.h>
#include <Path.h>
#include <Roster.h>
#include <MenuField.h>
#include <Menu.h>
#include <MenuItem.h>
#include <stdio.h>
#include "CommonFunctions.h"

#define PUBLISH_TYPE 'ptyp'

class PublishFilePanel : public BFilePanel
{
	public:
					PublishFilePanel(BMessenger* target);
		BMenuField*	publishTypeMenuField;
		BMenu*		publishTypeMenu;
		
	private:
		BMessage*	pubMsg;
};
#endif
