#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <View.h>
#include <Picture.h>
#include <PictureButton.h>
#include <TabView.h>
#include <Entry.h>
#include <Path.h>
#include <Directory.h>
#include <Alert.h>
#include <Application.h>
#include <sqlite3.h>
#include <stdio.h>
#include <Roster.h>
#include <String.h>
#include <File.h>
#include <GroupLayout.h>
#include <GridLayout.h>

#include "MPMenuBar.h"
#include "SummaryView.h"
#include "ThoughtView.h"
#include "ErrorAlert.h"
#include "NewWindow.h"
#include "OpenWindow.h"

#define MNG_CONTENT_MSG		'mcm'
#define MNG_LAYOUT_MSG		'mlm'
#define UPDATE_NEW_MP		'unmp'
#define UPDATE_OPEN_MP		'uomp'

class MainWindow : public BWindow
{
public:
							MainWindow(void);
			void			MessageReceived(BMessage *msg);
			void			Draw(BRect rect);
			bool			QuitRequested(void);
			void			FrameResized(float width, float height);
			void			PopulateSummaryView(int mpID);
private:
			MPMenuBar		*mpMenuBar;
			BBitmap			*testImage;
			BView			*toolbarView;
			BPicture		*onImage;
			BPictureButton 	*newButton;
			BTabView		*contentTabView;
			BTab			*tmpTab;
			BEntry			homeEntry;
			BEntry			newCourseTestEntry;
			BEntry			entry;
			BPath			tmpPath;
			BDirectory 		*homeDir;
			BAlert			*debugAlert;
			BAlert			*userAlert;
			BListView		*openListView;
			SummaryView		*sumView;
			ThoughtView		*thoughtView;
			BString			tmpString;
			int				returnValue;
			sqlite3			*mpdb;
			int				sqlValue;
			char			*sqlErrMsg;
			char			**selectResult;
			int				nrow, ncol;
			ErrorAlert		*errorAlert;
			NewWindow		*newWin;
			OpenWindow		*openWin;
			float			xPos;
			float			yPos;
			BString			mptitle;
			int64			mpid;
};

#endif
