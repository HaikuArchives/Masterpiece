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

#define CREATE_NEW_MP		'cnmp'
#define	CREATE_NEW_THT		'cntt'
#define	OPEN_EXISTING_MP	'oemp'
#define	OPEN_EXISTING_THT	'oett'
#define SHOW_LAUNCHER		'lnch' // show mp launcher window

class MPLauncher : public BWindow
{
public:
						MPLauncher(void);
		void			MessageReceived(BMessage* msg);
		bool			QuitRequested(void);
		void			OpenMasterpieceDB(void);
		
private:
		BView*			backView;
		BGroupLayout*	mainGroup;
		BButton*		newMasterpieceButton;
		BButton*		newThoughtButton;
		BListView*		openThoughtListView;
		BListView*		openMasterpieceListView;
		BStringView*	thoughtStringView;
		BStringView*	masterpieceStringView;
		BStringView*	openMasterpieceStringView;
		BStringView*	openThoughtStringView;
		sqlite3*		mpdb;
		ErrorAlert*		eAlert;
		char*			sqlErrMsg;
		char**			selectResult;
		int				returnValue;
		int				sqlValue;
		int				nrow, ncol;
		int				selected;
		int64			showLauncher;
		BString			tmpString;
		
		MPBuilder*		mpBuilder;
		MPEditor*		mpEditor;
};

#endif
