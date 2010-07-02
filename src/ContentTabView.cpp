#include "ContentTabView.h"

ContentTabView::ContentTabView(void)
	:	BTabView(BRect(30, 100, 330, 200), "contabview")
{
	BRect tr = Bounds();
	tr.InsetBy(5,5);
	tr.bottom -= TabHeight();
	tmpTab = new BTab();
	AddTab(new BView(tr, 0, B_FOLLOW_ALL, B_WILL_DRAW), tmpTab);
	tmpTab->SetLabel("Thoughts");
	tmpTab = new BTab();
	AddTab(new BView(tr, 0, B_FOLLOW_ALL, B_WILL_DRAW), tmpTab);
	tmpTab->SetLabel("Images");
}

ContentTabView::~ContentTabView(void)
{
}

void ContentTabView::Draw(BRect rect)
{
	rgb_color backColor = {215, 215, 215, 255};

	SetViewColor(backColor);
}
