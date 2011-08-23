#ifndef PUBLISHFILEPANEL_H
#define PUBLISHFILEPANEL_H

#include <Message.h>
#include <Messenger.h>
#include <FilePanel.h>
#include <Button.h>
#include <Window.h>
#include <View.h>

class PublishFilePanel : public BFilePanel
{
	public:
		PublishFilePanel(BMessenger* target, BRefFilter* filter=NULL);
		void SelectionChanged();
		
	private:
		// popupmenu item as a dropdown selector
		BButton* fCurrentDirBtn;
};

#endif
