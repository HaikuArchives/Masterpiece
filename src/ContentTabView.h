#ifndef CONTENTTABVIEW_H
#define CONTENTTABVIEW_H

//#include <Message.h>
//#include <Bitmap.h>
//#include <String.h>
//#include <View.h>
//#include <StringView.h>
//#include <Button.h>
//#include <TextView.h>
#include <TabView.h>
/*
#define ADD_NEW_COURSE		'add'
#define CANCEL_NEW_COURSE	'cnc'
#define TEXT_INSET 3.0
*/
class ContentTabView : public BTabView
{
public:
						ContentTabView(void);
						~ContentTabView(void);
						
		void			Draw(BRect rect);
		
		BTab			*tmpTab;
		/*
		BTextView		*titleText;
		BStringView		*titleString;
		BButton			*newButton;
		BButton			*cancelButton;
		*/
		
private:

protected:
};

#endif
