#ifndef MARKUPWINDOW_H
#define MARKUPWINDOW_H

#include <Window.h>
#include <View.h>
#include <StringView.h>
#include <stdio.h>

class MarkupWindow : public BWindow
{
public:
				MarkupWindow(BRect frame, const char* title);
		//void	AddText(BRect tframe, const char* hname, const char* helptext);
		//void	AddMarkupItem(item name, document location, need reference to do it);
		bool	QuitRequested(void);
		
private:
		BView*			backView;
};

#endif
