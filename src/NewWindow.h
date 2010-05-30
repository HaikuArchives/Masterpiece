#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <Window.h>
#include <View.h>
#include <TextView.h>

class NewWindow : public BWindow
{
public:
						NewWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
private:
			BTextView	*titleText;

};

#endif
