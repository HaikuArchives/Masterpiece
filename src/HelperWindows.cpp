#include "HelperWindows.h"

HelperWindows::HelperWindows(BRect frame, const char* title)
	:	BWindow(frame, title, B_TITLED_WINDOW, B_NOT_H_RESIZABLE | B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE)
{
	AddShortcut('q', B_COMMAND_KEY, new BMessage(B_QUIT_REQUESTED));
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	backView->SetHighColor((rgb_color){0, 0, 0, 255});
	AddChild(backView);
}
void HelperWindows::AddText(BRect tframe, const char* hname, const char* helptext)
{
	backView->AddChild(new BStringView(tframe, hname, helptext));
}
bool HelperWindows::QuitRequested(void)
{
	return true;
}
