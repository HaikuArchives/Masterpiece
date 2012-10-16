#include "EditorTextView.h"

EditorTextView::EditorTextView(BRect frame, const char* name, BRect textRect, uint32 resizingMode, uint32 flags, const BMessage &msg, const BMessenger &msgr)
	:	BTextView(frame, name, textRect, resizingMode, flags), statusMessage(msg), statusMessenger(msgr)
{
	
}
void EditorTextView::MouseDown(BPoint point)
{
	statusMessage.MakeEmpty();
	statusMessage.AddInt64("clearStatus", 1);
	statusMessenger.SendMessage(&statusMessage);
	BTextView::MouseDown(point);
}
void EditorTextView::KeyDown(const char* bytes, int32 numBytes)
{
	statusMessage.MakeEmpty();
	statusMessage.AddInt64("clearStatus", 1);
	statusMessenger.SendMessage(&statusMessage);
	BTextView::KeyDown(bytes, numBytes);
}
