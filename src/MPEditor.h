#ifndef MPEDITOR_H
#define MPEDITOR_H

#include "EditorMenu.h"
#include "SaveIdea.h"
#include "EditIdeaName.h"
#include "HelperWindows.h"
#include "CommonFunctions.h"
#include "PublishFilePanel.h"
#include "MarkupWindow.h"
#include "AboutWindow.h"

//#include <sys/wait.h>

#define SHOW_LAUNCHER		'lnch' // show mp launcher window
#define UPDATE_TITLE		'uttl' // update title...

class MPEditor : public BWindow
{
public:
							MPEditor(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID);
		void				MessageReceived(BMessage* msg);
		bool				QuitRequested(void);
		void				SetStatusBar(const char* string);
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
		static int32		PreviewThread(void* data);
		static int32		PublishThread(void* data);
		BString				fileExt; // file extension of converted file
		BMessage*			editorMessage;
		
};


#endif
