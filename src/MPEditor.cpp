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
	AddShortcut('m', B_COMMAND_KEY, new BMessage(MENU_MRK_THT));
	AddShortcut('a', B_COMMAND_KEY, new BMessage(MENU_ABT_THT));
	// initialize controls
	pubEditorPanel = NULL;
	BRect r = Bounds();
	r.bottom = 16;
	editorMenuBar = new EditorMenu(r);
	r = Bounds();
	r.top = editorMenuBar->Frame().bottom + 1;
	r.right -= B_V_SCROLL_BAR_WIDTH;
	r.bottom -= B_H_SCROLL_BAR_HEIGHT;
	//r.bottom = r.bottom - 50;
	editorTextView = new BTextView(r, NULL, r, B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);
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
	BString fileExt; // file extension of converted file
	thread_id previewThread;
	
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
				saveIdea = new SaveIdea(BMessage(UPDATE_TITLE), BMessenger(this), xPos, yPos, sqlObject->ReturnLastInsertRowID());
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
			}
			break;
		case MENU_PRV_THT: // preview thought in html in webpositive
			//SetStatusBar("Generating Preview...");
			/*
			childpid = fork();
			if(childpid >= 0) // fork worked
			{
				if(childpid == 0) // child process
				{
					ExecutePreview(editorTextView->Text());
					exit(0);
				}
				else // parent
				{
					wait(&childstatus);
					SetStatusBar("Preview Completed Successfully");
				}
			}
			else // fork failed with -1
			{
				// need to generate real error here.
				perror("fork");
			}
			*/
			previewThread = spawn_thread(PreviewThread, "preview thread", B_NORMAL_PRIORITY, (void*)this);
			if(previewThread >= 0) // successful
			{
				SetStatusBar("Start");
				UpdateIfNeeded();
				resume_thread(previewThread);
			}
			/*
			thread_id backupThread = spawn_thread(BackupThread,"project backup thread",
												B_NORMAL_PRIORITY, this);
			if (backupThread >= 0)
			{
				fStatusBar->SetText(TR("Backing up project"));
				UpdateIfNeeded();
				
				SetMenuLock(true);
				resume_thread(backupThread);
			}
			*/
			//ExecutePreview(editorTextView->Text());
			//SetStatusBar("Preview Completed Successfully");
			break;
		case MENU_PUB_THT: // publish thought by opening publish window
			if(!pubEditorPanel)
			{
				pubEditorPanel = new PublishFilePanel(new BMessenger(this));
			}
			pubEditorPanel->Show();
			SetStatusBar("Publishing File...");
			break;
		case PUBLISH_TYPE:
			// write data to a file
			fileExt = pubEditorPanel->publishTypeMenu->FindMarked()->Label();
			fileExt = fileExt.ToLower();
			ExecutePublish(msg, editorTextView->Text(), fileExt);
			SetStatusBar("Publish Completed Successfully");
			/*
			childpid = fork();
			if(childpid >= 0) // fork worked
			{
				if(childpid == 0) // child process
				{
					ExecutePublish(msg, editorTextView->Text(), fileExt);
					exit(0);
				}
				else // parent
				{
					wait(&childstatus);
					SetStatusBar("Publish Completed Successfully");
				}
			}
			else // fork failed with -1
			{
				// need to generate real error here
				perror("fork");
			}
			*/
			break;
		case MENU_HLP_THT: // open help topic window
			printf("open help topic window");
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
			printf("open keyboard reference window");
			break;
		case MENU_MRK_THT: // open markup reference window
			xPos = (r.right - r.left) / 2; // find xpos for window
			yPos = (r.bottom - r.top) / 2; // find ypos for window
			markupWindow = new MarkupWindow(BRect(xPos, yPos, xPos + 600, yPos + 400), "Markup Reference");
			markupWindow->AddMarkupItem("Cheat Sheet", "cheatsheet.txt");
			markupWindow->Show();
			printf("open markup reference window");
			break;
		case MENU_ABT_THT: // open about window
			xPos = (r.right - r.left) / 2; // find xpos for window
			yPos = (r.bottom - r.top) / 2; // find ypos for window
			aboutWindow = new AboutWindow(BRect(xPos, yPos, xPos + 600, yPos + 400), "About MasterPiece");
			aboutWindow->Show();
			printf("open about window");
			break;
		case UPDATE_TITLE: // update title with the name from the saveidea window
			if(msg->FindString("updatetitle", &updateTitle) == B_OK) // updated title exists in variable
			{
				tmpString = "Masterpiece Editor - ";
				tmpString += updateTitle;
				this->SetTitle(tmpString);
			}
			else // 
			{
				eAlert = new ErrorAlert("3.1 Editor Error: Message not found."); // message variable not found
				eAlert->Launch();
			}
			break;
		/*
		case SET_STATUS: // get message and update statusbar
			if(msg->FindString("statusstring", &tmpStatus) == B_OK)
			{
				SetStatusBar(tmpStatus);
			}
			else
			{
				eAlert = new ErrorAlert("3.1 Editor Error: Message not found.");
				eAlert->Launch();
			}
			break;
		*/
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
/*
 * Spawn thread reference from paladin.  can use this to test and see if it works...
 * Will work on tonight if I get a chance and see how it goes.
 */
/*
		case M_BACKUP_PROJECT:
		{
			thread_id backupThread = spawn_thread(BackupThread,"project backup thread",
												B_NORMAL_PRIORITY, this);
			if (backupThread >= 0)
			{
				fStatusBar->SetText(TR("Backing up project"));
				UpdateIfNeeded();
				
				SetMenuLock(true);
				resume_thread(backupThread);
			}
			break;
		}
*/
int32 MPEditor::PreviewThread(void* data)
{
	MPEditor* parent = (MPEditor*)data;

	ExecutePreview(parent->editorTextView->Text());

	parent->Lock();
	parent->SetStatusBar("Test");
	parent->Unlock();
	
	return 0;
}
/*
int32
ProjectWindow::BackupThread(void *data)
{
	ProjectWindow *parent = (ProjectWindow*)data;
	Project *proj = parent->fProject;
	
	char timestamp[32];
	time_t t = real_time_clock();
	strftime(timestamp,32,"_%Y-%m-%d-%H%M%S",localtime(&t));
	
	BPath folder(proj->GetPath().GetFolder());
	BPath folderparent;
	folder.GetParent(&folderparent);
	
	BString command = "cd '";
	command << folderparent.Path() << "'; ";
	command << "zip -9 -r -y '"
			<< gBackupPath.GetFullPath() << "/"
			<< proj->GetName() << timestamp << "' '"
			<< folder.Leaf() << "' -x *.o";
	
	system(command.String());
	
	parent->Lock();
	parent->fStatusBar->SetText("");
	parent->SetMenuLock(false);
	parent->Unlock();
	
	return 0;
}*/
