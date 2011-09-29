#ifndef MPEDITOR_H
#define MPEDITOR_H

#include "pyembed.h"

#include <Window.h>
#include <View.h>
#include <sqlite3.h>
#include <Application.h>
#include <String.h>
#include <string.h>
#include <ScrollView.h>
#include <Messenger.h>
#include <Message.h>
#include <stdio.h>
#include <stdlib.h>
#include <GroupLayout.h>
#include <SpaceLayoutItem.h>
#include <GridLayoutBuilder.h>
#include <TextView.h>
#include <ScrollView.h>
#include <File.h>
#include <Entry.h>
#include <malloc.h>

#include "ErrorAlert.h"
#include "EditorMenu.h"
#include "SaveIdea.h"
#include "EditIdeaName.h"
#include "HelperWindows.h"
#include "CommonFunctions.h"
#include "PublishFilePanel.h"
#include "MarkupWindow.h"

#define SHOW_LAUNCHER		'lnch' // show mp launcher window
#define UPDATE_TITLE		'uttl' // update title...

class MPEditor : public BWindow
{
public:
							MPEditor(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID);
		void				MessageReceived(BMessage* msg);
		bool				QuitRequested(void);
private:
		BView*				backView;
		BMessage			launcherMessage;
		BMessenger			launcherMessenger;
		BTextView*			editorTextView;
		MPEditor*			tmpEditor;
		ErrorAlert*			eAlert;
		SaveIdea*			saveIdea;
		EditIdeaName*		editIdeaName;
		HelperWindows*		helperWindow;
		MarkupWindow*		markupWindow;
		sqlite3*			mpdb;
		sqlite3_stmt*		ideaStatement;
		char*				sqlErrMsg;
		BString				tmpString;
		BString				updateTitle;
		BString				tmpText;
		int					sqlValue;
		int					currentideaID;
		float				xPos;
		float				yPos;
		SqlObject*			sqlObject;
		BEntry				entry;
		BPath				path;
		entry_ref			ref;
		const char*			name;
		PublishFilePanel*	pubEditorPanel;
};


#endif
