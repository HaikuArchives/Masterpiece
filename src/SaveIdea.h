#ifndef SAVEIDEA_H
#define SAVEIDEA_H

#include <Window.h>
#include <View.h>
#include <Button.h>
#include <GroupLayout.h>
#include <GridLayoutBuilder.h>
#include <SpaceLayoutItem.h>
#include <TextView.h>
#include <stdio.h>
#include <Message.h>
#include <Messenger.h>
#include <String.h>
#include <StringView.h>
#include <sqlite3.h>

#include "ErrorAlert.h"
#include "CommonFunctions.h"

#define SAVE_IDEA	'save' // save button
#define	CANCEL_SAVE	'can'  // cancel save

class SaveIdea : public BWindow
{
public:
				SaveIdea(float mainX, float mainY, int currentID);
		void	MessageReceived(BMessage* msg);
		
private:

		ErrorAlert*		eAlert;
		BStringView*	titleString;
		BTextView*		titleText;
		BButton*		saveButton;
		BButton*		cancelButton;
		BView*			backView;
		sqlite3*		mpdb;
		sqlite3_stmt*	ideaStatement;
		char*			sqlErrMsg;	
		int				sqlValue;
		int				currentideaID;
};

#endif
