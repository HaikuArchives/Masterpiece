#ifndef THOUGHTVIEW_H
#define THOUGHTVIEW_H

#include <View.h>
#include <String.h>
#include <StringView.h>
#include <stdio.h>
#include <Box.h>

#include "MultiLineTextControl.h"

class ThoughtView : public BView
{
public:
				ThoughtView(BRect r);	// main view widgets here
				~ThoughtView(void);
				
				
		void	Draw(BRect rect);		// set view draw here
		void	AttachedToWindow();		// set view to get background properties here
		
		BStringView				*sumViewTitleString;
		MultiLineTextControl	*thoughtEntry;
		BBox					*thoughtBox;
		
private:
protected:
};

#endif
