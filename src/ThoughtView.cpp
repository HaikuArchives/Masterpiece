// need to move this view to the layout api stuff
#include "ThoughtView.h"

ThoughtView::ThoughtView(BRect r)
	:	BView(r, "thoughtview", B_FOLLOW_ALL, B_WILL_DRAW)
{
	thoughtEntry = new MultiLineTextControl(BRect(0, 0, 400, 250), NULL, NULL, false, "", NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	thoughtString = new BStringView(BRect(0, 0, 10, 10), NULL, "Add New Thought");
	thoughtString->SetAlignment(B_ALIGN_LEFT);
	thoughtString->SetFont(be_bold_font);
	//helpString = new BStringView(BRect(0, 0, 10, 10), NULL, "Markup Help");
	//helpString->SetAlignment(B_ALIGN_RIGHT);
	thoughtGroup = new BGroupLayout(B_HORIZONTAL, 0.0);
	SetLayout(thoughtGroup);
	AddChild(BGridLayoutBuilder()
		.Add(thoughtString, 0, 0)
		.Add(BSpaceLayoutItem::CreateGlue(), 1, 0)
		//.Add(helpString, 2, 0)
		.Add(thoughtEntry, 0, 1, 2, 1)
		.SetInsets(5, 2, 5, 2)
	);
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
