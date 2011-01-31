#ifndef MPLAUNCHER_H
#define MPLAUNCHER_H

#include <Window.h>
#include <View.h>
#include <sqlite3.h>
#include <GridLayout.h>

class MPLauncher : public BWindow
{
public:
				MPLauncher(void);
		void	MessageReceived(BMessage* msg);
		
private:
	
};

#endif
