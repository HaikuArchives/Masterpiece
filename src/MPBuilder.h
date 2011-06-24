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
#include <TextView.h>

#include "ErrorAlert.h"
#include "BuilderMenu.h"
#include "SaveIdea.h"
#include "EditIdeaName.h"
#include "CommonFunctions.h"
#include "MPEditor.h"
#include "IdeaStringItem.h"

#define SHOW_LAUNCHER			'lnch' // show mp launcher window
#define MP_UPDATE_TITLE			'uttl' // update title
#define	MOVE_RIGHT				'mvrt' // move right
#define MOVE_LEFT				'mvlt' // move left
#define MOVE_TOP				'mvtp' // move top
#define	MOVE_UP					'mvup' // move up
#define MOVE_DOWN				'mvdn' // move down
#define MOVE_BOTTOM				'mvbt' // move bottom
#define DELETE_BUILDER_THT		'dtht' // delete thought
#define DISPLAY_AVAIL_TEXT		'davt' // display available text for selected item
#define DISPLAY_ORDER_TEXT		'dort' // display ordered text for selected item
#define ORDER_THOUGHT_EDITOR	'oted' // open ordered thought editor
#define AVAIL_THOUGHT_EDITOR	'ated' // open available thought editor

class MPBuilder : public BWindow
{
public:
						MPBuilder(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID);
		void			MessageReceived(BMessage* msg);
		bool			QuitRequested(void);
		void			PopulateBuilderListViews(void);
		void			ReorderOrderedListView(void);
		void			ModifyOrderedItems(int curOrderNumber, int newOrderNumber);
private:
		BView*			backView;
		BGroupLayout*	mainGroup;
		BMessage		launcherMessage;
		BMessenger		launcherMessenger;
		BListView*		availableThoughtListView;
		BListView*		orderedThoughtListView;
		BTextView*		builderTextView;
		BButton*		rightButton;
		BButton*		leftButton;
		BButton*		topButton;
		BButton*		upButton;
		BButton*		downButton;
		BButton*		bottomButton;
		BButton*		deleteButton;
		MPEditor*		tmpEditor;
		ErrorAlert*		eAlert;
		SaveIdea*		saveIdea;
		EditIdeaName*	editIdeaName;
		sqlite3*		mpdb;
		sqlite3_stmt*	ideaStatement;
		sqlite3_stmt*	reorderStatement;
		char*			sqlErrMsg;
		BString			updateTitle;
		BString			tmpString;
		int64			showLauncher;
		int				sqlValue;
		int				currentideaID;
		int				selected;
		int				bindValue;
		float			xPos;
		float			yPos;
		SqlObject*		sqlObject;
		SqlObject*		sqlObject2;
};

#endif
