#include "SummaryView.h"

SummaryView::SummaryView(BRect r)
	:	BView(r, "sumview", B_FOLLOW_ALL, B_WILL_DRAW)
{
	sumViewTitleString = new BStringView(BRect(10, 10, 200, 30), NULL, "MP Summary");
	thoughtTitleString = new BStringView(BRect(10, 50, 200, 70), NULL, "Rough Ideas");
	thoughtCountString = new BStringView(BRect(10, 100, 200, 120), NULL, "X Ideas");
	orderedTitleString = new BStringView(BRect(210, 50, 410, 70), NULL, "Ordered Ideas");
	orderedCountString = new BStringView(BRect(210, 100, 410, 120), NULL, "X Ideas");
	sumLayout = new BGridLayout();
	sumViewTitleString->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetLayout(sumLayout);
	sumLayout->SetInsets(2, 2, 2, 2);
	sumLayout->AddView(sumViewTitleString, 0, 0);
	sumLayout->AddView(thoughtTitleString, 1, 0);
}

SummaryView::~SummaryView(void)
{
}

void SummaryView::AttachedToWindow()
{
	if(Parent())
		SetViewColor(Parent()->ViewColor());
	BView::AttachedToWindow();
}

void SummaryView::Draw(BRect rect)
{
}

