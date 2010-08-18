#include "NewMasterView.h"

NewMasterView::NewMasterView(void)
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

void NewMasterView::AttachedToWindow()
{
	if(Parent())
		SetViewColor(Parent()->ViewColor());
	BView::AttachedToWindow();
}

// set view and widgets background and foreground colors.
void NewMasterView::Draw(BRect rect)
{
	SetHighColor(0, 0, 0);
	StrokeRect(Bounds());
}
