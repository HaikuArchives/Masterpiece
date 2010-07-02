#include "ContentTabView.h"

ContentTabView::ContentTabView(void)
	:	BView(BRect(30, 100, 330, 200), "newview", B_FOLLOW_ALL, B_WILL_DRAW)
{
	BRect viewFrame(110, 10, 280, 30);
	BRect textFrame;
	textFrame.left = TEXT_INSET;
	textFrame.right = viewFrame.right - viewFrame.left - TEXT_INSET;
	textFrame.top = TEXT_INSET;
	textFrame.bottom = viewFrame.bottom - viewFrame.top - TEXT_INSET;
	titleString = new BStringView(BRect(10, 10, 100, 20), NULL, "Enter Title:");
	titleText = new BTextView(viewFrame, "textTitle", textFrame, B_FOLLOW_NONE, B_WILL_DRAW);
	newButton = new BButton(BRect(190, 50, 270, 75), NULL, "Add", new BMessage(ADD_NEW_COURSE), B_FOLLOW_NONE, B_WILL_DRAW);
	cancelButton = new BButton(BRect(100, 50, 180, 75), NULL, "Cancel", new BMessage(CANCEL_NEW_COURSE), B_FOLLOW_NONE, B_WILL_DRAW);
	AddChild(titleString);
	AddChild(newButton);
	AddChild(cancelButton);
	AddChild(titleText);
	viewFrame.InsetBy(-2.0, -2.0);
}

NewMasterView::~NewMasterView(void)
{
}

void NewMasterView::Draw(BRect rect)
{
	rgb_color backColor = {215, 215, 215, 255};

	SetDrawingMode(B_OP_ALPHA);
	SetHighColor(215, 215, 215);
	titleString->SetViewColor(backColor);
	newButton->SetLowColor(backColor);
	cancelButton->SetLowColor(backColor);
	FillRect(Bounds());
	SetHighColor(0, 0, 0);
	StrokeRect(Bounds());
}
