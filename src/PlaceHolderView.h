#ifndef PLACEHOLDERVIEW_H
#define PLACEHOLDERVIEW_H

#include <View.h>
#include <StringView.h>
#include <GridLayout.h>

class PlaceHolderView : public BView
{
public:
				PlaceHolderView(BRect r);	// main view widgets here
				~PlaceHolderView(void);
				
				
		void	Draw(BRect rect);		// set view draw here
		void	AttachedToWindow();		// set view to get background properties here
		
		BStringView* welcomeTitleString;
		BGridLayout* welcomeLayout;
		
private:
protected:
};

#endif
