#include "SaveIdea.h"

SaveIdea::SaveIdea(float mainX, float mainY, int currentID)
	:	BWindow(BRect(0, 0, 358, 60), "Save As", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE)
{
	BRect textFrame(0, 0, 300, 10);
	titleText = new BTextView(textFrame, NULL, textFrame, B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW);
	titleText->MakeResizable(false);
	titleText->SetWordWrap(false);
	titleString = new BStringView(BRect(10, 10, 200, 30), NULL, "Enter Thought Title:");
	saveButton = new BButton(BRect(190, 50, 270, 75), NULL, "Save", new BMessage(SAVE_IDEA));
	cancelButton = new BButton(BRect(190, 50, 270, 75), NULL, "Cancel", new BMessage(CANCEL_SAVE));
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	
	backView->SetLayout(new BGroupLayout(B_HORIZONTAL, 0.0));
	backView->AddChild(BGridLayoutBuilder()
		.Add(titleString, 0, 0, 2, 1)
		.Add(titleText, 2, 0, 3, 1)
		.Add(cancelButton, 3, 1)
		.Add(saveButton, 4, 1)
		.Add(BSpaceLayoutItem::CreateGlue(), 0, 2, 2, 1)
		.SetInsets(2, 5, 2, 2)
	);
	MoveTo(mainX, mainY);
}
void SaveIdea::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case SAVE_IDEA:
			this->Close();
			break;
		case CANCEL_SAVE:
			this->Close();
			break;
		default:
			BWindow::MessageReceived(msg);
			break;
	}
}
