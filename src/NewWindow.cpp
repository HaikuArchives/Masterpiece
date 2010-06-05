#include "NewWindow.h"

#include <Application.h>
#include <View.h>
#include <StringView.h>
#include <Button.h>
#include <Alert.h>

#define TEXT_INSET 3.0

NewWindow::NewWindow(void)
	:	BWindow(BRect(100,100,450,200), "New MasterPiece", B_MODAL_WINDOW, B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE)
{
	BRect viewFrame(10, 40, 100, 100);
	BRect textFrame;
	textFrame.left = TEXT_INSET;
	textFrame.right = viewFrame.right - viewFrame.left - TEXT_INSET;
	textFrame.top = TEXT_INSET;
	textFrame.bottom = viewFrame.bottom - viewFrame.top - TEXT_INSET;
	BView *fullView = new BView(Bounds(), 0, B_FOLLOW_ALL, B_WILL_DRAW);
	BStringView *titleString = new BStringView(BRect(10, 10, 100, 20), NULL, "Enter Title:");
	titleText = new BTextView(viewFrame, "textTitle", textFrame, B_FOLLOW_NONE, B_WILL_DRAW);
	
	BButton *addButton = new BButton(BRect(120, 10, 180, 30), NULL, "Add", new BMessage(1), B_FOLLOW_ALL, B_WILL_DRAW);
	BButton *cancelButton = new BButton(BRect(120, 50, 180, 80), NULL, "Close", new BMessage(B_QUIT_REQUESTED), B_FOLLOW_ALL, B_WILL_DRAW);
	fullView->AddChild(titleString);
	fullView->AddChild(addButton);
	fullView->AddChild(cancelButton);
	fullView->AddChild(titleText);
	AddChild(fullView);
	viewFrame.InsetBy(-2.0, -2.0);
	
}

void NewWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case 1:
		{
			// do something here...
			const char *textFieldText;
			textFieldText = titleText->Text();
			BAlert *alert = new BAlert("Alert", textFieldText, "OK");
			const char *tmpRoot;
			tmpRoot = "/boot/home/MasterPiece/";
			const char *finalString;
			finalString->sprintf("%s%s", tmpRoot, textFieldText);
			create_directory(finalString, 0777);
			long result = alert->Go();
			Close();
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

bool NewWindow::QuitRequested(void)
{
	Close();
	return true;
}
