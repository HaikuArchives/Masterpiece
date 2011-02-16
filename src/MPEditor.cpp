#include "MPEditor.h"

MPEditor::MPEditor(const BMessage &msg, const BMessenger &msgr, BString windowTitle, int ideaID)
	:	BWindow(BRect(100, 100, 900, 700), windowTitle, B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE), launcherMessage(msg), launcherMessenger(msgr)
{
	BRect r = Bounds();
	r.bottom = r.bottom - 50;
	editorTextView = new BTextView(r, NULL, r, B_FOLLOW_ALL, B_WILL_DRAW);	
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	
	backView->SetLayout(new BGroupLayout(B_HORIZONTAL, 0.0));
	backView->AddChild(BGridLayoutBuilder()
		.Add(new EditorMenu(), 0, 0)
		.Add(new BScrollView("scroll_editor", editorTextView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 0, 1)
		.SetInsets(0, 0, 0, 0)
	);
	currentideaID = ideaID;
	
	mpdb = OpenSqliteDB();
	if(mpdb == NULL)
	{
		eAlert = new ErrorAlert("sql db was not opened properly.");
		eAlert->Launch();
	}
	if(currentideaID != -1)
	{
		// need to pull data from db and populate thoughtview with it...
		sqlValue = sqlite3_prepare_v2(mpdb, "select ideatext from ideatable where ideaid = ?", -1, &ideaStatement, NULL);
		sqlite3_bind_int(ideaStatement, 1, currentideaID);
		sqlite3_step(ideaStatement);
		editorTextView->SetText(sqlite3_mprintf("%s", sqlite3_column_text(ideaStatement, 0)));
		sqlite3_finalize(ideaStatement);
	}
}
void MPEditor::MessageReceived(BMessage* msg)
{
	BRect r(Bounds());
	switch(msg->what)
	{
		case MENU_NEW_THT:
			tmpEditor = new MPEditor(BMessage(SHOW_LAUNCHER), BMessenger(this), "MasterPiece Editor - untitled", -1);
			tmpEditor->Show();
			break;
		case MENU_EDT_THT:
			printf(" must open edit name dialog\r\n");
			break;
		case MENU_SAV_THT:
			if(currentideaID == -1)
			{
				printf("need to write sql for idea insert, then must open save dialog to set name\r\n");
				xPos = (r.right - r.left) / 2;
				yPos = (r.bottom - r.top) / 2;
				saveIdea = new SaveIdea(xPos, yPos, currentideaID);
				saveIdea->Show();
			}
			else
			{
				// need to open mpdb first...
				sqlValue = sqlite3_prepare_v2(mpdb, "update ideatable set ideatext = ? where ideaid = ?", -1, &ideaStatement, NULL);
				sqlite3_bind_text(ideaStatement, 1, editorTextView->Text(), -1, SQLITE_TRANSIENT);
				sqlite3_bind_int(ideaStatement, 2, currentideaID);
				sqlite3_step(ideaStatement);
				sqlite3_finalize(ideaStatement);
				printf("must just write sql\r\n");
			}
			break;
		case MENU_PRV_THT:
			printf("save data, export to python html one and open data in preview window or webpositive");
			break;
		case MENU_PUB_THT:
			printf("save data, open publish to window, export to python and save as name in publish window");
			break;
		case MENU_HLP_THT:
			printf("open help topic window");
			break;
		case MENU_KEY_THT:
			printf("open keyboard reference window");
			break;
		case MENU_MRK_THT:
			printf("open markup reference window");
			break;
		case MENU_ABT_THT:
			printf("open about window");
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
	launcherMessage.MakeEmpty();
	launcherMessage.AddInt64("showLauncher", 1);
	launcherMessenger.SendMessage(&launcherMessage);
	return true;
}
