#include "SummaryView.h"

SummaryView::SummaryView(void)
	:	BView(BRect(30, 100, 330, 200), "sumview", B_FOLLOW_ALL, B_WILL_DRAW)
{
}

SummaryView::~SummaryView(void)
{
}

void SummaryView::Draw(BRect rect)
{
	rgb_color backColor = {215, 215, 215, 255};
	
	SetDrawingMode(B_OP_ALPHA);
	SetHighColor(215, 215, 215);
	FillRect(Bounds());
	SetHighColor(0, 0, 0);
	StrokeRect(Bounds());
}

