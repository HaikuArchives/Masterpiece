#include "KeyView.h"

KeyView::KeyView(BRect frame, const char* name)
	:	BView(frame, name, B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	MakeFocus(true);
}
void KeyView::KeyDown(const char *bytes, int32 numBytes)
{
	//printf("a key is pressed\n");
	BMessage *msg = Window()->CurrentMessage();
	if(numBytes == 1)
	{
		switch(bytes[0])
		{
			case B_FUNCTION_KEY:
				//printf("Function Key Pressed\n");
				if(msg)
				{
					int32 key;
					msg->FindInt32("key", &key);
					switch(key)
					{
						case B_F1_KEY:
							//printf("F1 Pressed\n");
							break;
					}
				}
				break;
			
			case B_ESCAPE:
				//printf("escape key pressed\n");
				break;
				
			default:
				BView::KeyDown(bytes, numBytes);
				break;
		}
	}
}