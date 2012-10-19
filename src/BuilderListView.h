#ifndef BUILDERLISTVIEW_H
#define BUILDERLISTVIEW_H

#include <BListView.h>

#define CLEAR_STATUS	'clst' // send message to clear statusbar

class BuilderListView : public BListView
{
public:
						BuilderListView(BRect frame, constant BMessage &msg, const BMessenger &msgr);
						MouseDown(BPoint point);
						KeyDown(const char* bytes, int32 numBytes);
private:
			BMessage	statusMessage;
			BMessenger	statusMessenger;
};

#endif
