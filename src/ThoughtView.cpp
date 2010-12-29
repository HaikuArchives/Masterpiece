// need to move this view to the layout api stuff
#include "ThoughtView.h"

ThoughtView::ThoughtView(BRect r)
	:	BView(r, "thoughtview", B_FOLLOW_ALL, B_WILL_DRAW)
{
	thoughtEntry = new MultiLineTextControl(BRect(0, 0, 400, 250), NULL, NULL, false, "", NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	thoughtString = new BStringView(BRect(0, 0, 100, 50), NULL, "Thought Placement Test");
	thoughtBox = new BBox(BRect(0, 0, 10, 10), NULL, B_FOLLOW_ALL, B_WILL_DRAW, B_FANCY_BORDER);
	thoughtString->AddChild(thoughtBox);
	thoughtBox->SetLabel("Add New Thought");
	BGridLayout* thoughtGrid = new BGridLayout();
	SetLayout(thoughtGrid);
	thoughtGrid->SetInsets(0, 0, 0, 0);
	thoughtGrid->AddView(thoughtEntry, 0, 0);
	thoughtGrid->AddView(thoughtString, 0, 1);
	//thoughtGrid->AddView(thoughtBox, 1, 1);
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
