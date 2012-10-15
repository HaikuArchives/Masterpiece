#include "EditorTextView.h"

EditorTextView::EditorTextView(BRect frame, const char* name, BRect textRect, uint32 resizingMode, uint32 flags, const BMessage &msg, const BMessenger &msgr)
	:	BTextView(frame, name, textRect, resizingMode, flags), statusMessage(msg), statusMessenger(msgr)
{
	
}
void EditorTextView::MessageReceived(BMessage* msg)
{
	/*
	switch(msg->what)
	{
		case B_MOUSE_DOWN:
		{
			if(msg->FindPoint("where", &pt) == B_OK)
			{
			}
			printf("it worked\n");
			//BTextView::MouseDown(pt);
			break;
		}
		case B_KEY_DOWN:
		{
			printf("key down\n");
			//BTextView::KeyDown(bytes, numBytes);
			break;
		}
		default:
		{
			BTextView::MessageReceived(msg);
		}
	}
	*/
	BTextView::MessageReceived(msg);
}
void EditorTextView::MouseDown(BPoint point)
{
	statusMessage.MakeEmpty();
	statusMessage.AddInt64("clearStatus", 1);
	statusMessenger.SendMessage(&statusMessage);
	printf("clicker\n");
	BTextView::MouseDown(point);
}
void EditorTextView::KeyDown(const char* bytes, int32 numBytes)
{
	statusMessage.MakeEmpty();
	statusMessage.AddInt64("clearStatus", 1);
	statusMessenger.SendMessage(&statusMessage);
	printf("keypress\n");
	BTextView::KeyDown(bytes, numBytes);
}
