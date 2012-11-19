#ifndef MPEDITOR_H
#define MPEDITOR_H

#include "MainMenu.h"
#include "EditorTextView.h"
#include "SaveIdea.h"
#include "EditIdeaName.h"
#include "HelperWindows.h"
#include "CommonFunctions.h"
#include "PublishFilePanel.h"
#include "AboutWindow.h"

#define SHOW_LAUNCHER		'lnch' // show mp launcher window
#define UPDATE_TITLE		'uttl' // update title...
#define SAVE_TITLE			'svid' // update title...
#define CLEAR_STATUS		'clst' // clear statusbar
#define EXPORT_IDEA			'expi' // export idea

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
		BFilePanel*			exportPanel;
		EditorTextView*		editorTextView;
		MPEditor*			tmpEditor;
		ErrorAlert*			eAlert;
		SaveIdea*			saveIdea;
		EditIdeaName*		editIdeaName;
		HelperWindows*		helperWindow;
		AboutWindow*		aboutWindow;
		sqlite3*			mpdb;
		sqlite3_stmt*		ideaStatement;
		char*				sqlErrMsg;
		BString				tmpString;
		BString				updateTitle;
		BString				saveTitle;
		BString				tmpText;
		int					sqlValue;
		int					currentideaID;
		float				xPos;
		float				yPos;
		SqlObject*			sqlObject;
		PublishFilePanel*	pubEditorPanel;
		BStringView*		editorStatusBar;
		MainMenu*			editorMenuBar;
		static int32		PreviewThread(void* data); // preview thread
		static int32		PublishThread(void* data); // publish thread
		static int32		HelpThread(void* data); // general help thread
		static int32		ExportThread(void* data); // export thread
		BString				fileExt; // file extension of converted file
		entry_ref			pubRef; // publish ref entry value
		BString				pubName;  // publish file name
		int64				clearStatus;
		int					openflag;
		entry_ref			exportref; // export ref entry value
};

#endif
