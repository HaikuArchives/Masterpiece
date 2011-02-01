#include "MPBuilder.h"

MPBuilder::MPBuilder(void)
	:	BWindow(BRect(100, 100, 900, 700), "MasterPiece Builder - untitled", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE)
{
}
void MPBuilder::MessageReceived(BMessage* msg)
{
}
bool MPBuilder::QuitRequested(void)
{
	return true;
}
