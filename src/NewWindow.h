#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <Window.h>
#include <View.h>
#include <Button.h>
#include <GridLayout.h>
#include <StringView.h>
#include <TextView.h>
#include <sqlite3.h>

#include "DeepBevelView.h"
#include "ErrorAlert.h"

#define ADD_NEW_MP		'add' // add new mp button
#define CANCEL_NEW_MP	'can' // cancel message
#define TEXT_INSET 3.0

class NewWindow : public BWindow
{
public:
				NewWindow(float mainX, float mainY);
		void	MessageReceived(BMessage *msg);
		void	Draw(BRect rect);
private:
		BTextView	*titleText;
		BStringView *titleString;
		BButton		*newButton;
		BButton		*cancelButton;
		DeepBevelView *bevelView;
		ErrorAlert	*eAlert;
		int			returnValue;
		sqlite3		*mpdb;
		int			sqlValue;
		char		*sqlErrMsg;
		char		**selectResult;
		int			nrow, ncol;
};


#endif
