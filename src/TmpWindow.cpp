#include "TmpWindow.h"

TmpWindow::TmpWindow(void)
	:	BWindow(BRect(20, 20, 200, 200), "TMP", B_MODAL_WINDOW, B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE)
{
	BRect r(Bounds());
	tmpView = new BView(Bounds(), "tmp", B_FOLLOW_ALL, B_WILL_DRAW);
	rgb_color tmpColor = {215, 215, 215, 255};
	tmpView->SetViewColor(tmpColor);
	AddChild(tmpView);
	tmpButton = new BButton(BRect(10, 10, 90, 35), NULL, "Close", new BMessage(CLOSE_TMP), B_FOLLOW_NONE, B_WILL_DRAW);
	tmpView->AddChild(tmpButton);
}

void TmpWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case CLOSE_TMP:
		// do soemthing
		this->Close();
		break;
		
		default:
		BWindow::MessageReceived(msg);
		break;
	}
}
