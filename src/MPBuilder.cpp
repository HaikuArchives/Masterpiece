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
	
	//mpdb = OpenSqliteDB(); // open mpdb db
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
								if(sqlite3_step(ideaStatement) == SQLITE_DONE) // execute the update statement
								{
								}
								else
								{
									eAlert = new ErrorAlert("1.37 Sql Error: Move Right Update Execution Failed.");
									eAlert->Launch();
								}
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
					eAlert = new ErrorAlert("1.25 Sql Error: Move Right Update Prepare Failed");
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
				/*
				sqlObject = new SqlObject(mpdb, ideaStatement, "38");
				sqlObject->PrepareSql("update ideatable set mpid=NULL, ordernumber=NULL where ideaid=?");
				sqlObject->BindValue(1, item->ReturnID());
				sqlObject->StepSql();
				sqlObject->FinalizeSql();
				//sqlite3_finalize(ideaStatement); // finish with sql statement
				*/
				ReorderOrderedListView(); // reorder orderedlistview items for mp
				PopulateBuilderListViews(); // update listviews' items
			}
			break;
		case MOVE_UP: // move idea up 1 in ordered list
			selected = orderedThoughtListView->CurrentSelection(); // selected list item value
			if(selected >= 1) // if something is selected that is not at top
			{
				ModifyOrderedItems(selected, (selected - 1)); // sql reorder of items
			}
			else if(selected == 0) // this item is already at the top of the list
			{
				eAlert = new ErrorAlert("4.6 Builder Error: Idea is already at the top of the list.");
				eAlert->Launch();
			}
			else // no item was selected
			{
				eAlert = new ErrorAlert("4.7 Builder Error: No Idea is selected to move up.");
				eAlert->Launch();
			}
			break;
		case MOVE_DOWN:
			selected = orderedThoughtListView->CurrentSelection(); // selected list item value
			if(selected >= 0 && selected < (orderedThoughtListView->CountItems() - 1)) // if something is selected that is not at bottom
			{
				ModifyOrderedItems(selected, (selected + 1)); // sql reorder of items
			}
			else if(selected == (orderedThoughtListView->CountItems() - 1)) // this item is already at the bottom of the list
			{
				eAlert = new ErrorAlert("4.8 Builder Error: Idea is already at the bottom of the list.");
				eAlert->Launch();
			}
			else // no item was selected
			{
				eAlert = new ErrorAlert("4.9 Builder Error: No Idea is selected to move down.");
				eAlert->Launch();
			}
			break;
		case MOVE_TOP:
			selected = orderedThoughtListView->CurrentSelection(); // selected list item value
			if(selected >= 1) // if something is selected that is not at top
			{
				ModifyOrderedItems(selected, 0); // sql reorder of items
			}
			else if(selected == 0) // this item is already at the top of the list
			{
				eAlert = new ErrorAlert("4.10 Builder Error: Idea is already at the top of the list.");
				eAlert->Launch();
			}
			else // no item was selected
			{
				eAlert = new ErrorAlert("4.11 Builder Error: No Idea is selected to move to top.");
				eAlert->Launch();
			}
			break;
		case MOVE_BOTTOM:
			selected = orderedThoughtListView->CurrentSelection(); // selected list item value
			if(selected >= 0 && selected < (orderedThoughtListView->CountItems() - 1)) // if something is selected that is not at bottom
			{
				ModifyOrderedItems(selected, (orderedThoughtListView->CountItems() - 1)); // sql move to bottom
			}
			else if(selected == (orderedThoughtListView->CountItems() - 1)) // this item is already at bottom
			{
				eAlert = new ErrorAlert("4.12 Builder Error: Idea is already at the bottom of the list.");
				eAlert->Launch();
			}
			else // no item was selected
			{
				eAlert = new ErrorAlert("4.13 Builder Error: No Idea is selected to move to the bottom.");
				eAlert->Launch();
			}
		case DISPLAY_AVAIL_TEXT: // display preview text from item id
			if(availableThoughtListView->CurrentSelection() >= 0)
			{
				orderedThoughtListView->DeselectAll();
			}
			selected = availableThoughtListView->CurrentSelection(); // selected list item value
			if(selected >= 0) // if something is selected
			{
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(availableThoughtListView->ItemAt(selected));
				builderTextView->SetText(item->ReturnText());
				topButton->SetEnabled(false); // disable top button
				upButton->SetEnabled(false); // disable the up button
				downButton->SetEnabled(false); // disable down button
				bottomButton->SetEnabled(false); // disable bottom button
				leftButton->SetEnabled(false); // disable left button
				rightButton->SetEnabled(true); // enable right button
			}
			break;
		case DISPLAY_ORDER_TEXT: // display preview text from item id
			if(orderedThoughtListView->CurrentSelection() >= 0)
			{
				availableThoughtListView->DeselectAll();
			}
			selected = orderedThoughtListView->CurrentSelection(); // selected list item value
			if(selected == 0) // if its top item
			{
				topButton->SetEnabled(false); // disable top button
				upButton->SetEnabled(false); // disable the up button
				downButton->SetEnabled(true); // disable down button
				bottomButton->SetEnabled(true); // disable bottom button
				leftButton->SetEnabled(true); // disable left button
				rightButton->SetEnabled(false); // enable right button
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
				leftButton->SetEnabled(true); // disable left button
				rightButton->SetEnabled(false); // enable right button
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
				leftButton->SetEnabled(true); // disable left button
				rightButton->SetEnabled(false); // enable right button
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
void MPBuilder::ReorderOrderedListView(void)
{
	int a = 1;
	sqlValue = sqlite3_prepare_v2(mpdb, "select ideaid from ideatable where ismp=0 and mpid=? order by ordernumber", -1, &ideaStatement, NULL);
	if(sqlValue == SQLITE_OK) // sql statement was prepared
	{
		if(sqlite3_bind_int(ideaStatement, 1, currentideaID) == SQLITE_OK) // bind was successful
		{
			while(sqlite3_step(ideaStatement) == SQLITE_ROW) // step through the sql return values
			{
				sqlite3_prepare_v2(mpdb, "update ideatable set ordernumber=? where ideaid=?", -1, &reorderStatement, NULL);
				if(sqlValue == SQLITE_OK) // sql statement was prepared
				{
					if(sqlite3_bind_int(reorderStatement, 1, a) == SQLITE_OK) // bind was successful
					{
						if(sqlite3_bind_int(reorderStatement, 2, sqlite3_column_int(ideaStatement, 0)) == SQLITE_OK) // bind was successful
						{
							if(sqlite3_step(reorderStatement) == SQLITE_DONE) // execute the update statement was successful
							{
								sqlite3_reset(reorderStatement);
								a++;
							}
							else // sql update failed
							{
								eAlert = new ErrorAlert("1.36 Sql Error: OrderNumber Update Failed");
								eAlert->Launch();
							}
						}
						else // sql bind failed
						{
							eAlert = new ErrorAlert("1.35 Sql Error: No Idea ID Bound.");
							eAlert->Launch();
						}
					}
					else // sql bind failed
					{
						eAlert = new ErrorAlert("1.34 Sql Error: No Order Integer Bound.");
						eAlert->Launch();
					}
				}
				else // sql update was not prepared
				{
					eAlert = new ErrorAlert("1.33 Sql Error: OrderNumber Update did not prepare.");
					eAlert->Launch();
				}
			}
		}
		else // sql bind failed
		{
			eAlert = new ErrorAlert("1.32 Sql Error: No Masterpiece ID Bound.");
			eAlert->Launch();
		}
	}
	else // sql select failed
	{
		eAlert = new ErrorAlert("1.31 Sql Error: No Ordered Thoughts Exist.  Please Add Some First.");
		eAlert->Launch();
	}
	sqlite3_finalize(ideaStatement); // finish with sql statement
	sqlite3_finalize(reorderStatement); // finish with sql statement
}
void MPBuilder::ModifyOrderedItems(int curOrderNumber, int newOrderNumber)
{
	IdeaStringItem* item;
	IdeaStringItem* item2;
	item = dynamic_cast<IdeaStringItem*>(orderedThoughtListView->ItemAt(curOrderNumber));
	if(newOrderNumber == 0)
	{
		item2 = dynamic_cast<IdeaStringItem*>(orderedThoughtListView->FirstItem());
	}
	else if(newOrderNumber == (orderedThoughtListView->CountItems() - 1))
	{
		item2 = dynamic_cast<IdeaStringItem*>(orderedThoughtListView->LastItem());
	}
	else
	{
		item2 = dynamic_cast<IdeaStringItem*>(orderedThoughtListView->ItemAt(newOrderNumber));
	}
	sqlValue = sqlite3_prepare_v2(mpdb, "update ideatable set ordernumber=? where ideaid=?", -1, &ideaStatement, NULL);
	if(sqlValue == SQLITE_OK) // sql statement was prepared
	{
		if(sqlite3_bind_int(ideaStatement, 1, item2->ReturnOrderNumber()) == SQLITE_OK) // sql bind successful
		{
			if(sqlite3_bind_int(ideaStatement, 2, item->ReturnID()) == SQLITE_OK) // sql bind successful
			{
				if(sqlite3_step(ideaStatement) == SQLITE_DONE) // execute the update statement succesfully
				{
					sqlite3_reset(ideaStatement); // reset ideastatement for new bindings
					if(newOrderNumber == 0)
					{
						bindValue = sqlite3_bind_int(ideaStatement, 1, (item2->ReturnOrderNumber()+1)); // move next to first
					}
					else if(newOrderNumber == (orderedThoughtListView->CountItems() - 1))
					{
						bindValue = sqlite3_bind_int(ideaStatement, 1, (item2->ReturnOrderNumber()-1)); // move next to last
					}
					else
					{
						bindValue = sqlite3_bind_int(ideaStatement, 1, item->ReturnOrderNumber()); // swap with item
					}
					if(bindValue == SQLITE_OK) // sql bind successful
					{
						if(sqlite3_bind_int(ideaStatement, 2, item2->ReturnID()) == SQLITE_OK) // sql bind successful
						{
							if(sqlite3_step(ideaStatement) == SQLITE_DONE) // execute update succesfully
							{
							}
							else // sql update failed
							{
								eAlert = new ErrorAlert("1.40 Sql Error: Move Down 2 Update Execution Failed");
								eAlert->Launch();
							}
						}
						else // sql bind 2 failed
						{
							eAlert = new ErrorAlert("1.41 Sql Error: Move Down 2 Bind 2 Failed.");
							eAlert->Launch();
						}
					}
					else // sql bind 1 failed
					{
						eAlert = new ErrorAlert("1.42 Sql Error: Move Down 2 Bind 1 Failed");
						eAlert->Launch();
					}
				}
				else // sql update failed
				{
					eAlert = new ErrorAlert("1.43 Sql Error: Move Down 1 Update Execution Failed.");
					eAlert->Launch();
				}
			}
			else // sql bind 2 failed
			{
				eAlert = new ErrorAlert("1.44 Sql Error: Move Down 1 Bind 2 Failed");
				eAlert->Launch();
			}
		}
		else // sql bind 1 failed
		{
			eAlert = new ErrorAlert("1.45 Sql Error: Move Down 1 Bind 1 Failed.");
			eAlert->Launch();
		}
	}
	else // sql update failed
	{
		eAlert = new ErrorAlert("1.52 Sql Error: Move Down 1 Update Prepare Failed");
		eAlert->Launch();
	}
	sqlite3_finalize(ideaStatement); // finalize the sql statement
	ReorderOrderedListView(); // reorder orderedlistview items for mp
	PopulateBuilderListViews(); // update listviews' items
	orderedThoughtListView->Select(newOrderNumber); // highlight the newly moved item
}
