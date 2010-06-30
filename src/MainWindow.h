#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <MenuBar.h>
#include <View.h>
#include <Picture.h>
#include <PictureButton.h>
#include <TextView.h>
#include <TabView.h>
#include <Entry.h>
#include <Path.h>
#include <Directory.h>
#include <Alert.h>
#include <ListView.h>

#define MENU_NEW_MSG		'new'
#define MENU_OPN_MSG		'open'
#define ADD_NEW_COURSE		'add'
#define CANCEL_NEW_COURSE	'cnc'
#define MNG_CONTENT_MSG		'mcm'
#define MNG_LAYOUT_MSG		'mlm'

class MainWindow : public BWindow
{
public:
						MainWindow(void);
			void		MessageReceived(BMessage *msg);
			bool		QuitRequested(void);
			
private:
			BMenuBar	*fMenuBar;
			BMenu		*fileMenu;
			BMenu		*manageMenu;
			BMenuItem	*newFileMenuItem;
			BMenuItem	*openFileMenuItem;
			BMenuItem	*quitMenuItem;
			BMenuItem	*contentMenuItem;
			BMenuItem	*layoutMenuItem;
			BBitmap		*testImage;
			BView		*toolbarView;
			BPicture	*onImage;
			BPictureButton *newButton;
			BTextView	*titleText;
			BView		*fullView;
			BView		*openView;
			BTabView	*contentTabView;
			BTab		*tmpTab;
			BEntry		homeEntry;
			BEntry		newCourseTestEntry;
			BEntry		entry;
			BPath		tmpPath;
			BDirectory 	*homeDir;
			BAlert		*debugAlert;
			BListView	*openListView;
};

#endif
