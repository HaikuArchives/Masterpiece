#include "PlaceHolderView.h"

PlaceHolderView::PlaceHolderView(BRect r)
	:	BView(r, "sumview", B_FOLLOW_ALL, B_WILL_DRAW)
{
	welcomeTitleString = new BStringView(BRect(10, 10, 200, 30), NULL, "Welcome to MasterPiece");
	welcomeLayout = new BGridLayout();
	SetLayout(welcomeLayout);
	welcomeLayout->SetInsets(2, 2, 2, 2);
	welcomeLayout->AddView(welcomeTitleString, 0, 0);
}

PlaceHolderView::~PlaceHolderView(void)
{
}

void PlaceHolderView::AttachedToWindow()
{
	if(Parent())
		SetViewColor(Parent()->ViewColor());
	BView::AttachedToWindow();
}

void PlaceHolderView::Draw(BRect rect)
{
}

