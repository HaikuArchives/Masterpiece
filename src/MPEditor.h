#ifndef MPEDITOR_H
#define MPEDITOR_H

#include "EditorMenu.h"
#include "EditorTextView.h"
#include "SaveIdea.h"
#include "EditIdeaName.h"
#include "HelperWindows.h"
#include "CommonFunctions.h"
#include "PublishFilePanel.h"
#include "MarkupWindow.h"
#include "AboutWindow.h"

#define SHOW_LAUNCHER		'lnch' // show mp launcher window
#define UPDATE_TITLE		'uttl' // update title...
#define CLEAR_STATUS		'clst' // clear statusbar

class MPEditor : public BWindow
{
public:
							MPEditor(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID);
		void				MessageReceived(BMessage* msg);
		bool				QuitRequested(void);
		void				SetStatusBar(const char* string);
		const char*			GetStatusBar(void);
private:
		BView*				backView;
		BMessage			launcherMessage;
		BMessenger			launcherMessenger;
		EditorTextView*		editorTextView;
		MPEditor*			tmpEditor;
		ErrorAlert*			eAlert;
		SaveIdea*			saveIdea;
		EditIdeaName*		editIdeaName;
		HelperWindows*		helperWindow;
		MarkupWindow*		markupWindow;
		AboutWindow*		aboutWindow;
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
		PublishFilePanel*	pubEditorPanel;
		BStringView*		editorStatusBar;
		EditorMenu*			editorMenuBar;
		static int32		PreviewThread(void* data); // preview thread
		static int32		PublishThread(void* data); // publish thread
		BString				fileExt; // file extension of converted file
		entry_ref			pubRef; // publish ref entry value
		BString				pubName;  // publish file name
		int64				clearStatus;
		
};

#endif
