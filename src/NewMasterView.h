#ifndef NEWMASTERVIEW_H
#define NEWMASTERVIEW_H

//#include <Message.h>
#include <Bitmap.h>
#include <String.h>
#include <View.h>
#include <StringView.h>
#include <Button.h>
#include <TextView.h>

#define ADD_NEW_COURSE		'add'	// add new mp button message
#define CANCEL_NEW_COURSE	'cnc'	// cancel message
#define TEXT_INSET 3.0

class NewMasterView : public BView
{
public:
						NewMasterView(void);
						~NewMasterView(void);
						
		void			Draw(BRect rect);
		
		BTextView		*titleText;		// New MasterPiece InputTextBox
		BStringView		*titleString;	// Label
		BButton			*newButton;		// Add Button
		BButton			*cancelButton;	// Cancel Button
		
private:

protected:
};

#endif
