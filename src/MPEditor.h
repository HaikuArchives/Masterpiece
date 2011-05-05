#ifndef MPEDITOR_H
#define MPEDITOR_H

#include <Window.h>
#include <View.h>
#include <sqlite3.h>
#include <Application.h>
#include <String.h>
#include <ScrollView.h>
#include <Messenger.h>
#include <Message.h>
#include <stdio.h>
#include <GroupLayout.h>
#include <SpaceLayoutItem.h>
#include <GridLayoutBuilder.h>
#include <TextView.h>
#include <ScrollView.h>

#include "ErrorAlert.h"
#include "EditorMenu.h"
#include "SaveIdea.h"
#include "EditIdeaName.h"
#include "CommonFunctions.h"

#define SHOW_LAUNCHER		'lnch' // show mp launcher window
#define UPDATE_TITLE		'uttl' // update title...

class MPEditor : public BWindow
{
public:
						MPEditor(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID);
		void			MessageReceived(BMessage* msg);
		bool			QuitRequested(void);
private:
		BView*			backView;
		BMessage		launcherMessage;
		BMessenger		launcherMessenger;
		BTextView*		editorTextView;
		MPEditor*		tmpEditor;
		ErrorAlert*		eAlert;
		SaveIdea*		saveIdea;
		EditIdeaName*	editIdeaName;
		sqlite3*		mpdb;
		sqlite3_stmt*	ideaStatement;
		char*			sqlErrMsg;
		BString			tmpString;
		BString			updateTitle;
		int				sqlValue;
		int				currentideaID;
		float			xPos;
		float			yPos;
		SqlObject*		sqlObject;
};


#endif
