#ifndef MPPREVIEW_H
#define MPPREVIEW_H

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
#include "CommonFunctions.h"

class MPPreview : public BWindow
{
public:
						MPPreview(int ideaID);
		void			IdeaParser(BString inputText, BTextView* displayTextView);
		void			MessageReceived(BMessage* msg);
		bool			QuitRequested(void);
private:
		BView*			backView;
		BTextView*		previewTextView;
		ErrorAlert*		eAlert;
		sqlite3*		mpdb;
		sqlite3_stmt*	ideaStatement;
		char*			sqlErrMsg;
		BString			tmpString;
		BString			rawText;
		BString			parsedText;
		BFont			parseFont;
		uint32			sameProperties;
		int				sqlValue;
		int				currentideaID;
		int32			startPos;
		int32			endPos;
		float			xPos;
		float			yPos;
		SqlObject*		sqlObject;
};

#endif
