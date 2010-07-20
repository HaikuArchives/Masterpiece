#include "SummaryView.h"

SummaryView::SummaryView(BRect r)
	:	BView(r, "sumview", B_FOLLOW_ALL, B_WILL_DRAW)
{
}

SummaryView::~SummaryView(void)
{
}

void SummaryView::Draw(BRect rect)
{
	
	SetDrawingMode(B_OP_ALPHA);
	SetHighColor(215, 215, 215);
	FillRect(Bounds());
	SetHighColor(0, 0, 0);
	StrokeRect(Bounds());
}

