#ifndef PUBLISHFILEPANEL_H
#define PUBLISHFILEPANEL_H

class PublishFilePanel : public BFilePanel
{
	public:
		PublishFilePanel(BMessenger* target, BRefFilter* filter=NULL);
		void SelectionChanged();
		
	private:
		// popupmenu item as a dropdown selector
};

#endif
