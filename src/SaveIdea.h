#ifndef SAVEIDEA_H
#define SAVEIDEA_H

#include "CommonFunctions.h"

#define SAVE_IDEA		'save' // save button
#define	CANCEL_SAVE		'can'  // cancel save
#define UPDATE_TITLE	'uttl' // update title...
#define END_SAVE_IDEA	'esid' // tab from editor to next button

class SaveIdea : public BWindow
{
public:
				SaveIdea(const BMessage &msg, const BMessenger &msgr, float mainX, float mainY, int currentID);
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
		int				sqlValue;
		int				currentideaID;
		SqlObject*		sqlObject;
};

#endif
