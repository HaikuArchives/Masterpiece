#include "MPBuilder.h"

MPBuilder::MPBuilder(BString windowTitle)
	:	BWindow(BRect(100, 100, 900, 700), windowTitle, B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE)
{
}
void MPBuilder::MessageReceived(BMessage* msg)
{
}
bool MPBuilder::QuitRequested(void)
{
	//this->Show(); // need to figure out how to show mplauncher again...
	//this->Hide(); // if i figure out the mplauncher show, i can hide mpbuilder
	be_app->PostMessage(B_QUIT_REQUESTED); // this kills app
	return true;
}
