#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include "CommonFunctions.h"

#define LOAD_CONTENT	'ldct' // load content in window


class AboutWindow : public BWindow
{
public:
				AboutWindow(BRect frame, const char* title);
		void	MessageReceived(BMessage* msg);
		bool	QuitRequested(void);
		
private:
		BView*			backView;
		BTextView*		creditView;
		ErrorAlert*		eAlert;
};

#endif
