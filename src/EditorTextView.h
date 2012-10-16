#ifndef EDITORTEXTVIEW_H
#define EDITORTEXTVIEW_H

#include "CommonFunctions.h"

#define	CLEAR_STATUS	'clst' // send message to clear statusbar

class EditorTextView : public BTextView
{
public:
					EditorTextView(BRect frame, const char* name, BRect textRect, uint32 resizingMode, uint32 flags, const BMessage &msg, const BMessenger &msgr);
		void		MouseDown(BPoint point);
		void		KeyDown(const char* bytes, int32 numBytes);
private:
		BMessage	statusMessage;
		BMessenger	statusMessenger;		
};

#endif
