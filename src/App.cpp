#include "App.h"

// app function

App::App(void)
	:	BApplication("application/x-vnd.pjr-MasterPiece")
{
	MainWindow *mainwin = new MainWindow(); // initialize the main app window
	mainwin->Show(); // show the main app window
}


int
main(void)
{
	App *app = new App(); // initialize app
	app->Run(); // run the app
	delete app; // clean up the app
	return 0; // return 0 and end.
}
