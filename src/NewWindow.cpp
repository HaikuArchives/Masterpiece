#include "NewWindow.h"

NewWindow::NewWindow(float mainX, float mainY)
	:	BWindow(BRect(20, 20, 200, 85), "Enter Title", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE)
{
	rgb_color myColor = {215, 215, 215, 255};
	BRect viewFrame(10, 10, 160, 30);
	BRect textFrame;
	textFrame.left = TEXT_INSET;
	textFrame.right = viewFrame.right - viewFrame.left - TEXT_INSET;
	textFrame.top = TEXT_INSET;
	textFrame.bottom = viewFrame.bottom - viewFrame.top - TEXT_INSET;
	titleText = new BTextView(viewFrame, "textTitle", textFrame, false, B_WILL_DRAW);
	titleText->SetWordWrap(false);
	titleText->SetViewColor(myColor);
	newButton = new BButton(BRect(190, 50, 270, 75), NULL, "Add", new BMessage(ADD_NEW_MP), B_FOLLOW_NONE, B_WILL_DRAW);
	cancelButton = new BButton(BRect(100, 50, 180, 75), NULL, "Cancel", new BMessage(CANCEL_NEW_MP), B_FOLLOW_NONE, B_WILL_DRAW);
	BGridLayout* mainGrid = new BGridLayout();
	SetLayout(mainGrid);
	mainGrid->SetInsets(2, 2, 2, 2);
	mainGrid->AddView(titleText, 0, 0, 2, 1);
	mainGrid->AddView(cancelButton, 0, 1);
	mainGrid->AddView(newButton, 1, 1);
	viewFrame.InsetBy(-2.0, -2.0);
	MoveTo(mainX, mainY);
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
void NewWindow::Draw(BRect rect)
{
}
