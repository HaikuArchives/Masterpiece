#include "MPEditor.h"

using namespace pyembed;

MPEditor::MPEditor(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID)
	:	BWindow(BRect(100, 100, 900, 700), windowTitle, B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE), launcherMessage(msg), launcherMessenger(msgr)
{
	AddShortcut('q', B_COMMAND_KEY, new BMessage(B_QUIT_REQUESTED));
	AddShortcut('n', B_COMMAND_KEY, new BMessage(MENU_NEW_THT));
	AddShortcut('e', B_COMMAND_KEY, new BMessage(MENU_EDT_THT));
	AddShortcut('s', B_COMMAND_KEY, new BMessage(MENU_SAV_THT));
	AddShortcut('r', B_COMMAND_KEY, new BMessage(MENU_PRV_THT));
	AddShortcut('p', B_COMMAND_KEY, new BMessage(MENU_PUB_THT));
	AddShortcut('k', B_COMMAND_KEY, new BMessage(MENU_KEY_THT));
	// initialize controls
	pubEditorPanel = NULL;
	BRect r = Bounds();
	r.bottom = r.bottom - 50;
	editorTextView = new BTextView(r, NULL, r, B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	// gui layout builder
	backView->SetLayout(new BGroupLayout(B_HORIZONTAL, 0.0));
	backView->AddChild(BGridLayoutBuilder()
		.Add(new EditorMenu(), 0, 0)
		.Add(new BScrollView("scroll_editor", editorTextView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 0, 1)
		.SetInsets(0, 0, 0, 0)
	);
	
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
	}
}
void MPEditor::MessageReceived(BMessage* msg)
{
	BRect r(Bounds());
	int argc = 1;
	char* argvv = "ladida";
	char** argv = &argvv;
	Python py(argc, argv);
	BString publishPath; // user generated filename
	BString tmpPath; // string path of tmppub.tht file, then string path of tmppub.ext
	BFile previewFile; // tmppub.tht file
	BString scriptFile; // python script file name
	BString fileExt; // file extension of converted file
	BString dirPath; // user created directory path string
	BEntry publishFile; // file that is renamed to the new user generated filename from tmppath
	BEntry removeTmpFile; // tmp file that information that will be removed
	BDirectory publishDirectory; // user generated directory
	status_t err; // auto errors
	
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
			}
			break;
		case MENU_PRV_THT: // preview thought in html in webpositive
			tmpPath = GetAppDirPath();
			tmpPath += "/tmp.tht";
			previewFile.SetTo(tmpPath, B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE); // B_ERASE_FILE
			if(previewFile.InitCheck() != B_OK)
			{
				printf("Couldn't write file\n");
			}
			previewFile.Write(editorTextView->Text(), strlen(editorTextView->Text()));
			previewFile.Unset();
			try
			{
				py.run_file("preview.py");
			}
			catch(Python_exception ex)
			{
				printf("Python error: %s\n", ex.what());
			}
			
			tmpPath = "/boot/apps/WebPositive/WebPositive file://";
			tmpPath += GetAppDirPath();
			tmpPath += "/tmp.html &";
			system(tmpPath);
			// prior to closing the course, check for the tmp.tht and tmp.html files and delte them.
			// delete the tmp file on exit. need to know when it happens, so try vfork, fork, spawn, waitpid.
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
			tmpPath = GetAppDirPath();
			tmpPath += "/tmppub.tht";
			removeTmpFile.SetTo(tmpPath);
			previewFile.SetTo(tmpPath, B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE); // B_ERASE_FILE
			if(previewFile.InitCheck() != B_OK)
			{
				printf("couldn't read file\n");
			}
			previewFile.Write(editorTextView->Text(), strlen(editorTextView->Text()));
			previewFile.Unset();
			
			// build the correct publish python script name
			fileExt = pubEditorPanel->publishTypeMenu->FindMarked()->Label();
			fileExt = fileExt.ToLower();
			scriptFile = "pub";
			scriptFile += fileExt;
			scriptFile += ".py";
			printf(scriptFile);
			printf("\n");
			tmpPath = GetAppDirPath();
			tmpPath += "/tmppub.";
			tmpPath += fileExt;
			try
			{
				py.run_file(scriptFile.String());
			}
			catch(Python_exception ex)
			{
				printf("Python error: %s\n", ex.what());
			}
			
			// now i need to get the finished file and mv/rename it to the correct location
			if(msg->FindString("name", &name) == B_OK)
			{
				printf("default save message: %s\n", name);
			}
			if(msg->FindRef("directory", &ref) == B_OK)
			{
				publishPath = name;
				publishPath.Append(".");
				publishPath.Append(fileExt);
				printf(publishPath);
				publishFile.SetTo(tmpPath);
				publishFile.Rename(publishPath, true);
				printf("Tmp Path: %s\nPublishPath: %s\n", tmpPath.String(), publishPath.String());
				entry.SetTo(&ref); // directory where the file is to be saved as defined by user
				entry.SetTo(&ref);
				entry.GetPath(&path);
				dirPath = path.Path();
				dirPath += "/";				
				if(publishDirectory.SetTo(dirPath) == B_OK) // set publish directory to the user created directory
				{
					printf("publishdirectory %s\n", path.Path());
					printf("successful directory set\n");
					err = publishFile.MoveTo(&publishDirectory, NULL, true); // move publish file to publish directory
					if(err != B_OK)
					{
						eAlert = new ErrorAlert("3.13 Editor Error: File could not be written due to: ", strerror(err));
						eAlert->Launch();		
					}
				}
				else
				{
					printf("directory set failed");
				}
			}
			// clean up the temporary files...
			err = removeTmpFile.Remove();
			if(err != B_OK)
			{
				eAlert = new ErrorAlert("3.14 Editor Error: Tmp File could not be removed due to: ", strerror(err));
				eAlert->Launch();
			}
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
			printf("open markup reference window");
			break;
		case MENU_ABT_THT: // open about window
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
