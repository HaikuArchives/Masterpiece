#include "MPLauncher.h"

MPLauncher::MPLauncher(void)
	:	BWindow(BRect(100, 100, 450, 300), "MasterPiece Launcher", B_DOCUMENT_WINDOW,  B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE)
{
	mainGrid = new BGridLayout();
	newThoughtButton = new BButton(BRect(10, 10, 90, 35), NULL, "Create a New", new BMessage(CREATE_NEW_THT), B_FOLLOW_NONE, B_WILL_DRAW);
	newMasterpieceButton = new BButton(BRect, 10, 10, 90, 35), NULL, "Create a New", new BMessage(CREATE_NEW_MP), B_FOLLOW_NONE, B_WILL_DRAW);
	
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	
}
