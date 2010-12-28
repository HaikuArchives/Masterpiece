// need to move this view to the layout api stuff
#include "ThoughtView.h"

ThoughtView::ThoughtView(BRect r)
	:	BView(r, "thoughtview", B_FOLLOW_ALL, B_WILL_DRAW)
{
	thoughtEntry = new MultiLineTextControl(BRect(0, 0, 400, 250), NULL, NULL, false, "", NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	//BGroupLayout* thoughtGrid = new BGroupLayout(B_VERTICAL, 0);
	thoughtString = new BStringView(BRect(0, 0, 100, 50), NULL, "Thought Placement Test");
	BGridLayout* thoughtGrid = new BGridLayout();
	SetLayout(thoughtGrid);
	thoughtGrid->SetInsets(0, 0, 0, 0);
	thoughtGrid->AddView(thoughtEntry);
	thoughtGrid->AddView(thoughtString);
	//AddChild(thoughtEntry);
	//AddChild(thoughtString);
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

