#ifndef MPEDITOR_H
#define MPEDITOR_H

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

#include "ErrorAlert.h"

class MPEditor : public BWindow
{
public:
						MPEditor(const BMessage &msg, const BMessenger &msgr, BString windowTitle);
		void			MessageReceived(BMessage* msg);
		bool			QuitRequested(void);
private:
		BView*			backView;
		BGroupLayout*	mainGroup;
		BMessage		launcherMessage;
		BMessenger		launcherMessenger;
};


#endif
