#include "PictureView.h"
#include <TranslationUtils.h>
#include <TranslatorFormats.h>

PictureView::PictureView(void)
	:	BView(BRect(0, 0, 100, 100), "picview", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW),
		fBitmapIndex(0)
{
	for(int8 i = 1; i <= 2; i++)
	{
		BBitmap *smiley = BTranslationUtils::GetBitmap(B_PNG_FORMAT,i);
		fBitmaps[i-1] = (smiley && smiley->IsValid()) ? smiley : NULL;
	}
	
	if(fBitmaps[0] && fBitmaps[0]->IsValid())
		ResizeTo(fBitmaps[0]->Bounds().Width(),
				 fBitmaps[0]->Bounds().Height());
}

PictureView::~PictureView(void)
{
}

void PictureView::Draw(BRect rect)
{
	SetDrawingMode(B_OP_ALPHA);
	
	SetHighColor(255, 255, 255);
	
	FillRect(Bounds());
	
	if(fBitmaps[fBitmapIndex])
		DrawBitmap(fBitmaps[fBitmapIndex]);
		
	SetHighColor(0, 0, 0);
	
	StrokeRect(Bounds());
}

void PictureView::MouseUp(BPoint pt)
{
	if(fBitmapIndex == sizeof(*fBitmaps))
		fBitmapIndex = 0;
	else
		fBitmapIndex++;
	
	Invalidate();
}

