#ifndef MPLAUNCHER_H
#define MPLAUNCHER_H

#include <Window.h>
#include <View.h>
#include <sqlite3.h>
#include <GridLayout.h>
#include <Button.h>
#include <ListView.h>
#include <ScrollView.h>
#include <StringView.h>

class MPLauncher : public BWindow
{
public:
						MPLauncher(void);
		void			MessageReceived(BMessage* msg);
		
private:
	
		BView*			backView;
		BGridLayout*	mainGrid;
		BButton*		newMasterpieceButton;
		BButton*		newThoughtButton;
		BListView*		openThoughtListView;
		BListView*		openMasterpieceListView;
		BStringView*	thoughtStringView;
		BStringView*	masterpieceStringView;
		
};

#endif
