#include "MPEditor.h"

MPEditor::MPEditor(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID)
	:	BWindow(BRect(100, 100, 900, 700), windowTitle, B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE), launcherMessage(msg), launcherMessenger(msgr)
{
	AddShortcut('q', B_COMMAND_KEY, new BMessage(B_QUIT_REQUESTED));
	AddShortcut('n', B_COMMAND_KEY, new BMessage(MENU_NEW_THT));
	AddShortcut('e', B_COMMAND_KEY, new BMessage(MENU_EDT_THT));
	AddShortcut('s', B_COMMAND_KEY, new BMessage(MENU_SAV_THT));
	AddShortcut('r', B_COMMAND_KEY, new BMessage(MENU_PRV_THT));
	AddShortcut('p', B_COMMAND_KEY, new BMessage(MENU_PUB_THT));
	AddShortcut('k', B_COMMAND_KEY, new BMessage(MENU_KEY_THT));
	AddShortcut('a', B_COMMAND_KEY, new BMessage(MENU_ABT_THT));
	AddShortcut('x', B_COMMAND_KEY, new BMessage(MENU_EXP_THT));
	// initialize controls
	pubEditorPanel = NULL;
	exportPanel = NULL;
	BRect r = Bounds();
	r.bottom = 16;
	editorMenuBar = new MainMenu(r, "Edit Thought Name", BMessage(CLEAR_STATUS), BMessenger(this));
	r = Bounds();
	r.top = editorMenuBar->Frame().bottom + 1;
	r.right -= B_V_SCROLL_BAR_WIDTH;
	r.bottom -= B_H_SCROLL_BAR_HEIGHT;
	//r.bottom = r.bottom - 50;
	editorTextView = new EditorTextView(r, NULL, BRect(0, 5, r.right-10, r.bottom), B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE, BMessage(CLEAR_STATUS), BMessenger(this));
	r.top = r.bottom + 1;
	r.bottom = Bounds().bottom;
	editorStatusBar = new BStringView(r, "statusbar", NULL, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM);
	editorStatusBar->SetFontSize(10.0);
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	backView->AddChild(editorMenuBar);
	backView->AddChild(new BScrollView("scroll_editor", editorTextView, B_FOLLOW_ALL, 0, false, true, B_NO_BORDER));
	backView->AddChild(editorStatusBar);
	/*
	// gui layout builder
	backView->SetLayout(new BGroupLayout(B_HORIZONTAL, 0.0));
	backView->AddChild(BGridLayoutBuilder()
		.Add(editorMenuBar, 0, 0)
		.Add(new BScrollView("scroll_editor", editorTextView, B_FOLLOW_ALL, 0, false, true, B_FANCY_BORDER), 0, 1)
		.Add(editorStatusBar, 0, 2)
	);
	*/
	currentideaID = ideaID; // pass current idea id selected to editor window for use
	if(currentideaID != -1) // if id has a real value
	{
		sqlObject = new SqlObject(ideaStatement, "7");
		sqlObject->PrepareSql("select ideatext from ideatable where ideaid = ?");
		sqlObject->BindValue(1, currentideaID);
		sqlObject->StepSql();
		editorTextView->SetText(sqlObject->ReturnText(0));
		sqlObject->FinalizeSql();
		sqlObject->CloseSql();
		delete sqlObject;
	}
}
void MPEditor::MessageReceived(BMessage* msg)
{
	BRect r(Bounds());
	thread_id previewThread;
	thread_id publishThread;
	thread_id cheatThread;
	thread_id quickstartThread;
	thread_id rstThread;
	thread_id directThread;
	thread_id quickrefThread;
	thread_id qseThread;
	thread_id cheThread;
	thread_id mphelpThread;
	thread_id exportThread;
	status_t err;

	switch(msg->what)
	{
		case MENU_NEW_THT: // open another untitled editor window
			tmpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Editor - untitled", -1);
			tmpEditor->Show();
			break;
		case MENU_EDT_THT: // edit current idea name for editing
			xPos = (r.right - r.left) / 2; // find xpos for window
			yPos = (r.bottom - r.top) / 2; // find ypos for window
			editIdeaName = new EditIdeaName(BMessage(UPDATE_TITLE), BMessenger(this), xPos, yPos, currentideaID);
			editIdeaName->Show(); // show edit idea name window
			break;
		case MENU_SAV_THT: // save current idea progress
			if(currentideaID == -1) // if its untitled insert new thought, then show saveidea to apply a name...
			{
				sqlObject = new SqlObject(ideaStatement, "8");
				sqlObject->PrepareSql("insert into ideatable (ideaname, ideatext, ismp) values('untitled', ?, 0)");
				sqlObject->BindValue(1, editorTextView->Text());
				sqlObject->StepSql();
				xPos = (r.right - r.left) / 2; // find xpos for window
				yPos = (r.bottom - r.top) / 2; // find ypos for window
				saveIdea = new SaveIdea(BMessage(SAVE_TITLE), BMessenger(this), xPos, yPos, sqlObject->ReturnLastInsertRowID());
				currentideaID = sqlObject->ReturnLastInsertRowID();
				sqlObject->FinalizeSql();
				sqlObject->CloseSql();
				delete sqlObject;
				saveIdea->Show(); // show save window to name the untitled thought
			}
			else // already exists, just update ideatext and save new information
			{
				sqlObject = new SqlObject(ideaStatement, "9");
				sqlObject->PrepareSql("update ideatable set ideatext = ? where ideaid = ?");
				sqlObject->BindValue(1, editorTextView->Text());
				sqlObject->BindValue(2, currentideaID);
				sqlObject->StepSql();
				sqlObject->FinalizeSql();
				sqlObject->CloseSql();
				delete sqlObject;
				Lock();
				SetStatusBar("Thought Saved");
				Unlock();
			}
			break;
		case EXPORT_IDEA: // export idea
			printf("Export beginning\n");
			err = msg->FindRef("refs", 0, &exportref);
			if(err == B_OK)
			{
				exportThread = spawn_thread(ExportThread, "export thread", B_NORMAL_PRIORITY, (void*)this);
				if(exportThread >= 0) // successful
				{
					SetStatusBar("Exporting...");
					UpdateIfNeeded();
					resume_thread(exportThread);
				}
			}
			else
			{
				eAlert = new ErrorAlert("3.15 Editor Error: Directory Not Found");
				eAlert->Launch();
			}
			break;
		case MENU_EXP_THT: // export thought
			if(!exportPanel)
			{
				exportPanel = new BFilePanel(B_OPEN_PANEL, new BMessenger(this), NULL, B_DIRECTORY_NODE, false, new BMessage(EXPORT_IDEA), NULL, false, true);
			}
			exportPanel->Show();
			break;
		case MENU_PRV_THT: // preview thought in html in webpositive
			previewThread = spawn_thread(PreviewThread, "preview thread", B_NORMAL_PRIORITY, (void*)this);
			if(previewThread >= 0) // successful
			{
				SetStatusBar("Generating Preview...");
				UpdateIfNeeded();
				resume_thread(previewThread);
			}
			break;
		case MENU_PUB_THT: // publish thought by opening publish window
			if(!pubEditorPanel)
			{
				pubEditorPanel = new PublishFilePanel(new BMessenger(this));
			}
			pubEditorPanel->Show();
			break;
		case PUBLISH_TYPE:
			// write data to a file
			fileExt = pubEditorPanel->publishTypeMenu->FindMarked()->Label();
			fileExt = fileExt.ToLower();
			openflag = pubEditorPanel->openCheckBox->Value();
			if(msg->FindString("name", &pubName) == B_OK)
			{
				//printf("default save message: %s\n", pubName.String());
			}
			else
			{
				eAlert = new ErrorAlert("3.1 Editor Error: Message not found."); // message variable not found
				eAlert->Launch();
				//printf("no string name\n\n");
			}
			if(msg->FindRef("directory", &pubRef) == B_OK)
			{
			}
			else
			{
				eAlert = new ErrorAlert("3.1 Editor Error: Directory Reference not found."); // message variable not found
				eAlert->Launch();
				//printf("ref entry not found\n");
			}
			publishThread = spawn_thread(PublishThread, "publish thread", B_NORMAL_PRIORITY, (void*)this);
			if(publishThread >= 0) // successful
			{
				SetStatusBar("Publishing File...");
				UpdateIfNeeded();
				resume_thread(publishThread);
			}
			break;
		case MENU_KEY_THT: // open keyboard reference window
			xPos = (r.right - r.left) / 2; // find xpos for window
			yPos = (r.bottom - r.top) / 2; // find ypos for window
			helperWindow = new HelperWindows(BRect(xPos, yPos, xPos + 240, yPos + 190), "Keyboard Shortcuts");
			helperWindow->AddText(BRect(10, 10, 200, 25), "1", "Close the Window :: ALT + q");
			helperWindow->AddText(BRect(10, 35, 200, 50), "2", "New Thought :: ALT + n");
			helperWindow->AddText(BRect(10, 60, 200, 75), "3", "Edit Thought Name :: ALT + e");
			helperWindow->AddText(BRect(10, 85, 200, 100), "4", "Save Progress :: ALT + s");
			helperWindow->AddText(BRect(10, 110, 200, 125), "5", "Preview Thought :: ALT + r");
			helperWindow->AddText(BRect(10, 135, 200, 150), "6", "Publish Thought :: ALT + p");
			helperWindow->AddText(BRect(10, 160, 230, 175), "7", "View Keyboard Shortcuts :: ALT + k");
			helperWindow->Show();
			//printf("open keyboard reference window");
			break;
		case MENU_CHT_THT: // open markup reference window
			cheatThread = spawn_thread(HelpThread, "cheat thread", B_NORMAL_PRIORITY, (void*)"cheatsheet.html");
			if(cheatThread >= 0) // successful
			{
				resume_thread(cheatThread);
			}
			break;
		case MENU_QKS_THT: // quick start quide
			quickstartThread = spawn_thread(HelpThread, "qs thread", B_NORMAL_PRIORITY, (void*)"quickstart.html");
			if(quickstartThread >= 0) // successful
			{
				resume_thread(quickstartThread);
			}
			break;
		case MENU_RST_THT: // rst guide
			rstThread = spawn_thread(HelpThread, "rst thread", B_NORMAL_PRIORITY, (void*)"restructuredtext.html");
			if(rstThread >= 0) // successful
			{
				resume_thread(rstThread);
			}
			break;
		case MENU_DRT_THT: // directives guide
			directThread = spawn_thread(HelpThread, "direct thread", B_NORMAL_PRIORITY, (void*)"directives.html");
			if(directThread >= 0) // successful
			{
				resume_thread(directThread);
			}
			break;
		case MENU_QKE_THT: // quick start example
			qseThread = spawn_thread(HelpThread, "quick example thread", B_NORMAL_PRIORITY, (void*)"quickstart.txt");
			if(qseThread >= 0) // successful
			{
				resume_thread(qseThread);
			}
			break;
		case MENU_CHE_THT: // cheat sheet example
			cheThread = spawn_thread(HelpThread, "cheat example thread", B_NORMAL_PRIORITY, (void*)"cheatsheet.txt");
			if(cheThread >= 0) // successful
			{
				resume_thread(cheThread);
			}
			break;
		case MENU_QKR_THT: // quick ref guide
			quickrefThread = spawn_thread(HelpThread, "quick ref thread", B_NORMAL_PRIORITY, (void*)"quickref.html");
			if(quickrefThread >= 0) // successful
			{
				resume_thread(quickrefThread);
			}
			break;
		case MENU_ABT_THT: // open about window
			xPos = (r.right - r.left) / 2; // find xpos for window
			yPos = (r.bottom - r.top) / 2; // find ypos for window
			aboutWindow = new AboutWindow(BRect(xPos, yPos, xPos + 600, yPos + 400), "About MasterPiece");
			aboutWindow->Show();
			//printf("open about window");
			break;
		case MENU_MPH_THT: // mp tutorial
			mphelpThread = spawn_thread(HelpThread, "mphelp tutorial thread", B_NORMAL_PRIORITY, (void*)"masterpiece.html");
			if(mphelpThread >= 0) // successful
			{
				resume_thread(mphelpThread);
			}
			break;
		case UPDATE_TITLE: // update title with the name from the saveidea window
			if(msg->FindString("updatetitle", &updateTitle) == B_OK) // updated title exists in variable
			{
				tmpString = "Masterpiece Editor - ";
				tmpString += updateTitle;
				this->SetTitle(tmpString);
				this->SetStatusBar("Name Change Saved");
			}
			else // 
			{
				eAlert = new ErrorAlert("3.1 Editor Error: Message not found."); // message variable not found
				eAlert->Launch();
			}
			break;
		case SAVE_TITLE: // update title with the name from the saveidea window
			if(msg->FindString("saveidea", &saveTitle) == B_OK) // updated title exists in variable
			{
				tmpString = "Masterpiece Editor - ";
				tmpString += saveTitle;
				this->SetTitle(tmpString);
				this->SetStatusBar("Thought Saved");
			}
			else // 
			{
				eAlert = new ErrorAlert("3.21 Editor Error: Message not found."); // message variable not found
				eAlert->Launch();
			}
			break;
		case CLEAR_STATUS:
			if(msg->FindInt64("clearStatus", &clearStatus) == B_OK)
			{
				SetStatusBar("");
			}
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
	// on quit, show launcher by sending message
	launcherMessage.MakeEmpty();
	launcherMessage.AddInt64("showLauncher", 1);
	launcherMessenger.SendMessage(&launcherMessage);
	return true;
}
void MPEditor::SetStatusBar(const char* string)
{
	editorStatusBar->SetText(string);
}
const char* MPEditor::GetStatusBar(void)
{
	return editorStatusBar->Text();
}
int32 MPEditor::PreviewThread(void* data)
{
	MPEditor* parent = (MPEditor*)data;

	ExecutePreview(parent->editorTextView->Text());

	parent->Lock();
	parent->SetStatusBar("Preview Completed Successfully");
	parent->Unlock();
	
	return 0;
}
int32 MPEditor::PublishThread(void* data)
{
	MPEditor* parent = (MPEditor*)data;
	
	ExecutePublish(parent->editorTextView->Text(), parent->openflag, parent->fileExt, parent->pubRef, parent->pubName);

	parent->Lock();
	parent->SetStatusBar("Publish Completed Successfully");
	parent->Unlock();
	
	return 0;
}
int32 MPEditor::HelpThread(void* data)
{
	BString executeString;
	// take the data provided, which is the file, then call the system
	executeString = "open ";
	executeString += (const char*)data;
	executeString += " &";
	system(executeString);
	
	return 0;
}
int32 MPEditor::ExportThread(void* data)
{
	printf("Export in the middle\n");
	MPEditor* parent = (MPEditor*)data;
	parent->sqlObject = new SqlObject(parent->ideaStatement, "23");
	parent->sqlObject->PrepareSql("select ideaname from ideatable where ideaid = ?");
	parent->sqlObject->BindValue(1, parent->currentideaID);
	while(parent->sqlObject->StepSql() == SQLITE_ROW)
	{
		ExportIdea(parent->sqlObject->ReturnText(0), parent->editorTextView->Text(), parent->exportref);
	}
	parent->sqlObject->FinalizeSql();
	parent->sqlObject->CloseSql();
	delete parent->sqlObject;
	
	parent->Lock();
	parent->SetStatusBar("Export Completed Successfully");
	parent->Unlock();
	
	return 0;
}
