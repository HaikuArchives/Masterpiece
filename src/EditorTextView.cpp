#include "EditorTextView.h"

EditorTextView::EditorTextView(BRect frame, const char* name, BRect textRect, uint32 resizingMode, uint32 flags)
	:	BTextView(frame, name, textRect, resizingMode, flags)
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
	printf("clicker\n");
	BTextView::MouseDown(point);
}
void EditorTextView::KeyDown(const char* bytes, int32 numBytes)
{
	printf("keypress\n");
	BTextView::KeyDown(bytes, numBytes);
}
