#ifndef MPLAUNCHER_H
#define MPLAUNCHER_H

#include "MPBuilder.h"
#include "MPEditor.h"
#include "CommonFunctions.h"
#include "IdeaStringItem.h"

#define CREATE_NEW_MP		'cnmp'
#define	CREATE_NEW_THT		'cntt'
#define SELECT_EXIST_MP		'semp'
#define	SELECT_EXIST_THT	'sett'
#define	OPEN_EXISTING_MP	'oemp'
#define	OPEN_EXISTING_THT	'oett'
#define DELETE_LAUNCHER_THT	'delt'
#define DELETE_MP			'delm'
#define IMPORT_THT			'impt' // import thought(s)
#define SHOW_LAUNCHER		'lnch' // show mp launcher window
#define START_DELETE		'sdel' // start delete chooser

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
		BButton*		importThoughtButton;
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
