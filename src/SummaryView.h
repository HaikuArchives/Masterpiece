#ifndef SUMMARYVIEW_H
#define SUMMARYVIEW_H

#include <View.h>
#include <String.h>
#include <StringView.h>
#include <stdio.h>

class SummaryView : public BView
{
public:
				SummaryView(BRect r);	// main view widgets here
				~SummaryView(void);
				
				
		void	Draw(BRect rect);		// set view draw here
		
		BStringView	*sumViewTitleString;
		BString title;
		
private:
protected:
};

#endif
