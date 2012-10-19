#include "BuilderListView.h"

BuilderListView::BuilderListView(BRect frame, const BMessage &msg, const BMessenger &msgr)
	:	BListView(frame, NULL, B_SINGLE_SELECTION_LIST, B_FOLLOW_TOP | B_FOLLOW_LEFT, B_WILL_DRAW | B_NAVIGABLE), statusMessage(msg), statusMessenger(msgr)
{
}
void BuilderListView::MouseDown(BPoint point)
{
	statusMessage.MakeEmpty();
	statusMessage.AddInt64("clearStatus", 1);
	statusMessenger.SendMessage(&statusMessage);
	BListView::MouseDown(point);
}
void BuilderListView::KeyDown(const char* bytes, int32 numBytes)
{
	statusMessage.MakeEmpty();
	statusMessage.AddInt64("clearStatus", 1);
	statusMessenger.SendMessage(&statusMessage);
	BListView::KeyDown(bytes, numBytes);
}
