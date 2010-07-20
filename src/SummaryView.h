#ifndef SUMMARYVIEW_H
#define SUMMARYVIEW_H

#include <View.h>

class SummaryView : public BView
{
public:
				SummaryView(BRect r);
				~SummaryView(void);
				
				
		void	Draw(BRect rect);
		
private:
protected:
};

#endif
