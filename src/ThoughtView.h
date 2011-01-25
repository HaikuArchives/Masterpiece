#ifndef THOUGHTVIEW_H
#define THOUGHTVIEW_H

#include <View.h>
#include <String.h>
#include <stdio.h>
#include <GridLayout.h>
#include <GroupLayout.h>
#include <StringView.h>

#include "MultiLineTextControl.h"

class ThoughtView : public BView
{
public:
								ThoughtView(BRect r);	// main view widgets here
								~ThoughtView(void);
				
				
		void					Draw(BRect rect);		// set view draw here
		void					AttachedToWindow();		// set view to get background properties here
		
		BGridLayout*			thoughtGrid;
		MultiLineTextControl*	thoughtEntry;
		BStringView*			thoughtString;
		
private:
protected:
};

#endif
