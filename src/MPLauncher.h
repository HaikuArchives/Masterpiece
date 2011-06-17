#ifndef MPLAUNCHER_H
#define MPLAUNCHER_H

#include <Window.h>
#include <View.h>
#include <sqlite3.h>
#include <GroupLayout.h>
#include <SpaceLayoutItem.h>
#include <GridLayoutBuilder.h>
#include <Button.h>
#include <ListView.h>
#include <ScrollView.h>
#include <StringView.h>
#include <Message.h>
#include <Messenger.h>
#include <Application.h>
#include <String.h>
#include <Path.h>
#include <Roster.h>
#include <stdio.h>
#include <stdlib.h>

#include "ErrorAlert.h"
#include "MPBuilder.h"
#include "MPEditor.h"
#include "CommonFunctions.h"
#include "IdeaStringItem.h"

#define CREATE_NEW_MP		'cnmp'
#define	CREATE_NEW_THT		'cntt'
#define	OPEN_EXISTING_MP	'oemp'
#define	OPEN_EXISTING_THT	'oett'
#define DELETE_THT			'delt'
#define DELETE_MP			'delm'
#define SHOW_LAUNCHER		'lnch' // show mp launcher window

class MPLauncher : public BWindow
{
public:
						MPLauncher(void);
		void			MessageReceived(BMessage* msg);
		bool			QuitRequested(void);
		void			PopulateLauncherListViews(void);
		
private:
		BView*			backView;
		BGroupLayout*	mainGroup;
		BButton*		newMasterpieceButton;
		BButton*		newThoughtButton;
		BButton*		delThoughtButton;
		BButton*		delMasterpieceButton;
		BListView*		openThoughtListView;
		BListView*		openMasterpieceListView;
		BStringView*	thoughtStringView;
		BStringView*	masterpieceStringView;
		BStringView*	openMasterpieceStringView;
		BStringView*	openThoughtStringView;
		sqlite3*		mpdb;
		sqlite3_stmt*	ideaStatement;
		ErrorAlert*		eAlert;
		char*			sqlErrMsg;
		int				sqlValue;
		int				selected;
		int64			showLauncher;
		BString			tmpString;
		
		MPBuilder*		mpBuilder;
		MPEditor*		mpEditor;
		SqlObject*		sqlObject;
};

#endif
