#ifndef MPLAUNCHER_H
#define MPLAUNCHER_H

#include <Window.h>
#include <View.h>
#include <sqlite3.h>
#include <GroupLayout.h>
#include <SpaceLayoutItem.h>
#include <GridLayoutBuilder.h>
#include <Button.h>
#include <ListView.h>
#include <ScrollView.h>
#include <StringView.h>
#include <Message.h>
#include <Messenger.h>
#include <Application.h>

#define CREATE_NEW_MP		'cnmp'
#define	CREATE_NEW_THT		'cntt'
#define	OPEN_EXISTING_MP	'oemp'
#define	OPEN_EXISTING_THT	'oett'

class MPLauncher : public BWindow
{
public:
						MPLauncher(void);
		void			MessageReceived(BMessage* msg);
		bool			QuitRequested(void);
		
private:
	
		BView*			backView;
		BGroupLayout*	mainGroup;
		BButton*		newMasterpieceButton;
		BButton*		newThoughtButton;
		BListView*		openThoughtListView;
		BListView*		openMasterpieceListView;
		BStringView*	thoughtStringView;
		BStringView*	masterpieceStringView;
		BStringView*	openMasterpieceStringView;
		BStringView*	openThoughtStringView;
		
};

#endif
