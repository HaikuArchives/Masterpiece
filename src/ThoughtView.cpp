// need to move this view to the layout api stuff
#include "ThoughtView.h"

ThoughtView::ThoughtView(BRect r)
	:	BView(r, "thoughtview", B_FOLLOW_ALL, B_WILL_DRAW)
{
	thoughtBox = new BBox(r, NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW, B_FANCY_BORDER);
	thoughtBox->SetLabel("Thought View");
	thoughtEntry = new MultiLineTextControl(BRect(10, 50, 400, 250), NULL, NULL, false, "", NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	AddChild(thoughtEntry);
	AddChild(thoughtBox);
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

