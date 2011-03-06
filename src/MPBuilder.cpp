#include "MPBuilder.h"

MPBuilder::MPBuilder(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID)
	:	BWindow(BRect(100, 100, 900, 700), windowTitle, B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE), launcherMessage(msg), launcherMessenger(msgr)
{
	// initialize controls
	BRect r = Bounds();
	r.bottom = r.bottom - 50;
	availableThoughtListView = new BListView(BRect(10, 10, 100, 30), NULL, B_MULTIPLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW);
	orderedThoughtListView = new BListView(BRect(10, 10, 100, 30), NULL, B_MULTIPLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW);
	rightButton = new BButton(BRect(10, 10, 90, 35), NULL, ">", new BMessage(MOVE_RIGHT), B_FOLLOW_NONE, B_WILL_DRAW);
	leftButton = new BButton(BRect(10, 10, 90, 35), NULL, "<", new BMessage(MOVE_LEFT), B_FOLLOW_NONE, B_WILL_DRAW);
	topButton = new BButton(BRect(10, 10, 90, 35), NULL, "TOP", new BMessage(MOVE_TOP), B_FOLLOW_NONE, B_WILL_DRAW);
	upButton = new BButton(BRect(10, 10, 90, 35), NULL, "UP", new BMessage(MOVE_UP), B_FOLLOW_NONE, B_WILL_DRAW);
	downButton = new BButton(BRect(10, 10, 90, 35), NULL, "DOWN", new BMessage(MOVE_DOWN), B_FOLLOW_NONE, B_WILL_DRAW);
	bottomButton = new BButton(BRect(10, 10, 90, 35), NULL, "BOTTOM", new BMessage(MOVE_BOTTOM), B_FOLLOW_NONE, B_WILL_DRAW);
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	// gui layout builder
	backView->SetLayout(new BGroupLayout(B_HORIZONTAL, 0.0));
	backView->AddChild(BGridLayoutBuilder()
		.Add(new BuilderMenu(), 0, 0, 10, 1)
		.Add(new BStringView(BRect(10, 10, 200, 30), NULL, "All Available Thoughts"), 0, 1)		
		.Add(new BStringView(BRect(10, 10, 200, 30), NULL, "Ordered Thoughts"), 5, 1)		
		.Add(new BScrollView("scroll_available", availableThoughtListView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 0, 2, 4, 10)
		.Add(rightButton, 4, 4)
		.Add(leftButton, 4, 5)
		.Add(topButton, 4, 6)
		.Add(upButton, 4, 7)
		.Add(downButton, 4, 8)
		.Add(bottomButton, 4, 9)
		.Add(new BScrollView("scroll_ordered", orderedThoughtListView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 5, 2, 5, 10)
		.SetInsets(0, 0, 0, 0)
	);
	
	currentideaID = ideaID; // pass current idea id selected to builder window to use
	
	mpdb = OpenSqliteDB(); // open mpdb db
	if(mpdb == NULL) // if db doesn't exist
	{
		eAlert = new ErrorAlert("1.21 Sql Error: Sql DB was not opened properly");
		eAlert->Launch();
	}
	if(currentideaID != -1) // if id has a real value
	{
		// Pull data from db and populate ordered thought list with them...
		PopulateBuilderListViews();
	}
	// Pull data from db and populate available thought list with them.
}
void MPBuilder::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}
bool MPBuilder::QuitRequested(void)
{
	// on quit, show launcher with message
	launcherMessage.MakeEmpty();
	launcherMessage.AddInt64("showLauncher", 1);
	launcherMessenger.SendMessage(&launcherMessage);
	return true;
}
void MPBuilder::PopulateBuilderListViews(void)
{
	availableThoughtListView->MakeEmpty();
	orderedThoughtListView->MakeEmpty();
	sqlValue = sqlite3_prepare_v2(mpdb, "select ideaname, ideaid, ideatext from ideatable where ismp = 0 and mpid is null", -1, &ideaStatement, NULL);
	if(sqlValue == SQLITE_OK) // sql statement was prepared
	{
		while(sqlite3_step(ideaStatement) == SQLITE_ROW) // step through the sql return values
		{
			tmpString = sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 0));
			availableThoughtListView->AddItem(new IdeaStringItem(tmpString, sqlite3_column_int(ideaStatement, 1)));
		}
	}
	else // sql select failed
	{
	}
	sqlite3_finalize(ideaStatement); // finish with sql statement
}
