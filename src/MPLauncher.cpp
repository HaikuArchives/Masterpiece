#include "MPLauncher.h"

MPLauncher::MPLauncher(void)
	:	BWindow(BRect(100, 100, 450, 300), "MasterPiece Launcher", B_DOCUMENT_WINDOW,  B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE)
{
	mainGrid = new BGridLayout();
	newThoughtButton = new BButton(BRect(10, 10, 90, 35), NULL, "Create a New...", new BMessage(CREATE_NEW_THT), B_FOLLOW_NONE, B_WILL_DRAW);
	newMasterpieceButton = new BButton(BRect(10, 10, 90, 35), NULL, "Create a New...", new BMessage(CREATE_NEW_MP), B_FOLLOW_NONE, B_WILL_DRAW);
	thoughtStringView = new BStringView(BRect(10, 10, 200, 30), NULL, "Work on a Thought");
	masterpieceStringView = new BStringView(BRect(10, 10, 200, 30), NULL, "Work on a Masterpiece");
	openMasterpieceStringView = new BStringView(BRect(10, 10, 200, 30), NULL, "Open an Existing...");
	openThoughtStringView = new BStringView(BRect(10, 10, 200, 30), NULL, "Open an Existing...");
	openThoughtListView = new BListView(BRect(10, 10, 100, 30), NULL, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW);
	openMasterpieceListView = new BListView(BRect(10, 10, 100, 30), NULL, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW);
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	
	backView->SetLayout(mainGrid);
	mainGrid->SetInsets(2, 2, 2, 2);
	mainGrid->AddView(masterpieceStringView, 0, 0);
	mainGrid->AddView(thoughtStringView, 1, 0);
	mainGrid->AddView(newMasterpieceButton, 0, 1);
	mainGrid->AddView(newThoughtButton, 1, 1);
	mainGrid->AddView(openMasterpieceStringView, 0, 2);
	mainGrid->AddView(openThoughtStringView, 1, 2);
	mainGrid->AddView(new BScrollView("scroll_masterpiecelist", openMasterpieceListView,  B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 0, 3, 1, 3);
	mainGrid->AddView(new BScrollView("scroll_thoughtlist", openThoughtListView,  B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 1, 3, 1, 3);
	
	openMasterpieceListView->SetInvocationMessage(new BMessage(OPEN_EXISTING_MP));
	openThoughtListView->SetInvocationMessage(new BMessage(OPEN_EXISTING_THT));
}
void MPLauncher::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case CREATE_NEW_MP:
			// do something here
			break;
		case CREATE_NEW_THT:
			// do something here
			break;
		case OPEN_EXISTING_MP:
			// do something here
			break;
		case OPEN_EXISTING_THT:
			// do something here
			break;
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}		
	}
}
