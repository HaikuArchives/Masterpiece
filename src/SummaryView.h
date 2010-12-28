#ifndef SUMMARYVIEW_H
#define SUMMARYVIEW_H

#include <View.h>
#include <String.h>
#include <StringView.h>
#include <stdio.h>
#include <GridLayout.h>

class SummaryView : public BView
{
public:
				SummaryView(BRect r);	// main view widgets here
				~SummaryView(void);
				
				
		void	Draw(BRect rect);		// set view draw here
		void	AttachedToWindow();		// set view to get background properties here
		
		BStringView* sumViewTitleString;
		BStringView* thoughtTitleString;
		BStringView* orderedTitleString;
		BStringView* thoughtCountString;
		BStringView* orderedCountString;
		BGridLayout* sumLayout;
		
private:
protected:
};

#endif
