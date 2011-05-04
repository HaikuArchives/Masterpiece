#ifndef EDITIDEANAME_H
#define EDITIDEANAME_H

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

#define UPDATE_IDEA_NAME	'save' // save button
#define	CANCEL_IDEA_NAME	'can'  // cancel save
#define UPDATE_TITLE		'uttl' // update title...

class EditIdeaName : public BWindow
{
public:
				EditIdeaName(const BMessage &msg, const BMessenger &msgr, float mainX, float mainY, int currentID);
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
		BMessage		updatetitleMessage;
		BMessenger		updatetitleMessenger;
		BString			tmpString;
		int				sqlValue;
		int				currentideaID;
		SqlObject*		sqlObject;
};

#endif
