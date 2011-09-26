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

class MarkupWindow : public BWindow
{
public:
				MarkupWindow(BRect frame, const char* title);
		void	AddMarkupItem(BString topicString, BString contentString);
		bool	QuitRequested(void);
		
private:
		BView*			backView;
		BListView*		topicListView;
		BTextView*		contentTextView;
		ErrorAlert*		eAlert;
};

#endif
