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

void OpenMasterView::AttachedToWindow()
{
	if(Parent())
		SetViewColor(Parent()->ViewColor());
	BView::AttachedToWindow();
}

void OpenMasterView::Draw(BRect rect)
{
	/*
	 * need to figure out how to draw better on resize
	*/
	SetHighColor(0, 0, 0);
	StrokeRect(Bounds());
}
