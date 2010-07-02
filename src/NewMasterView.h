#ifndef NEWMASTERVIEW_H
#define NEWMASTERVIEW_H

#include <Message.h>
#include <Bitmap.h>
#include <String.h>
#include <View.h>
#include <StringView.h>
#include <Button.h>
#include <TextView.h>

#define ADD_NEW_COURSE		'add'
#define CANCEL_NEW_COURSE	'cnc'
#define TEXT_INSET 3.0

class NewMasterView : public BView
{
public:
						NewMasterView(void);
						~NewMasterView(void);
						
		void			Draw(BRect rect);
		
		BTextView		*titleText;
		BStringView		*titleString;
		BButton			*newButton;
		BButton			*cancelButton;
		
private:

protected:
};

#endif
