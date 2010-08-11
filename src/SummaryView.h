#ifndef SUMMARYVIEW_H
#define SUMMARYVIEW_H

#include <View.h>

class SummaryView : public BView
{
public:
				SummaryView(BRect r);	// main view widgets here
				~SummaryView(void);
				
				
		void	Draw(BRect rect);		// set view draw here
		
private:
protected:
};

#endif
