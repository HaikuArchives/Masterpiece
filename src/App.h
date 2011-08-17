#ifndef APP_H
#define APP_H

#include <Application.h>

#include "MPLauncher.h"

// initialize the application

class App : public BApplication
{
public:
							App(int aargc, char** aargv);
			static int		GetGlobalArgc(void);
			static char**	GetGlobalArgv(void);
			BApplication*	GetThisApp(void);
private:
			static int 		global_argc;
			static char** 	global_argv;
};

#endif
