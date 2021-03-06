#ifndef MPBUILDER_H
#define MPBUILDER_H

#include "MainMenu.h"
#include "EditorTextView.h"
#include "BuilderButton.h"
#include "BuilderListView.h"
#include "SaveIdea.h"
#include "EditIdeaName.h"
#include "CommonFunctions.h"
#include "MPEditor.h"
#include "IdeaStringItem.h"
#include "HelperWindows.h"
#include "PublishFilePanel.h"

#define SHOW_LAUNCHER			'lnch' // show mp launcher window
#define MP_UPDATE_TITLE			'uttl' // update title
#define SAVE_TITLE				'svid' // update title...
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
#define END_EDIT_VIEW			'eedt' // end edit view
#define CLEAR_STATUS			'clst' // clear statusbar
#define EXPORT_IDEA				'expi' // export idea

class MPBuilder : public BWindow
{
public:
							MPBuilder(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID);
		virtual				~MPBuilder();
		void				MessageReceived(BMessage* msg);
		bool				QuitRequested(void);
		void				PopulateBuilderListViews(void);
		void				ReorderOrderedListView(void);
		void				ModifyOrderedItems(int curOrderNumber, int newOrderNumber);
		void				SetStatusBar(const char* string);
		const char*			GetStatusBar(void);
private:
		BView*				backView;
		BMessage			launcherMessage;
		BMessenger			launcherMessenger;
		BStringView*		availableStringView;
		BStringView*		orderedStringView;
		BStringView*		quickStringView;
		BuilderListView*	availableThoughtListView;
		BuilderListView*	orderedThoughtListView;
		EditorTextView*		builderTextView;
		BuilderButton*		rightButton;
		BuilderButton*		leftButton;
		BuilderButton*		topButton;
		BuilderButton*		upButton;
		BuilderButton*		downButton;
		BuilderButton*		bottomButton;
		BuilderButton*		deleteButton;
		PublishFilePanel*	publishPanel;
		BFilePanel*			exportPanel;
		MPEditor*			tmpEditor;
		ErrorAlert*			eAlert;
		SaveIdea*			saveIdea;
		EditIdeaName*		editIdeaName;
		HelperWindows*		helperWindow;
		sqlite3*			mpdb;
		sqlite3_stmt*		ideaStatement;
		sqlite3_stmt*		reorderStatement;
		char*				sqlErrMsg;
		BString				updateTitle;
		BString				saveTitle;
		BString				tmpString;
		int64				showLauncher;
		int					sqlValue;
		int					currentideaID;
		int					selected;
		int					bindValue;
		int					availorderBit;
		float				xPos;
		float				yPos;
		SqlObject*			sqlObject;
		SqlObject*			sqlObject2;
		BStringView*		builderStatusBar;
		MainMenu*			builderMenuBar;
		static int32		PreviewThread(void* data); // preview thread
		static int32		PublishThread(void* data); // publish thread
		static int32		HelpThread(void* data); // general help thread
		static int32		ExportThread(void* data);
		BString				fileExt; // file extension of converted file
		entry_ref			pubRef; // publish ref entry value
		BString				pubName; // publish file name
		BString				mpData;
		int64				clearStatus;
		int					openflag;
		entry_ref			exportref;
		
};

#endif
