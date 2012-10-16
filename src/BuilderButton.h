#ifndef BUILDERBUTTON_H
#define BUILDERBUTTON_H

#include "CommonFunctions.h"

#define CLEAR_STATUS	'clst' // send message to clear statusbar

class BuilderButton : public BButton
{
public:
					BuilderButton(BRect frame, const char* label, BMessage* message, const BMessage &msg, const BMessenger &msgr);
		void		MouseDown(BPoint point);
		void		KeyDown(const char* bytes, int32 numBytes);
private:
		BMessage	statusMessage;
		BMessenger	statusMessenger;
};

#endif
