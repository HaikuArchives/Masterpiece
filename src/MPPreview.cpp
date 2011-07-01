#include "MPPreview.h"

MPPreview::MPPreview(int ideaID)
	:	BWindow(BRect(100, 100, 900, 700), "tmp", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS, B_CURRENT_WORKSPACE)
{
	// initialize controls
	BRect r = Bounds();
	r.bottom = r.bottom - 50;
	previewTextView = new BTextView(r, NULL, r, B_FOLLOW_ALL, B_WILL_DRAW);	
	previewTextView->SetStylable(true);
	previewTextView->MakeEditable(false);
	previewTextView->MakeSelectable(false);
	previewTextView->MakeResizable(true);
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backView);
	// gui layout builder
	backView->SetLayout(new BGroupLayout(B_HORIZONTAL, 0.0));
	backView->AddChild(BGridLayoutBuilder()
		.Add(new BScrollView("scroll_editor", previewTextView, B_FOLLOW_ALL_SIDES, 0, false, true, B_FANCY_BORDER), 0, 0)
		.SetInsets(0, 0, 0, 0)
	);
	currentideaID = ideaID; // pass current idea id selected to editor window for use
	if(currentideaID != -1) // if id has a real value
	{
		sqlObject = new SqlObject(ideaStatement, "7");
		sqlObject->PrepareSql("select ideatext, ideaname, ismp from ideatable where ideaid = ?");
		sqlObject->BindValue(1, currentideaID);
		sqlObject->StepSql();
		if(sqlObject->ReturnInt(2) == 0) // thought was selected to preview
		{
			rawText = sqlObject->ReturnText(0);
			previewTextView->SetText(rawText);
			BString tmpText;
			tmpText = "MasterPiece Preview - ";
			tmpText += sqlObject->ReturnText(1);
			this->SetTitle(tmpText);
			parsedText = IdeaParser(rawText, previewTextView);
			// parse rawText here....
			// getfontandcolor, store it, modify it, then setfontandcolor
			// ParseRawText(rawText, previewTextView);
			/*
				{
					previewTextView->InsertText(parsed lines);
					BEST METHOD MIGHT BE TO SETTEXT IN PREVIEWTEXTVIEW, THEN
					SELECT THE TEXT AND PARSE ACCORDINGLY USING SETFONTANDCOLOR
				}
			*/
			// display text in previewTextView such as...
		}
		else if(sqlObject->ReturnInt(2) == 1) // masterpiece was selected to preview
		{
			// sql query to get all thoughts
			// parse rawText for each thought and call insert to previewtextview here....
		}
		
		sqlObject->FinalizeSql();
		sqlObject->CloseSql();
	}
}
/*
void GetFontAndColor(BFont     *font,
                     uint32    *sameProperties,
                     rgb_color *color = NULL,
                     bool      *sameColor = NULL)

void SetFontAndColor(const BFont *font,
                     uint32      properties = B_FONT_ALL,
                     rgb_color   *color = NULL)


BFont   font;
uint32  sameProperties;
theTextView->GetFontAndColor(&font, &sameProperties);
font.SetSize(24.0);
theTextView->SetFontAndColor(&font, B_FONT_ALL);

BFont theBigFont(be_plain_font);
theBigFont.SetSize(48.0);
theBigFont.SetRotation(-45.0);
theBigFont.SetShear(120.0);
theTextView->SetFontAndColor(&theBigFont, B_FONT_SIZE);

BFont      font;
uint32     sameProperties;
rgb_color  redColor = {255, 0, 0, 255};
theTextView->GetFontAndColor(&font, &sameProperties);
theTextView->SetFontAndColor(&font, B_FONT_ALL, &redColor);

*/
BString MPPreview::IdeaParser(BString inputText, BTextView* displayTextView)
{
	displayTextView->GetFontAndColor(&parseFont, &sameProperties);
	parseFont.SetSize(24.0);
	displayTextView->SetFontAndColor(&parseFont, B_FONT_ALL);
	// create parser here
	return "done";
}
void MPPreview::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}
bool MPPreview::QuitRequested(void)
{
	return true;
}
