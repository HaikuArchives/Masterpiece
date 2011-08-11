#ifndef HELPERWINDOWS_H
#define HELPERWINDOWS_H

#include <Window.h>
#include <View.h>
#include <StringView.h>
#include <stdio.h>

class HelperWindows : public BWindow
{
public:
				HelperWindows(BRect frame, const char* title);
		void	AddText(BRect tframe, const char* hname, const char* helptext);
		bool	QuitRequested(void);
		
private:
		BView*			backView;
};

#endif
