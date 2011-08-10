#include "KeyListView.h"

KeyListView::KeyListView(BRect frame)
	:	BListView(frame, NULL, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE)
{
}
void KeyListView::KeyDown(const char *bytes, int32 numBytes)
{
	printf("a key is pressed\n");
	BMessage *msg = Window()->CurrentMessage();
	if(numBytes == 1)
	{
		switch(bytes[0])
		{
			case B_FUNCTION_KEY:
				printf("Function Key Pressed\n");
				if(msg)
				{
					int32 key;
					msg->FindInt32("key", &key);
					switch(key)
					{
						case B_F1_KEY:
							printf("F1 Pressed\n");
							if(modifiers() | B_COMMAND_KEY)
							{
								printf("modifier command also pressed\n");
							}
							break;
					}
				}
				break;
			
			case B_ESCAPE:
				printf("escape key pressed\n");
				if(modifiers() | B_COMMAND_KEY)
				{
					printf("modifier command also pressed\n");
				}
				break;
			
			case 'u':
				printf("u is pressed\n");
				if(modifiers() | B_COMMAND_KEY)
				{
					printf("modifier command also pressed\n");
				}
				else if(modifiers() | B_OPTION_KEY)
				{
					printf("alt ?\n");
				}
				break;
				
			default:
				BListView::KeyDown(bytes, numBytes);
				break;
		}
	}
}