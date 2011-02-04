#ifndef MPBUILDER_H
#define MPBUILDER_H

#include <Window.h>
#include <View.h>
#include <sqlite3.h>
#include <Application.h>
#include <String.h>
#include <GroupLayout.h>
#include <GridLayoutBuilder.h>
#include <ListView.h>
#include <ScrollView.h>
#include <StringView.h>
#include <Messenger.h>
#include <Message.h>
#include <stdio.h>

#include "ErrorAlert.h"

class MPBuilder : public BWindow
{
public:
						MPBuilder(const BMessage &msg, const BMessenger &msgr, BString windowTitle);
						MPBuilder(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID);
		void			MessageReceived(BMessage* msg);
		bool			QuitRequested(void);
private:
		BView*			backView;
		BGroupLayout*	mainGroup;
		BMessage		launcherMessage;
		BMessenger		launcherMessenger;
};

#endif
