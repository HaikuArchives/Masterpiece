#include "MPEditor.h"

MPEditor::MPEditor(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID)
	:	BWindow(BRect(100, 100, 900, 700), windowTitle, B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE), launcherMessage(msg), launcherMessenger(msgr)
{
	BRect r = Bounds();
	r.bottom = r.bottom - 50;
	editorTextView = new BTextView(r, NULL, r, B_FOLLOW_ALL, B_WILL_DRAW);	
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	
	backView->SetLayout(new BGroupLayout(B_HORIZONTAL, 0.0));
	backView->AddChild(BGridLayoutBuilder()
		.Add(new EditorMenu(), 0, 0)
		.Add(new BScrollView("scroll_editor", editorTextView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 0, 1)
		//.SetInsets(0, 0, 0, 0)
	);
	currentideaID = ideaID;
}
void MPEditor::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case MENU_NEW_THT:
			tmpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Editor - untitled", -1);
			tmpEditor->Show();
			break;
		case MENU_EDT_THT:
			printf(" must open edit name dialog\r\n");
			break;
		case MENU_SAV_THT:
			if(currentideaID == -1)
			{
				printf("must open save dialog\r\n");
				// save name and then call insert into sql...
			}
			else
			{
				printf("must just write sql\r\n");
				// update data to sql with id value
			}
			break;
		case MENU_PRV_THT:
			break;
		case MENU_PUB_THT:
			break;
		case MENU_HLP_THT:
			break;
		case MENU_KEY_THT:
			break;
		case MENU_MRK_THT:
			break;
		case MENU_ABT_THT:
			break;
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}
bool MPEditor::QuitRequested(void)
{
	launcherMessage.MakeEmpty();
	launcherMessage.AddInt64("showLauncher", 1);
	launcherMessenger.SendMessage(&launcherMessage);
	return true;
}
