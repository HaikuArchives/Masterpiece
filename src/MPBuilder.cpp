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
		availableThoughtListView->SetInvocationMessage(new BMessage(AVAIL_THOUGHT_EDITOR));
		orderedThoughtListView->SetSelectionMessage(new BMessage(DISPLAY_ORDER_TEXT));
		orderedThoughtListView->SetInvocationMessage(new BMessage(ORDER_THOUGHT_EDITOR));
	}
}
void MPBuilder::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case MOVE_RIGHT: // add item to ordered list
			selected = availableThoughtListView->CurrentSelection(); // selected list item value
			if(selected >= 0) // if something is selected
			{
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(availableThoughtListView->ItemAt(selected));
				sqlValue = sqlite3_prepare_v2(mpdb, "update ideatable set mpid=?, ordernumber=? where ideaid=?", -1, &ideaStatement, NULL);
				if(sqlValue == SQLITE_OK) // sql statement was prepared
				{
					if(sqlite3_bind_int(ideaStatement, 1, currentideaID) == SQLITE_OK) // sql bind successful
					{
						if(sqlite3_bind_int(ideaStatement, 2, (orderedThoughtListView->CountItems()+1)) == SQLITE_OK) // sql bind successful
						{
							if(sqlite3_bind_int(ideaStatement, 3, item->ReturnID()) == SQLITE_OK) // sql bind successful
							{
								sqlite3_step(ideaStatement); // execute the update statement
							}
							else
							{
								eAlert = new ErrorAlert("1.28 Sql Error: Move Right Bind 3 Failed");
								eAlert->Launch();
							}
						}
						else
						{
							eAlert = new ErrorAlert("1.27 Sql Error: Move Right Bind 2 Failed");
							eAlert->Launch();
						}
					}
					else
					{
						eAlert = new ErrorAlert("1.26 Sql Error: Move Right Bind 1 Failed");
						eAlert->Launch();
					}
				}
				else // sql update failed
				{
					eAlert = new ErrorAlert("1.25 Sql Error: Move Right Update Failed");
					eAlert->Launch();
				}
				sqlite3_finalize(ideaStatement); // finish with sql statement
				PopulateBuilderListViews(); // update listviews' items
			}
			break;
		case MOVE_LEFT: // remove item from ordered list of current mp.
			selected = orderedThoughtListView->CurrentSelection(); // selected list item value
			if(selected >= 0) // if something is selected
			{
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(orderedThoughtListView->ItemAt(selected));
				sqlValue = sqlite3_prepare_v2(mpdb, "update ideatable set mpid=NULL, ordernumber=NULL where ideaid=?", -1, &ideaStatement, NULL);
				if(sqlValue == SQLITE_OK) // sql statement was prepared
				{
					if(sqlite3_bind_int(ideaStatement, 1, item->ReturnID()) == SQLITE_OK) // sql bind successful
					{
						sqlite3_step(ideaStatement); // execute the update statement
					}
					else
					{
						eAlert = new ErrorAlert("1.30 Sql Error: Move Left Bind Failed");
						eAlert->Launch();
					}
				}
				else // sql update failed
				{
					eAlert = new ErrorAlert("1.29 Sql Error: Move Left Update Failed");
					eAlert->Launch();
				}
				sqlite3_finalize(ideaStatement); // finish with sql statement
				PopulateBuilderListViews(); // update listviews' items
				// need to reorder the ordernumbers and then repopulate the orderedlistview
			}
			break;
		case DISPLAY_AVAIL_TEXT: // display preview text from item id
			topButton->SetEnabled(false); // disable top button
			upButton->SetEnabled(false); // disable the up button
			downButton->SetEnabled(false); // disable down button
			bottomButton->SetEnabled(false); // disable bottom button
			leftButton->SetEnabled(false); // disable left button
			rightButton->SetEnabled(true); // enable right button
			if(availableThoughtListView->CurrentSelection(0) >= 0)
			{
				orderedThoughtListView->DeselectAll();
			}
			//orderedThoughtListView->Deselect(orderedThoughtListView->CurrentSelection()); // deselect from this list when other list is active
			selected = availableThoughtListView->CurrentSelection(); // selected list item value
			if(selected >= 0) // if something is selected
			{
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(availableThoughtListView->ItemAt(selected));
				builderTextView->SetText(item->ReturnText());
			}
			break;
		case DISPLAY_ORDER_TEXT: // display preview text from item id
			leftButton->SetEnabled(true); // disable left button
			rightButton->SetEnabled(false); // enable right button
			if(orderedThoughtListView->CurrentSelection(0) >= 0)
			{
				availableThoughtListView->DeselectAll();
			}
			//availableThoughtListView->Deselect(availableThoughtListView->CurrentSelection()); // deselect from this list when other list is active
			selected = orderedThoughtListView->CurrentSelection(); // selected list item value
			if(selected == 0) // if its top item
			{
				topButton->SetEnabled(false); // disable top button
				upButton->SetEnabled(false); // disable the up button
				downButton->SetEnabled(true); // disable down button
				bottomButton->SetEnabled(true); // disable bottom button
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(orderedThoughtListView->ItemAt(selected));
				builderTextView->SetText(item->ReturnText());
			}
			if(selected > 0 && selected == (orderedThoughtListView->CountItems() - 1)) // if something is selected
			{
				topButton->SetEnabled(true); // enable top button
				upButton->SetEnabled(true); // enable up button
				downButton->SetEnabled(false); // disable down button
				bottomButton->SetEnabled(false); // disable bottom button
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(orderedThoughtListView->ItemAt(selected));
				builderTextView->SetText(item->ReturnText());
			}
			if(selected > 0 && selected < (orderedThoughtListView->CountItems() - 1))
			{
				topButton->SetEnabled(true); // enable top button
				upButton->SetEnabled(true); // enable up button
				downButton->SetEnabled(true); // disable down button
				bottomButton->SetEnabled(true); // disable bottom button
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(orderedThoughtListView->ItemAt(selected));
				builderTextView->SetText(item->ReturnText());
			}
			break;
		case ORDER_THOUGHT_EDITOR:
			selected = orderedThoughtListView->CurrentSelection(); // list item value
			if(selected < 0) // if selected nothing, open empty builder window
			{
				tmpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Editor - untitled", -1);
				tmpEditor->Show();
				this->Hide();
			}
			else // you selected an actual existing thought from listview
			{
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(orderedThoughtListView->ItemAt(selected));
				BString tmpText;
				tmpText = "MasterPiece Editor - ";
				tmpText += item->Text();
				tmpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), tmpText, item->ReturnID());
				tmpEditor->Show();
				this->Hide();
			}
			break;
		case AVAIL_THOUGHT_EDITOR:
			selected = availableThoughtListView->CurrentSelection(); // list item value
			if(selected < 0) // if selected nothing, open empty builder window
			{
				tmpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Editor - untitled", -1);
				tmpEditor->Show();
				this->Hide();
			}
			else // you selected an actual existing thought from listview
			{
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(availableThoughtListView->ItemAt(selected));
				BString tmpText;
				tmpText = "MasterPiece Editor - ";
				tmpText += item->Text();
				tmpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), tmpText, item->ReturnID());
				tmpEditor->Show();
				this->Hide();
			}
			break;
		case SHOW_LAUNCHER:
			if(msg->FindInt64("showLauncher", &showLauncher) == B_OK) // message was found
			{
				if(showLauncher == 1)
				{
					PopulateBuilderListViews(); // rerun the sql and repopulate the 2 listviews with any updated values
					if(this->IsHidden())
					{
						this->Show();
					}
				}
				else if(showLauncher == 0)
				{
					this->Hide();
				}
				else
				{
					eAlert = new ErrorAlert("4.4 Builder Error: Illegal Value was returned.");
					eAlert->Launch();
					// big error must display 
				}
			}
			else // message not found
			{
				eAlert = new ErrorAlert("4.5 Builder Error: Message Variable was not found.");
				eAlert->Launch();
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
	sqlValue = sqlite3_prepare_v2(mpdb, "select ideaname, ideaid, ideatext from ideatable where ismp = 0 and mpid is null", -1, &ideaStatement, NULL);
	if(sqlValue == SQLITE_OK) // sql statement was prepared
	{
		while(sqlite3_step(ideaStatement) == SQLITE_ROW) // step through the sql return values
		{
			availableThoughtListView->AddItem(new IdeaStringItem(sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 0)), sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 2)),sqlite3_column_int(ideaStatement, 1)));
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
		// populate the ordered list items from here with the information from passed id...
		sqlValue = sqlite3_prepare_v2(mpdb, "select ideaname, ideatext, ismp, mpid, ordernumber, ideaid from ideatable where ismp=0 and mpid=? order by ordernumber", -1, &ideaStatement, NULL);
		if(sqlValue == SQLITE_OK) // sql statement was prepared
		{
			if(sqlite3_bind_int(ideaStatement, 1, currentideaID) == SQLITE_OK)
			{
				while(sqlite3_step(ideaStatement) == SQLITE_ROW) // step through the sql return values
				{
					orderedThoughtListView->AddItem(new IdeaStringItem(sqlite3_mprintf("%d. %s", sqlite3_column_int(ideaStatement, 4), sqlite3_column_text(ideaStatement, 0)), sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 1)), sqlite3_column_int(ideaStatement, 2), sqlite3_column_int(ideaStatement, 3), sqlite3_column_int(ideaStatement, 4), sqlite3_column_int(ideaStatement, 5)));
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
		sqlite3_finalize(ideaStatement); // finish with sql statement
	}
}
