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

#include "MPMenuBar.h"
#include "NewMasterView.h"
#include "OpenMasterView.h"
#include "SummaryView.h"
#include "ErrorAlert.h"

#define MNG_CONTENT_MSG		'mcm'
#define MNG_LAYOUT_MSG		'mlm'

class MainWindow : public BWindow
{
public:
							MainWindow(void);
			void			MessageReceived(BMessage *msg);
			void			Draw(BRect rect);
			bool			QuitRequested(void);
private:
			MPMenuBar		*mpMenuBar;
			BBitmap			*testImage;
			BView			*toolbarView;
			BPicture		*onImage;
			BPictureButton 	*newButton;
			NewMasterView	*fullView;
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
			OpenMasterView	*openView;
			SummaryView		*sumView;
			BString			tmpString;
			int				returnValue;
			sqlite3			*mpdb;
			int				sqlValue;
			char			*sqlErrMsg;
			char			**selectResult;
			int				nrow, ncol;
			ErrorAlert		*errorAlert;
};

#endif
