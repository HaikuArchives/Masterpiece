#include "MPBuilder.h"

MPBuilder::MPBuilder(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID)
	:	BWindow(BRect(100, 100, 900, 700), windowTitle, B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE), launcherMessage(msg), launcherMessenger(msgr)
{
	// initialize controls
	BRect r = Bounds();
	r.bottom = r.bottom - 50;
	availableThoughtListView = new BListView(BRect(10, 10, 100, 30), NULL, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW);
	orderedThoughtListView = new BListView(BRect(10, 10, 100, 30), NULL, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW);
	builderTextView = new BTextView(BRect(0, 0, r.right, 100), NULL, BRect(10, 10, r.right, 100), B_FOLLOW_ALL, B_WILL_DRAW);
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
		.Add(new BScrollView("scroll_editor", builderTextView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 0, 12, 10, 5)
		.SetInsets(0, 0, 0, 0)
	);
	
	currentideaID = ideaID; // pass current idea id selected to builder window to use
	
	mpdb = OpenSqliteDB(); // open mpdb db
	if(mpdb == NULL) // if db doesn't exist
	{
		eAlert = new ErrorAlert("1.21 Sql Error: Sql DB was not opened properly");
		eAlert->Launch();
	}
	else
	{
		PopulateBuilderListViews();
		availableThoughtListView->SetSelectionMessage(new BMessage(DISPLAY_AVAIL_TEXT));
		availableThoughtListView->SetInvocationMessage(new BMessage(MOVE_RIGHT));
		orderedThoughtListView->SetSelectionMessage(new BMessage(DISPLAY_ORDER_TEXT));
		orderedThoughtListView->SetInvocationMessage(new BMessage(MOVE_LEFT));
	}
}
void MPBuilder::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case MOVE_RIGHT: // add item to ordered list
			printf("move right\r\n");
			break;
		case DISPLAY_AVAIL_TEXT: // display preview text from item id
			selected = availableThoughtListView->CurrentSelection(); // selected list item value
			if(selected >= 0) // if something is selected
			{
				curIndex = -1; // tmp value for current index where selected item's id equals id from avalidArray
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(availableThoughtListView->ItemAt(selected));
				for(int j = 0; j < availArrayLength; j++)
				{
					if(availidArray[j] == item->ReturnID())
					{
						curIndex = j;
					}
				}
				if(curIndex > -1)
				{
					builderTextView->SetText(availtextArray[curIndex]);
				}
			}
			break;
		case DISPLAY_ORDER_TEXT: // display preview text from item id
			selected = orderedThoughtListView->CurrentSelection(); // selected list item value
			if(selected >= 0) // if something is selected
			{
				curIndex = -1; // set tmp value
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(orderedThoughtListView->ItemAt(selected));
				builderTextView->SetText(item->Text());
				for(int m = 0; m < orderArrayLength; m++)
				{
					if(orderidArray[m] == item->ReturnID())
					{
						curIndex = m;
					}
				}
				if(curIndex > -1)
				{
					builderTextView->SetText(ordertextArray[curIndex]);
				}
			}
			break;
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
	tmpString = "select ideaid from ideatable where ismp = 0 and mpid is null";
	sqlValue = sqlite3_get_table(mpdb, tmpString, &selectResult, &nrow, &ncol, &sqlErrMsg);
	if(sqlValue == SQLITE_OK) // if sql query was successful
	{
		availArrayLength = nrow;
		// use the number of returned rows to set the arrays...
		availtextArray = new BString[nrow];
		availidArray = new int[nrow];
	}
	else // sql select failed
	{
		eAlert = new ErrorAlert("1.25 Sql Error: No Available Thoughts Exist. Please Create Some First.");
		eAlert->Launch();
	}
	sqlite3_free_table(selectResult); // free table either way
	sqlValue = sqlite3_prepare_v2(mpdb, "select ideaname, ideaid, ideatext from ideatable where ismp = 0 and mpid is null", -1, &ideaStatement, NULL);
	if(sqlValue == SQLITE_OK) // sql statement was prepared
	{
		k = 0;
		while(sqlite3_step(ideaStatement) == SQLITE_ROW) // step through the sql return values
		{
			tmpString = sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 0));
			availableThoughtListView->AddItem(new IdeaStringItem(tmpString, sqlite3_column_int(ideaStatement, 1)));
			availtextArray[k] = sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 2));
			availidArray[k] = sqlite3_column_int(ideaStatement, 1);
			k++;
		}
	}
	else // sql select failed
	{
		eAlert = new ErrorAlert("1.22 Sql Error: No Available Thoughts Exist. Please Create Some First.");
		eAlert->Launch();
	}
	sqlite3_finalize(ideaStatement); // finish with sql statement
	if(currentideaID != -1) // if id has a real value...
	{
		tmpString = "select ideaid from ideatable where ismp = 0 and mpid = ";
		tmpString << currentideaID;
		sqlValue = sqlite3_get_table(mpdb, tmpString, &selectResult, &nrow, &ncol, &sqlErrMsg);
		if(sqlValue == SQLITE_OK) // if sql query was successful
		{
			orderArrayLength = nrow;
			// use the number of returned rows to set the arrays...
			ordertextArray = new BString[nrow];
			orderidArray = new int[nrow];
		}
		else // sql select failed
		{
			eAlert = new ErrorAlert("1.26 Sql Error: No Ordered Thoughts Exist. Please Add Some First.", sqlErrMsg);
			eAlert->Launch();
		}
		sqlite3_free_table(selectResult); // free table either way
		// populate the ordered list items from here with the information from passed id...
		// select ideaname, ideaid, ideatext from ideatable where ismp = 0 and mpid = currentideaID
		sqlValue = sqlite3_prepare_v2(mpdb, "select ideaname, ideaid, ideatext from ideatable where ismp=0 and mpid=?", -1, &ideaStatement, NULL);
		if(sqlValue == SQLITE_OK) // sql statement was prepared
		{
			k = 0;
			if(sqlite3_bind_int(ideaStatement, 1, currentideaID) == SQLITE_OK)
			{
				while(sqlite3_step(ideaStatement) == SQLITE_ROW) // step through the sql return values
				{
					tmpString = sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 0));
					orderedThoughtListView->AddItem(new IdeaStringItem(tmpString, sqlite3_column_int(ideaStatement, 1)));
					ordertextArray[k] = sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 2));
					orderidArray[k] = sqlite3_column_int(ideaStatement, 1);
					k++;
				}
			}
			else
			{
				eAlert = new ErrorAlert("1.24 Sql Error: Sql bind failed.");
				eAlert->Launch();
			}
		}
		else // sql select failed
		{
			eAlert = new ErrorAlert("1.23 Sql Error: No Ordered Thoughts Exist.  Please Add Some First.");
			eAlert->Launch();
		}
	}
}
