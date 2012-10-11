#include "MPEditor.h"

using namespace pyembed;

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
	thread_id previewThread;
	thread_id publishThread;
	//int32 publishCode = 63;
	
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
			//SetStatusBar("Publishing File...");
			break;
		case PUBLISH_TYPE:
			// write data to a file
			fileExt = pubEditorPanel->publishTypeMenu->FindMarked()->Label();
			fileExt = fileExt.ToLower();
			editorMessage = msg;
			publishThread = spawn_thread(PublishThread, "publish thread", B_NORMAL_PRIORITY, (void*)this);
			//send_data(publishThread, publishCode, (void*) msg, strlen(msg));
			if(publishThread >= 0) // successful
			{
				SetStatusBar("Publishing File...");
				UpdateIfNeeded();
				resume_thread(publishThread);
			}
			//ExecutePublish(msg, editorTextView->Text(), fileExt);
			//SetStatusBar("Publish Completed Successfully");
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
	MPEditor* parent = (MPEditor*) data;
	ErrorAlert* eAlert;
	const char* name;
	BEntry entry;
	BPath path;
	entry_ref ref;
	int argc = 1;
	char* argvv = "ladida";
	char** argv = &argvv;
	Python py(argc, argv);
	BString publishPath; // user generated filename
	BString tmpInPath; // string path of tmppub.tht file, then string path of tmppub.ext
	BString tmpOutPath;
	BString pythonString;
	BFile previewFile; // tmppub.tht file
	BString runPath; // rst2pdf execute path
	BString dirPath; // user created directory path string
	BEntry publishFile; // file that is renamed to the new user generated filename from tmppath
	BEntry removeTmpFile; // tmp file that information that will be removed
	BEntry removeOldFile; // tmp final file that will be removed from appdir
	BDirectory publishDirectory; // user generated directory
	BString oldFilePath; // path to the renamed tmpfile
	BString newFilePath; // path to the actual saved file
	status_t err; // auto errors
	tmpInPath = GetAppDirPath();
	tmpInPath += "/tmppub.tht";
	tmpOutPath = GetAppDirPath();
	tmpOutPath += "/tmppub.";
	tmpOutPath += parent->fileExt;
	pythonString = "output = publish_file(source_path='";
	pythonString += tmpInPath;
	pythonString += "', destination_path='";
	pythonString += tmpOutPath;
	pythonString += "', writer_name='";
	if(parent->fileExt == "odt") pythonString += "odf_odt')";
	else if(parent->fileExt == "tex") pythonString += "latex')";
	else if(parent->fileExt == "htm") pythonString += "html')";
	else if(parent->fileExt == "xml") pythonString += "xml')";
	else if(parent->fileExt == "pdf")
	{
		// do nothing here.  there is no pythonstring to convert to pdf.
		// but I want to account for any abnormalities that may arise
		// if the wrong filetype is somehow selected.
	}
	else
	{
		eAlert = new ErrorAlert("4.3 Publish File Type Error: Invalid filetype.");
		eAlert->Launch();
	}
	removeTmpFile.SetTo(tmpInPath);
	previewFile.SetTo(tmpInPath, B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE); // B_ERASE_FILE
	if(previewFile.InitCheck() != B_OK)
	{
		eAlert = new ErrorAlert("3.4 Editor Error: Couldn't Create Initial Publish File.");
		eAlert->Launch();
	}
	previewFile.Write(parent->editorTextView->Text(), strlen(parent->editorTextView->Text()));
	previewFile.Unset();

	if(parent->fileExt == "pdf")
	{
		runPath = "/boot/common/bin/rst2pdf ";
		runPath += GetAppDirPath();
		runPath += "/tmppub.tht -o ";
		runPath += GetAppDirPath();
		runPath += "/tmppub.pdf";
		system(runPath);
	}
	else // not PDF run
	{
		try
		{
			py.run_string("from docutils.core import publish_file");
			py.run_string(pythonString.String());
		}
		catch(Python_exception ex)
		{
			eAlert = new ErrorAlert("3.5 Editor Error: Python Issue - ", ex.what());
			eAlert->Launch();
			err = removeTmpFile.Remove();
			if(err != B_OK)
			{
				eAlert = new ErrorAlert("3.14 Editor Error: Tmp File could not be removed due to: ", strerror(err));
				eAlert->Launch();
			}
		}
	}
	
	// now i need to get the finished file and mv/rename it to the correct location
	if(parent->editorMessage->FindString("name", &name) == B_OK)
	{
		printf("default save message: %s\n", name);
	}
	else
	{
		printf("no string name\n\n");
	}
	if(parent->editorMessage->FindRef("directory", &ref) == B_OK)
	{
		tmpInPath = GetAppDirPath();
		tmpInPath += "/tmppub.";
		tmpInPath += parent->fileExt;
		printf(" Current tmppath: ");
		printf(tmpInPath);
		printf("\n");
		publishPath = name;
		publishPath.Append(".");
		publishPath.Append(parent->fileExt);
		publishFile.SetTo(tmpInPath);
		publishFile.Rename(publishPath, true);
		oldFilePath = GetAppDirPath();
		oldFilePath += "/tmppub";
		oldFilePath += ".";
		oldFilePath += parent->fileExt;
		entry.SetTo(&ref); // directory where the file is to be saved as defined by user
		entry.SetTo(&ref);
		entry.GetPath(&path);
		dirPath = path.Path();
		dirPath += "/";				
		newFilePath = dirPath;
		newFilePath += name;
		newFilePath += ".";
		newFilePath += parent->fileExt;
		printf("old file: %s\n", oldFilePath.String());
		printf("new file: %s\n", newFilePath.String());
		if(publishDirectory.SetTo(dirPath) == B_OK) // set publish directory to the user created directory
		{
			err = publishFile.MoveTo(&publishDirectory, NULL, true); // move publish file to publish directory
			if(err != B_OK)
			{
				if(err == B_CROSS_DEVICE_LINK)
				{
					BFile oldFile;
					BFile newFile;
					if(oldFile.SetTo(oldFilePath, B_READ_ONLY) == B_OK)
					{
						if(newFile.SetTo(newFilePath, B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE) == B_OK)
						{
							off_t length;
							char* text;
							oldFile.GetSize(&length);
							text = (char*) malloc(length);
							if(text && oldFile.Read(text, length) >= B_OK) // write text to the newfile
							{
								err = newFile.Write(text, length);
								if(err >= B_OK)
								{
									eAlert = new ErrorAlert("3.13 Editor Error: File could not be written due to: ", strerror(err));
									eAlert->Launch();		
								}
								else
								{
									removeOldFile.SetTo(oldFilePath);
									err = removeOldFile.Remove();
									if(err != B_OK)
									{
										eAlert = new ErrorAlert("3.14 Editor Error: Tmp File could not be removed due to: ", strerror(err));
										eAlert->Launch();
									}
								}
							}
							free(text);
						}
					}
				}
				else
				{
					eAlert = new ErrorAlert("3.15 Editor Error: File could not be written due to: ", strerror(err));
					eAlert->Launch();		
				}
			}
		}
		else
		{
			eAlert = new ErrorAlert("3.6 Editor Error: Directory Set Failed");
			eAlert->Launch();
		}
	}
	// clean up the temporary files...
	err = removeTmpFile.Remove();
	if(err != B_OK)
	{
		eAlert = new ErrorAlert("3.14 Editor Error: Tmp File could not be removed due to: ", strerror(err));
		eAlert->Launch();
	}
	parent->Lock();
	parent->SetStatusBar("Publish Completed Successfully");
	parent->Unlock();
	
	return 0;
}
