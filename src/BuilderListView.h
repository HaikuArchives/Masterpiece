#ifndef BUILDERLISTVIEW_H
#define BUILDERLISTVIEW_H

#include <ListView.h>

#define CLEAR_STATUS	'clst' // send message to clear statusbar

class BuilderListView : public BListView
{
public:
						BuilderListView(BRect frame, const BMessage &msg, const BMessenger &msgr);
			void		MouseDown(BPoint point);
			void		KeyDown(const char* bytes, int32 numBytes);
private:
			BMessage	statusMessage;
			BMessenger	statusMessenger;
};

#endif
