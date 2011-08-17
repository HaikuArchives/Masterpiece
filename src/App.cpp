#include "App.h"

// app function

App::App(void)
	:	BApplication("application/x-vnd.pjr-MasterPiece")
{
	MPLauncher* mpLauncher = new MPLauncher(); // initialize the mplauncher window
	mpLauncher->Show(); // show the mp launcher window
}
int main(void)
{
	App *app = new App(); // initialize app
	app->Run(); // run the app
	delete app; // clean up the app
	return 0; // return 0 and end.
}
