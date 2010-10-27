#include "NewWindow.h"

NewWindow::NewWindow(void)
	:	BWindow(BRect(20, 20, 200, 200), "newwin", B_MODAL_WINDOW, B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE)
{
	rgb_color myColor = {215, 215, 215, 255};
	BRect viewFrame(110, 10, 280, 30);
	BRect textFrame;
	textFrame.left = TEXT_INSET;
	textFrame.right = viewFrame.right - viewFrame.left - TEXT_INSET;
	textFrame.top = TEXT_INSET;
	textFrame.bottom = viewFrame.bottom - viewFrame.top - TEXT_INSET;
	titleText = new BTextView(viewFrame, "textTitle", textFrame, B_FOLLOW_NONE, B_WILL_DRAW);
	newButton = new BButton(BRect(190, 50, 270, 75), NULL, "Add", new BMessage(ADD_NEW_MP), B_FOLLOW_NONE, B_WILL_DRAW);
	cancelButton = new BButton(BRect(100, 50, 180, 75), NULL, "Cancel", new BMessage(CANCEL_NEW_MP), B_FOLLOW_NONE, B_WILL_DRAW);
	BGridLayout* mainGrid = new BGridLayout();
	SetLayout(mainGrid);
	titleString = new BStringView(BRect(0, 0, 10, 10), NULL, "Enter Title:");
	mainGrid->AddView(titleString, 0, 0);
	mainGrid->AddView(titleText, 1, 0);
	mainGrid->AddView(cancelButton, 0, 1);
	mainGrid->AddView(newButton, 1, 1);
}
void NewWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case CANCEL_NEW_MP:
			if(!this->IsHidden())
			{
				this->titleText->SetText("");
				this->Hide();
			}
			break;
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}
