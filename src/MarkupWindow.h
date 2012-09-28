#ifndef MARKUPWINDOW_H
#define MARKUPWINDOW_H

#include "HelpStringItem.h"
#include "CommonFunctions.h"

#define LOAD_CONTENT	'ldct' // load content in window

class MarkupWindow : public BWindow
{
public:
				MarkupWindow(BRect frame, const char* title);
		void	MessageReceived(BMessage* msg);
		void	AddMarkupItem(BString topicstring, BString contentstring);
		bool	QuitRequested(void);
		
private:
		BView*			backView;
		BListView*		topicListView;
		BTextView*		contentTextView;
		ErrorAlert*		eAlert;
		int				selected;
};

#endif
