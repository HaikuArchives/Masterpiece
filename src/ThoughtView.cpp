#include "ThoughtView.h"

ThoughtView::ThoughtView(BRect r)
	:	BView(r, "thoughtview", B_FOLLOW_ALL, B_WILL_DRAW)
{
	sumViewTitleString = new BStringView(BRect(10, 10, 200, 30), NULL, "Thought View");
	thoughtEntry = new MultiLineTextControl(BRect(10, 50, 400, 250), NULL, NULL, false, "", NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	AddChild(sumViewTitleString);
	AddChild(thoughtEntry);
}

ThoughtView::~ThoughtView(void)
{
}

void ThoughtView::AttachedToWindow()
{
	if(Parent())
		SetViewColor(Parent()->ViewColor());
	BView::AttachedToWindow();
}

void ThoughtView::Draw(BRect rect)
{
}

