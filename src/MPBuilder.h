#ifndef MPBUILDER_H
#define MPBUILDER_H

#include <Window.h>
#include <View.h>
#include <sqlite3.h>
#include <Application.h>
#include <String.h>
#include <GroupLayout.h>
#include <GridLayoutBuilder.h>
#include <SpaceLayoutItem.h>
#include <ListView.h>
#include <ScrollView.h>
#include <StringView.h>
#include <Messenger.h>
#include <Message.h>
#include <stdio.h>

#include "ErrorAlert.h"
#include "BuilderMenu.h"
#include "SaveIdea.h"
#include "EditIdeaName.h"
#include "CommonFunctions.h"
#include "MPEditor.h"

#define SHOW_LAUNCHER	'lnch' // show mp launcher window
#define UPDATE_TITLE	'uttl' // update title
#define	MOVE_RIGHT		'mvrt' // move right
#define MOVE_LEFT		'mvlt' // move left
#define MOVE_TOP		'mvtp' // move top
#define	MOVE_UP			'mvup' // move up
#define MOVE_DOWN		'mvdn' // move down
#define MOVE_BOTTOM		'mvbt' // move bottom

class MPBuilder : public BWindow
{
public:
						MPBuilder(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID);
		void			MessageReceived(BMessage* msg);
		bool			QuitRequested(void);
private:
		BView*			backView;
		BGroupLayout*	mainGroup;
		BMessage		launcherMessage;
		BMessenger		launcherMessenger;
		BListView*		availableThoughtListView;
		BListView*		orderedThoughtListView;
		BButton*		rightButton;
		BButton*		leftButton;
		BButton*		topButton;
		BButton*		upButton;
		BButton*		downButton;
		BButton*		bottomButton;
		MPEditor*		tmpEditor;
		ErrorAlert*		eAlert;
		SaveIdea*		saveIdea;
		EditIdeaName*	editIdeaName;
		sqlite3*		mpdb;
		sqlite3_stmt*	ideaStatement;
		char*			sqlErrMsg;
		BString			updateTitle;
		BString			tmpString;
		int				sqlValue;
		int				currentideaID;
		float			xPos;
		float			yPos;
};

#endif
