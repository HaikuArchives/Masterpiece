#include "OpenMasterView.h"

OpenMasterView::OpenMasterView(void)
	:	BView(BRect(30, 100, 730, 500), "openview", B_FOLLOW_ALL, B_WILL_DRAW)
{
	openTitleString = new BStringView(BRect(10, 10, 175, 20), NULL, "Select a MasterPiece:");
	openListView = new BListView(BRect(180, 10, 660, 360), "mpList", B_SINGLE_SELECTION_LIST, B_FOLLOW_NONE, B_WILL_DRAW);
	openButton = new BButton(BRect(600, 370, 680, 395), NULL, "Open", new BMessage(OPEN_EXISTING_COURSE), B_FOLLOW_NONE, B_WILL_DRAW);
	cancelButton = new BButton(BRect(510, 370, 590, 395), NULL, "Cancel", new BMessage(CANCEL_OPEN_COURSE), B_FOLLOW_NONE, B_WILL_DRAW);
	AddChild(openTitleString);
	AddChild(new BScrollView("scroll_mpList", openListView, B_FOLLOW_NONE, 0, false, true, B_FANCY_BORDER));
	AddChild(openButton);
	AddChild(cancelButton);
}

OpenMasterView::~OpenMasterView(void)
{
}

void OpenMasterView::Draw(BRect rect)
{
	rgb_color backColor = {215, 215, 215, 255};

	SetDrawingMode(B_OP_ALPHA);
	SetHighColor(215, 215, 215);
	openTitleString->SetViewColor(backColor); // group of commands set's the color back to gray like the view.
	openTitleString->SetHighColor(backColor);
	openTitleString->FillRect(openTitleString->Bounds());
	openTitleString->SetHighColor(0, 0, 0);
	openTitleString->SetText("Select a MasterPiece:");
	openButton->SetLowColor(backColor);
	cancelButton->SetLowColor(backColor);
	FillRect(Bounds());
	SetHighColor(0, 0, 0);
	StrokeRect(Bounds());
}
