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
	deleteButton = new BButton(BRect(10, 10, 90, 35), NULL, "DELETE", new BMessage(DELETE_BUILDER_THT), B_FOLLOW_NONE, B_WILL_DRAW);
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	deleteButton->SetEnabled(false);
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
		.Add(deleteButton, 4, 10)
		.Add(new BScrollView("scroll_ordered", orderedThoughtListView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 5, 2, 5, 10)
		.Add(new BScrollView("scroll_editor", builderTextView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 0, 12, 10, 5)
		.SetInsets(0, 0, 0, 0)
	);
	
	currentideaID = ideaID; // pass current idea id selected to builder window to use
	PopulateBuilderListViews();
	availableThoughtListView->SetSelectionMessage(new BMessage(DISPLAY_AVAIL_TEXT));
	availableThoughtListView->SetInvocationMessage(new BMessage(AVAIL_THOUGHT_EDITOR));
	orderedThoughtListView->SetSelectionMessage(new BMessage(DISPLAY_ORDER_TEXT));
	orderedThoughtListView->SetInvocationMessage(new BMessage(ORDER_THOUGHT_EDITOR));
}
void MPBuilder::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case MENU_NEW_MP: // open new untitled thought
			tmpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "Masterpiece Editor - untitled", -1);
			tmpEditor->Show();
			break;
		case MENU_EDT_MP: // edit mp name
			// work on mp edit name next
			break;
		case MENU_SAV_MP: // save mp information
			break;
		case MENU_PRV_MP: // preview masterpiece
			break;
		case MENU_PUB_MP: // publish masterpiece
			break;
		case MENU_HLP_MP: // help topics
			break;
		case MENU_KEY_MP: // keyboard reference
			break;
		case MENU_ABT_MP: // about window
			break;
		case MOVE_RIGHT: // add item to ordered list
			selected = availableThoughtListView->CurrentSelection(); // selected list item value
			if(selected >= 0) // if something is selected
			{
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(availableThoughtListView->ItemAt(selected));
				int totalItems = orderedThoughtListView->CountItems() + 1;
				sqlObject = new SqlObject(ideaStatement, "28"); // open sqldb
				sqlObject->PrepareSql("update ideatable set mpid=?, ordernumber=? where ideaid=?"); // prepare statement
				sqlObject->BindValue(1, currentideaID); // bind
				sqlObject->BindValue(2, totalItems); // bind
				sqlObject->BindValue(3, item->ReturnID()); // bind
				sqlObject->StepSql(); // execute update statement
				sqlObject->FinalizeSql(); // finalize sql
				sqlObject->CloseSql(); // close sql
				PopulateBuilderListViews(); // update listviews' items
			}
			break;
		case MOVE_LEFT: // remove item from ordered list of current mp.
			selected = orderedThoughtListView->CurrentSelection(); // selected list item value
			if(selected >= 0) // if something is selected
			{
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(orderedThoughtListView->ItemAt(selected));
				sqlObject = new SqlObject(ideaStatement, "38"); // opendb
				sqlObject->PrepareSql("update ideatable set mpid=NULL, ordernumber=NULL where ideaid=?"); // prepare
				sqlObject->BindValue(1, item->ReturnID()); // bind
				sqlObject->StepSql(); // execute update
				sqlObject->FinalizeSql(); // finalize
				sqlObject->CloseSql(); // close
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
			break;
		case DELETE_BUILDER_THT:
			IdeaStringItem* deleteItem;
			if(availableThoughtListView->CurrentSelection() >= 0 && orderedThoughtListView->CurrentSelection() < 0)
			{
				selected = availableThoughtListView->CurrentSelection();
				deleteItem = dynamic_cast<IdeaStringItem*>(availableThoughtListView->ItemAt(selected));
			}
			if(orderedThoughtListView->CurrentSelection() >= 0 && availableThoughtListView->CurrentSelection() < 0)
			{
				selected = orderedThoughtListView->CurrentSelection();
				deleteItem = dynamic_cast<IdeaStringItem*>(orderedThoughtListView->ItemAt(selected));
			}
			sqlObject = new SqlObject(ideaStatement, "31");
			sqlObject->PrepareSql("delete from ideatable where ideaid = ?");
			sqlObject->BindValue(1, deleteItem->ReturnID());
			sqlObject->StepSql();
			sqlObject->FinalizeSql();
			sqlObject->CloseSql();
			ReorderOrderedListView(); // reorder orderedlistview items for mp
			PopulateBuilderListViews(); // update listviews' items
			deleteButton->SetEnabled(false);
			break;
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
				deleteButton->SetEnabled(true);
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
			deleteButton->SetEnabled(true);
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
	sqlObject = new SqlObject(ideaStatement, "30");
	sqlObject->PrepareSql("select ideaname, ideaid, ideatext from ideatable where ismp = 0 and mpid is null");
	while(sqlObject->StepSql() == SQLITE_ROW)
	{
		availableThoughtListView->AddItem(new IdeaStringItem(sqlObject->ReturnText(0), sqlObject->ReturnText(2), sqlObject->ReturnInt(1)));
	}
	sqlObject->FinalizeSql();
	sqlObject->CloseSql();
	if(currentideaID != -1) // if id has a real value...
	{
		sqlObject = new SqlObject(ideaStatement, "23");
		sqlObject->PrepareSql("select ideaname, ideatext, ismp, mpid, ordernumber, ideaid from ideatable where ismp=0 and mpid=? order by ordernumber");
		sqlObject->BindValue(1, currentideaID);
		while(sqlObject->StepSql() == SQLITE_ROW)
		{
			BString tmperString = sqlite3_mprintf("%d. %s", sqlObject->ReturnInt(4), sqlObject->ReturnText(0));
			orderedThoughtListView->AddItem(new IdeaStringItem(tmperString, sqlObject->ReturnText(1), sqlObject->ReturnInt(2), sqlObject->ReturnInt(3), sqlObject->ReturnInt(4), sqlObject->ReturnInt(5)));
		}
		sqlObject->FinalizeSql();
		sqlObject->CloseSql();
	}
}
void MPBuilder::ReorderOrderedListView(void)
{
	int a = 1;
	sqlObject = new SqlObject(ideaStatement, "33");
	sqlObject->PrepareSql("select ideaid from ideatable where ismp=0 and mpid=? order by ordernumber");
	sqlObject->BindValue(1, currentideaID);
	while(sqlObject->StepSql() == SQLITE_ROW)
	{
		sqlObject2 = new SqlObject(reorderStatement, "34", sqlObject->ReturnSqlDB());
		sqlObject2->PrepareSql("update ideatable set ordernumber=? where ideaid=?");
		sqlObject2->BindValue(1, a);
		sqlObject2->BindValue(2, sqlObject->ReturnInt(0));
		sqlObject2->StepSql();
		sqlObject2->ResetSql();
		sqlObject2->ClearBindings();
		a++;
	}
	sqlObject2->FinalizeSql();
	sqlObject->FinalizeSql();
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
	sqlObject = new SqlObject(ideaStatement, "40");
	sqlObject->PrepareSql("update ideatable set ordernumber=? where ideaid=?");
	sqlObject->BindValue(1, item2->ReturnOrderNumber());
	sqlObject->BindValue(2, item->ReturnID());
	sqlObject->StepSql();
	sqlObject->ResetSql();
	sqlObject->ClearBindings();
	if(newOrderNumber == 0)
	{
		sqlObject->BindValue(1, (item2->ReturnOrderNumber() + 1));
	}
	else if(newOrderNumber == (orderedThoughtListView->CountItems() - 1))
	{
		sqlObject->BindValue(1, (item2->ReturnOrderNumber() - 1));
	}
	else
	{
		sqlObject->BindValue(1, item->ReturnOrderNumber());
	}
	sqlObject->BindValue(2, item2->ReturnID());
	sqlObject->StepSql();
	sqlObject->FinalizeSql();
	sqlObject->CloseSql();
	ReorderOrderedListView(); // reorder orderedlistview items for mp
	PopulateBuilderListViews(); // update listviews' items
	orderedThoughtListView->Select(newOrderNumber); // highlight the newly moved item
}
