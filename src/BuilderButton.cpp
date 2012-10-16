#include "BuilderButton.h"

BuilderButton::BuilderButton(BRect frame, const char* label, BMessage* message, const BMessage &msg, const BMessenger &msgr)
	:	BButton(frame, NULL, label, message, B_FOLLOW_NONE, B_WILL_DRAW | B_NAVIGABLE), statusMessage(msg), statusMessenger(msgr)
{
}
void BuilderButton::MouseDown(BPoint point)
{
	statusMessage.MakeEmpty();
	statusMessage.AddInt64("clearStatus", 1);
	statusMessenger.SendMessage(&statusMessage);
	BButton::MouseDown(point);
}
void BuilderButton::KeyDown(const char* bytes, int32 numBytes)
{
	statusMessage.MakeEmpty();
	statusMessage.AddInt64("clearStatus", 1);
	statusMessenger.SendMessage(&statusMessage);
	BButton::KeyDown(bytes, numBytes);
}
