#include "App.h"

// app function

App::App(int argc, char** argv)
	:	BApplication("application/x-vnd.pjr-MasterPiece")
{
	global_argc = argc;
	global_argv = argv;
	MPLauncher* mpLauncher = new MPLauncher(); // initialize the mplauncher window
	mpLauncher->Show(); // show the mp launcher window
}
int App::GetGlobalArgc(void)
{
	return global_argc;
}

char** App::GetGlobalArgv(void)
{
	return global_argv;
}

BApplication* App::GetThisApp(void)
{
	return this;
}

int main(int argc, char** argv)
{
	App *app = new App(argc, argv); // initialize app
	app->Run(); // run the app
	delete app; // clean up the app
	return 0; // return 0 and end.
}
