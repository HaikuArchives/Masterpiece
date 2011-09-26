#ifndef MARKUPWINDOW_H
#define MARKUPWINDOW_H

#include <Window.h>
#include <View.h>
#include <StringView.h>
#include <ScrollView.h>
#include <ListView.h>
#include <TextView.h>
#include <GroupLayout.h>
#include <GridLayoutBuilder.h>
#include <stdio.h>

#include "ErrorAlert.h"
#include "HelpStringItem.h"

class MarkupWindow : public BWindow
{
public:
				MarkupWindow(BRect frame, const char* title);
		void	AddMarkupItem(BString topicstring, BString contentstring);
		bool	QuitRequested(void);
		
private:
		BView*			backView;
		BListView*		topicListView;
		BTextView*		contentTextView;
		ErrorAlert*		eAlert;
};

#endif
