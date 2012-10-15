#ifndef EDITORTEXTVIEW_H
#define EDITORTEXTVIEW_H

#include "CommonFunctions.h"

class EditorTextView : public BTextView
{
public:
					EditorTextView(BRect frame, const char* name, BRect textRect, uint32 resizingMode, uint32 flags);
		void		MessageReceived(BMessage* msg);
		void		MouseDown(BPoint point);
		void		KeyDown(const char* bytes, int32 numBytes);
private:
		BPoint		pt;
};

#endif
