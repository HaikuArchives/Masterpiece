#ifndef EDITIDEANAME_H
#define EDITIDEANAME_H

#include "CommonFunctions.h"

#define UPDATE_IDEA_NAME	'save' // save button
#define	CANCEL_IDEA_NAME	'can'  // cancel save
#define UPDATE_TITLE		'uttl' // update title...
#define END_EDIT_IDEA		'eeid' // tab from editor to next button

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
