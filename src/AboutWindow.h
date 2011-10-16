#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <Window.h>
#include <Message.h>
#include <Messenger.h>
#include <View.h>
#include <StringView.h>
#include <ScrollView.h>
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

class AboutWindow : public BWindow
{
public:
				AboutWindow(BRect frame, const char* title);
		void	MessageReceived(BMessage* msg);
		bool	QuitRequested(void);
		
private:
		BView*			backView;
		BTextView*		contentTextView;
		ErrorAlert*		eAlert;
};

#endif
