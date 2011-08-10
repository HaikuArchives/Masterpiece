#ifndef KEYLISTVIEW_H
#define KEYLISTVIEW_H

#include <ListView.h>
#include <Window.h>
#include <stdio.h>

class KeyListView : public BListView
{
public:
						KeyListView(BRect frame);
		virtual void 	KeyDown(const char *bytes, int32 numBytes);

private:

};

#endif
