#ifndef MARKUPWINDOW_H
#define MARKUPWINDOW_H

#include <Window.h>
#include <Message.h>
#include <Messenger.h>
#include <View.h>
#include <StringView.h>
#include <ScrollView.h>
#include <ListView.h>
#include <TextView.h>
#include <GroupLayout.h>
#include <GridLayoutBuilder.h>
#include <SpaceLayoutItem.h>
#include <String.h>
#include <stdio.h>
#include <File.h>
#include <malloc.h>

#include "ErrorAlert.h"
#include "HelpStringItem.h"
#include "CommonFunctions.h"

#define LOAD_CONTENT	'ldct' // load content in window

class MarkupWindow : public BWindow
{
public:
				MarkupWindow(BRect frame, const char* title);
		void	MessageReceived(BMessage* msg);
		void	AddMarkupItem(BString topicstring, BString contentstring);
		bool	QuitRequested(void);
		
private:
		BView*			backView;
		BListView*		topicListView;
		BTextView*		contentTextView;
		ErrorAlert*		eAlert;
		int				selected;
};

#endif
