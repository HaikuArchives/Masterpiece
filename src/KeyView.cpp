#include "KeyView.h"

KeyView::KeyView(BRect frame)
	:	BView(frame, NULL, B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
}
void KeyView::KeyDown(const char *bytes, int32 numBytes)
{
	if(bytes[0] == B_FUNCTION_KEY)
	{
		BMessage *msg = Window()->CurrentMessage();
		if(msg)
		{
			int32 key;
			msg->FindInt32("key", &key);
			switch(key)
			{
				case B_F1_KEY:
					printf("F1 Pressed\n");
					break;
			}
		}
	}
}