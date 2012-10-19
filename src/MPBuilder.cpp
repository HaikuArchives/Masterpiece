#include "MPBuilder.h"

MPBuilder::MPBuilder(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID)
	:	BWindow(BRect(100, 100, 900, 700), windowTitle, B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE), launcherMessage(msg), launcherMessenger(msgr)
{
	// initialize controls
	AddShortcut(B_TAB, B_COMMAND_KEY, new BMessage(END_EDIT_VIEW));
	AddShortcut('q', B_COMMAND_KEY, new BMessage(B_QUIT_REQUESTED));
	AddShortcut('n', B_COMMAND_KEY, new BMessage(MENU_NEW_MP));
	AddShortcut('e', B_COMMAND_KEY, new BMessage(MENU_EDT_MP));
	AddShortcut('s', B_COMMAND_KEY, new BMessage(MENU_SAV_MP));
	AddShortcut('r', B_COMMAND_KEY, new BMessage(MENU_PRV_MP));
	AddShortcut('p', B_COMMAND_KEY, new BMessage(MENU_PUB_MP));
	AddShortcut('k', B_COMMAND_KEY, new BMessage(MENU_KEY_MP));
	AddShortcut(B_RIGHT_ARROW, B_COMMAND_KEY, new BMessage(MOVE_RIGHT));
	AddShortcut(B_LEFT_ARROW, B_COMMAND_KEY, new BMessage(MOVE_LEFT));
	AddShortcut(B_UP_ARROW, B_COMMAND_KEY, new BMessage(MOVE_UP));
	AddShortcut(B_DOWN_ARROW, B_COMMAND_KEY, new BMessage(MOVE_DOWN));
	AddShortcut(B_HOME, B_COMMAND_KEY, new BMessage(MOVE_TOP));
	AddShortcut(B_END, B_COMMAND_KEY, new BMessage(MOVE_BOTTOM));
	AddShortcut('d', B_COMMAND_KEY, new BMessage(DELETE_BUILDER_THT));
	publishPanel = NULL;
	BRect r = Bounds();
	r.bottom = 16;
	builderMenuBar = new BuilderMenu(r, BMessage(CLEAR_STATUS), BMessenger(this));
	r = Bounds();
	r.left += B_V_SCROLL_BAR_WIDTH;
	r.right = 225;
	r.top = 26;
	r.bottom = 46;
	availableStringView = new BStringView(r, NULL, "All Available Thoughts");
	r = Bounds();
	r.top = 26;
	r.left = (int)(Bounds().right - Bounds().left) / 2 + 75;
	r.right -= B_V_SCROLL_BAR_WIDTH;
	r.bottom = 46;
	orderedStringView = new BStringView(r, NULL, "Ordered Thoughts");
	r = Bounds();
	r.top = availableStringView->Frame().bottom + 10;
	r.left = availableStringView->Frame().left;
	r.bottom = (r.bottom - r.top) / 2 + 100;
	r.right = orderedStringView->Frame().left - 150;
	availableThoughtListView = new BuilderListView(r, BMessage(CLEAR_STATUS), BMessenger(this));
	r = Bounds();
	r.top = orderedStringView->Frame().bottom + 10;
	r.left = orderedStringView->Frame().left;
	r.bottom = (r.bottom - r.top) / 2 + 100;
	r.right -= B_V_SCROLL_BAR_WIDTH + 10;
	orderedThoughtListView = new BuilderListView(r, BMessage(CLEAR_STATUS), BMessenger(this));
	r = Bounds();
	r.left = availableThoughtListView->Frame().left;
	r.top = availableThoughtListView->Frame().bottom + 10;
	r.bottom = r.top + 20;
	r.right = availableStringView->Frame().right;
	quickStringView = new BStringView(r, NULL, "Quick Edit");
	r = Bounds();
	r.left = availableThoughtListView->Frame().right + 40;
	r.top = availableThoughtListView->Frame().top + 5;
	r.right = r.left + 80;
	r.bottom = r.top + 25;
	rightButton = new BuilderButton(r, ">", new BMessage(MOVE_RIGHT), BMessage(CLEAR_STATUS), BMessenger(this));
	r = Bounds();
	r.left = rightButton->Frame().left;
	r.top = rightButton->Frame().bottom + 5;
	r.right = r.left + 80;
	r.bottom = r.top + 25;
	leftButton = new BuilderButton(r, "<", new BMessage(MOVE_LEFT), BMessage(CLEAR_STATUS), BMessenger(this));
	r = Bounds();
	r.left = leftButton->Frame().left;
	r.top = leftButton->Frame().bottom + 5;
	r.right = r.left + 80;
	r.bottom = r.top + 25;
	topButton = new BuilderButton(r, "TOP", new BMessage(MOVE_TOP), BMessage(CLEAR_STATUS), BMessenger(this));
	r = Bounds();
	r.left = topButton->Frame().left;
	r.top = topButton->Frame().bottom + 5;
	r.right = r.left + 80;
	r.bottom = r.top + 25;
	upButton = new BuilderButton(r, "UP", new BMessage(MOVE_UP), BMessage(CLEAR_STATUS), BMessenger(this));
	r = Bounds();
	r.left = upButton->Frame().left;
	r.top = upButton->Frame().bottom + 5;
	r.right = r.left + 80;
	r.bottom = r.top + 25;
	downButton = new BuilderButton(r, "DOWN", new BMessage(MOVE_DOWN), BMessage(CLEAR_STATUS), BMessenger(this));
	r = Bounds();
	r.left = downButton->Frame().left;
	r.top = downButton->Frame().bottom + 5;
	r.right = r.left + 80;
	r.bottom = r.top + 25;
	bottomButton = new BuilderButton(r, "BOTTOM", new BMessage(MOVE_BOTTOM), BMessage(CLEAR_STATUS), BMessenger(this));
	r = Bounds();
	r.left = bottomButton->Frame().left;
	r.top = bottomButton->Frame().bottom + 5;
	r.right = r.left + 80;
	r.bottom = r.top + 25;
	deleteButton = new BuilderButton(r, "DELETE", new BMessage(DELETE_BUILDER_THT), BMessage(CLEAR_STATUS), BMessenger(this));
	r = Bounds();
	r.top = builderMenuBar->Frame().bottom + 1;
	r.right -= B_V_SCROLL_BAR_WIDTH;
	r.bottom -= B_H_SCROLL_BAR_HEIGHT;
	r.top = r.bottom + 1;
	r.bottom = Bounds().bottom;
	builderStatusBar = new BStringView(r, "statusbar", NULL, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM);
	builderStatusBar->SetFontSize(10.0);
	r = Bounds();
	r.left = quickStringView->Frame().left;
	r.top = quickStringView->Frame().bottom + 10;
	r.bottom = builderStatusBar->Frame().top - 1;
	r.right -= B_V_SCROLL_BAR_WIDTH + 10;
	//builderTextView = new BTextView(r, NULL, BRect(0, 0, r.right-10, 100), B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);
	builderTextView = new EditorTextView(r, NULL, BRect(0, 0, r.right-10, 100), B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE, BMessage(CLEAR_STATUS), BMessenger(this));
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	backView->AddChild(builderMenuBar);
	backView->AddChild(availableStringView);
	backView->AddChild(orderedStringView);
	backView->AddChild(new BScrollView("scroll_available", availableThoughtListView, B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, false, true, B_FANCY_BORDER));
	backView->AddChild(new BScrollView("scroll_ordered", orderedThoughtListView, B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, false, true, B_FANCY_BORDER));
	backView->AddChild(quickStringView);
	backView->AddChild(new BScrollView("scroll_editor", builderTextView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER));
	backView->AddChild(rightButton);
	backView->AddChild(leftButton);
	backView->AddChild(topButton);
	backView->AddChild(upButton);
	backView->AddChild(downButton);
	backView->AddChild(bottomButton);
	backView->AddChild(deleteButton);
	backView->AddChild(builderStatusBar);
	deleteButton->SetEnabled(false);
	/*
	// gui layout builder
	backView->SetLayout(new BGroupLayout(B_HORIZONTAL, 0.0));
	backView->AddChild(BGridLayoutBuilder()
		.Add(builderMenuBar, 0, 0, 10, 1)
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
		.Add(new BStringView(BRect(10, 10, 200, 30), NULL, "Quick Edit"), 0, 12)
		.Add(new BScrollView("scroll_editor", builderTextView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 0, 13, 10, 5)
		.Add(builderStatusBar, 0, 19)
		.SetInsets(0, 0, 0, 0)
	);
	*/
	
	currentideaID = ideaID; // pass current idea id selected to builder window to use
	PopulateBuilderListViews();
	availableThoughtListView->SetSelectionMessage(new BMessage(DISPLAY_AVAIL_TEXT));
	availableThoughtListView->SetInvocationMessage(new BMessage(AVAIL_THOUGHT_EDITOR));
	orderedThoughtListView->SetSelectionMessage(new BMessage(DISPLAY_ORDER_TEXT));
	orderedThoughtListView->SetInvocationMessage(new BMessage(ORDER_THOUGHT_EDITOR));
}
MPBuilder::~MPBuilder()
{
}
void MPBuilder::MessageReceived(BMessage* msg)
{
	BRect r(Bounds());
	thread_id previewThread;
	thread_id publishThread;

	switch(msg->what)
	{
		case MENU_NEW_MP: // open new untitled thought
			tmpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "Masterpiece Editor - untitled", -1);
			tmpEditor->Show();
			break;
		case MENU_EDT_MP: // edit mp name
			xPos = (r.right - r.left) / 2; // find xpos for window
			yPos = (r.bottom - r.top) / 2; // find ypos for window
			editIdeaName = new EditIdeaName(BMessage(MP_UPDATE_TITLE), BMessenger(this), xPos, yPos, currentideaID);
			editIdeaName->Show();
			break;
		case MENU_SAV_MP: // save mp information
			// need to save a untitled mp.
			if(currentideaID == -1) // save the mp first.
			{
				sqlObject = new SqlObject(ideaStatement, "8");
				sqlObject->PrepareSql("insert into ideatable(ideaname, ismp) values('untitled', 1)");
				sqlObject->StepSql();
				xPos = (r.right - r.left) / 2; // find xpos for window
				yPos = (r.bottom - r.top) / 2; // find ypos for window
				saveIdea = new SaveIdea(BMessage(SAVE_TITLE), BMessenger(this), xPos, yPos, sqlObject->ReturnLastInsertRowID());
				currentideaID = sqlObject->ReturnLastInsertRowID();
				sqlObject->FinalizeSql();
				sqlObject->CloseSql();
				delete sqlObject;
				saveIdea->Show();
			}
			// save the mini editor window
			// need to get the current ideaid from the selected item...
			IdeaStringItem* saveItem;
			availorderBit = -1;
			if(availableThoughtListView->CurrentSelection() >= 0 && orderedThoughtListView->CurrentSelection() < 0)
			{
				selected = availableThoughtListView->CurrentSelection();
				saveItem = dynamic_cast<IdeaStringItem*>(availableThoughtListView->ItemAt(selected));
				availorderBit = 0;
			}
			if(orderedThoughtListView->CurrentSelection() >= 0 && availableThoughtListView->CurrentSelection() < 0)
			{
				selected = orderedThoughtListView->CurrentSelection();
				saveItem = dynamic_cast<IdeaStringItem*>(orderedThoughtListView->ItemAt(selected));
				availorderBit = 1;
			}
			if(selected >= 0)
			{
				sqlObject = new SqlObject(ideaStatement, "9");
				sqlObject->PrepareSql("update ideatable set ideatext = ? where ideaid = ?");
				sqlObject->BindValue(1, builderTextView->Text());
				sqlObject->BindValue(2, saveItem->ReturnID());
				sqlObject->StepSql();
				sqlObject->FinalizeSql();
				sqlObject->CloseSql();
				delete sqlObject;
				Lock();
				SetStatusBar("Thought Saved");
				Unlock();
				PopulateBuilderListViews();
				if(availorderBit == 0)
				{
					availableThoughtListView->Select(selected);
				}
				else if(availorderBit == 1)
				{
					orderedThoughtListView->Select(selected);
				}
				else
				{
					// No Big Deal cause a new MP was being saved.
					// error cause nothing was selected for this save to occur and be called.
					//eAlert = new ErrorAlert("4.15 Not an Item from Either List of Ideas.");
					//eAlert->Launch();
				}
			}
			else
			{
				eAlert = new ErrorAlert("4.14 No Selected Idea to Save.");
				eAlert->Launch();
			}
			break;
		case MENU_PRV_MP: // preview masterpiece
			previewThread = spawn_thread(PreviewThread, "preview thread", B_NORMAL_PRIORITY, (void*)this);
			if(previewThread >= 0) // successful
			{
				SetStatusBar("Generating Preview...");
				UpdateIfNeeded();
				resume_thread(previewThread);
			}
			break;
		case MENU_PUB_MP: // publish masterpiece
			if(!publishPanel)
			{
				publishPanel = new PublishFilePanel(new BMessenger(this));
			}
			publishPanel->Show();
			break;
		case PUBLISH_TYPE:
			fileExt = publishPanel->publishTypeMenu->FindMarked()->Label();
			fileExt = fileExt.ToLower();
			if(msg->FindString("name", &pubName) == B_OK)
			{
				// set's file name to pubName
			}
			else
			{
				eAlert = new ErrorAlert("2.1 Builder Error: Message not found");
				eAlert->Launch();
			}
			if(msg->FindRef("directory", &pubRef) == B_OK)
			{
			}
			else
			{
				eAlert = new ErrorAlert("2.1 Builder Error: Directory Reference not found");
				eAlert->Launch();
			}
			publishThread = spawn_thread(PublishThread, "publish thread", B_NORMAL_PRIORITY, (void*)this);
			if(publishThread >= 0) // successful
			{
				SetStatusBar("Publishing File...");
				UpdateIfNeeded();
				resume_thread(publishThread);
			}
			break;
		case MENU_HLP_MP: // help topics
			break;
		case MENU_KEY_MP: // keyboard reference
			xPos = (r.right - r.left) / 2; // find xpos for window
			yPos = (r.bottom - r.top) / 2; // find ypos for window
			helperWindow = new HelperWindows(BRect(xPos, yPos, xPos + 240, yPos + 190), "Keyboard Shortcuts");
			helperWindow->AddText(BRect(10, 10, 200, 25), "1", "Close the Window :: ALT + q");
			helperWindow->AddText(BRect(10, 35, 200, 50), "2", "New Thought :: ALT + n");
			helperWindow->AddText(BRect(10, 60, 200, 75), "3", "Edit Masterpiece Name :: ALT + e");
			helperWindow->AddText(BRect(10, 85, 200, 100), "4", "Save Progress :: ALT + s");
			helperWindow->AddText(BRect(10, 110, 200, 125), "5", "Preview Masterpiece :: ALT + r");
			helperWindow->AddText(BRect(10, 135, 200, 150), "6", "Publish Masterpiece :: ALT + p");
			helperWindow->AddText(BRect(10, 160, 230, 175), "7", "View Keyboard Shortcuts :: ALT + k");
			helperWindow->Show();
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
				delete sqlObject;
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
				delete sqlObject;
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
				// this is not an error worth passing.
				//eAlert = new ErrorAlert("4.7 Builder Error: No Idea is selected to move up.");
				//eAlert->Launch();
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
				// this is not an error worth passing.
				//eAlert = new ErrorAlert("4.9 Builder Error: No Idea is selected to move down.");
				//eAlert->Launch();
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
				// this is not an error worth passing
				//eAlert = new ErrorAlert("4.11 Builder Error: No Idea is selected to move to top.");
				//eAlert->Launch();
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
				// this is not an error worth passing
				//eAlert = new ErrorAlert("4.13 Builder Error: No Idea is selected to move to the bottom.");
				//eAlert->Launch();
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
			delete sqlObject;
			ReorderOrderedListView(); // reorder orderedlistview items for mp
			PopulateBuilderListViews(); // update listviews' items
			deleteButton->SetEnabled(false);
			break;
		case DISPLAY_AVAIL_TEXT: // display preview text from item id
			/*
			if(GetStatusBar() != "")
			{
				SetStatusBar("");
			}
			else
			{
				printf("Error\n");
			}
			*/
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
			if(selected >= 0)
			{
				IdeaStringItem* item;
				item = dynamic_cast<IdeaStringItem*>(orderedThoughtListView->ItemAt(selected));
				builderTextView->SetText(item->ReturnText());
				deleteButton->SetEnabled(true);
			}
			if(selected == 0) // if its top item
			{
				topButton->SetEnabled(false); // disable top button
				upButton->SetEnabled(false); // disable the up button
				downButton->SetEnabled(true); // disable down button
				bottomButton->SetEnabled(true); // disable bottom button
				leftButton->SetEnabled(true); // disable left button
				rightButton->SetEnabled(false); // enable right button
			}
			if(selected > 0 && selected == (orderedThoughtListView->CountItems() - 1)) // if something is selected
			{
				topButton->SetEnabled(true); // enable top button
				upButton->SetEnabled(true); // enable up button
				downButton->SetEnabled(false); // disable down button
				bottomButton->SetEnabled(false); // disable bottom button
				leftButton->SetEnabled(true); // disable left button
				rightButton->SetEnabled(false); // enable right button
			}
			if(selected > 0 && selected < (orderedThoughtListView->CountItems() - 1))
			{
				topButton->SetEnabled(true); // enable top button
				upButton->SetEnabled(true); // enable up button
				downButton->SetEnabled(true); // disable down button
				bottomButton->SetEnabled(true); // disable bottom button
				leftButton->SetEnabled(true); // disable left button
				rightButton->SetEnabled(false); // enable right button
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
		case MP_UPDATE_TITLE: // update title with the name from the editideawindow
			if(msg->FindString("updatetitle", &updateTitle) == B_OK) // updated title exists in variable
			{
				tmpString = "Masterpiece Builder - ";
				tmpString += updateTitle;
				this->SetTitle(tmpString);
				this->SetStatusBar("Name Change Saved");
			}
			else
			{
				eAlert = new ErrorAlert("4.2 Builder Error: Message not found"); // message variable not found
			}
			break;
		case SAVE_TITLE: // update title with the name from the editideawindow
			if(msg->FindString("saveidea", &saveTitle) == B_OK) // updated title exists in variable
			{
				tmpString = "Masterpiece Builder - ";
				tmpString += saveTitle;
				this->SetTitle(tmpString);
				this->SetStatusBar("Thought Saved");
			}
			else
			{
				eAlert = new ErrorAlert("4.2 Builder Error: Message not found"); // message variable not found
			}
			break;
		case END_EDIT_VIEW:
			printf("give focus to another element\n");
			availableThoughtListView->MakeFocus(true);
			break;
		case CLEAR_STATUS:
			if(msg->FindInt64("clearStatus", &clearStatus) == B_OK)
			{
				if(GetStatusBar() != "") SetStatusBar("");
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
	delete sqlObject;
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
		delete sqlObject;
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
	delete sqlObject2;
	delete sqlObject;
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
	delete sqlObject;
	ReorderOrderedListView(); // reorder orderedlistview items for mp
	PopulateBuilderListViews(); // update listviews' items
	orderedThoughtListView->Select(newOrderNumber); // highlight the newly moved item
}
void MPBuilder::SetStatusBar(const char* string)
{
	builderStatusBar->SetText(string);
}
const char* MPBuilder::GetStatusBar(void)
{
	return builderStatusBar->Text();
}
int32 MPBuilder::PreviewThread(void* data)
{
	MPBuilder* parent = (MPBuilder*)data;
	IdeaStringItem* previewItem;
	parent->mpData = "";
	for(int32 i = 0; i < parent->orderedThoughtListView->CountItems(); i++)
	{
		previewItem = dynamic_cast<IdeaStringItem*>(parent->orderedThoughtListView->ItemAt(i));
		parent->mpData += previewItem->ReturnText();
	}
	ExecutePreview(parent->mpData);
	
	parent->Lock();
	parent->SetStatusBar("Preview Completed Successfully");
	parent->Unlock();
	
	return 0;
}
int32 MPBuilder::PublishThread(void* data)
{
	MPBuilder* parent = (MPBuilder*)data;
	
	// write all the data to a file...
	IdeaStringItem* publishItem;
	parent->mpData = "";
	for(int32 i = 0; i < parent->orderedThoughtListView->CountItems(); i++)
	{
		publishItem = dynamic_cast<IdeaStringItem*>(parent->orderedThoughtListView->ItemAt(i));
		parent->mpData += publishItem->ReturnText();
	}
	
	ExecutePublish(parent->mpData, parent->fileExt, parent->pubRef, parent->pubName);
	
	parent->Lock();
	parent->SetStatusBar("Publish Completed Successfully");
	parent->Unlock();
	
	return 0;
}
