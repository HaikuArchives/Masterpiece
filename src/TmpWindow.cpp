#include "TmpWindow.h"

TmpWindow::TmpWindow(void)
	:	BWindow(BRect(20, 20, 200, 200), "TMP", B_MODAL_WINDOW, B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE)
{
	BRect r(Bounds());
	tmpView = new BView(Bounds(), "tmp", B_FOLLOW_ALL, B_WILL_DRAW);
	rgb_color tmpColor = {215, 215, 215, 255};
	tmpView->SetViewColor(tmpColor);
	AddChild(tmpView);
}
