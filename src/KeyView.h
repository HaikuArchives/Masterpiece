#ifndef KEYVIEW_H
#define KEYVIEW_H

#include <View.h>
#include <Window.h>
#include <stdio.h>

class KeyView : public BView
{
public:
						KeyView(BRect frame, const char* name);
		virtual void 	KeyDown(const char *bytes, int32 numBytes);

private:

};

#endif
