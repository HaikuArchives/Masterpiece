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

#define PUBLISH_TYPE	'ptyp'

class PublishFilePanel : public BFilePanel
{
	public:
		PublishFilePanel(BMessenger* target);
		void SelectionChanged();
		
	private:
		// popupmenu item as a dropdown selector
		BMenuField*	publishTypeMenuField;
		BMenu*		publishTypeMenu;
		BButton* fCurrentDirBtn;
};
#endif
