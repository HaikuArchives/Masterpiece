#include "ContentTabView.h"

ContentTabView::ContentTabView(void)
	:	BTabView(BRect(100, 20, 900, 600), "contabview", B_WIDTH_AS_USUAL, B_FOLLOW_ALL, B_FULL_UPDATE_ON_RESIZE | B_WILL_DRAW)
{
	BRect tr = Bounds();
	tr.InsetBy(5,5);
	tr.top = tr.top;
	tr.bottom = tr.bottom - TabHeight();
	tmpTab = new BTab();
	imageTab = new BTab();
	AddTab(new BView(tr, 0, B_FOLLOW_ALL, B_WILL_DRAW), tmpTab);
	tmpTab->SetLabel("Thoughts");
	//tmpTab = new BTab();
	AddTab(new BView(tr, 0, B_FOLLOW_ALL, B_WILL_DRAW), imageTab);
	imageTab->SetLabel("Images");
}

ContentTabView::~ContentTabView(void)
{
}

void ContentTabView::Draw(BRect rect)
{
	rgb_color backColor = {215, 215, 215, 255};

	//SetViewColor(backColor);
	//tmpTab = new BTab();
	//AddTab(new BView(Bounds(), 0, B_FOLLOW_ALL, B_WILL_DRAW), tmpTab);
	//AddTab(new BView(Bounds(), 0, B_FOLLOW_ALL, B_WILL_DRAW), imageTab);
	//tmpTab->SetLabel("Thoughts");
}
