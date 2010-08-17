#include "SummaryView.h"

SummaryView::SummaryView(BRect r)
	:	BView(r, "sumview", B_FOLLOW_ALL, B_WILL_DRAW)
{
	sumViewTitleString = new BStringView(BRect(10, 10, 200, 50), NULL, "MP Summary");
	thoughtTitleString = new BStringView(BRect(10, 50, 200, 70), NULL, "Rough Ideas");
	orderedTitleString = new BStringView(BRect(210, 50, 200, 70), NULL, "Ordered Thoughts");
	AddChild(sumViewTitleString);
	AddChild(thoughtTitleString);
	AddChild(orderedTitleString);
}

SummaryView::~SummaryView(void)
{
}

void SummaryView::Draw(BRect rect)
{
	//rgb_color backColor = {215, 215, 215, 255};
	
	//SetDrawingMode(B_OP_ALPHA);
	//SetHighColor(215, 215, 215);	// background gray app color
	//sumViewTitleString->SetViewColor(backColor);
	//sumViewTitleString->SetHighColor(backColor);
	//sumViewTitleString->FillRect(sumViewTitleString->Bounds());
	//sumViewTitleString->SetHighColor(0, 0, 0);
	//FillRect(Bounds());	// fill view with high color above
	//SetHighColor(0, 0, 0); // set color to black
	//StrokeRect(Bounds());	// set black rectangle around view
	
	// the above code has redrawing issues when resized
}

