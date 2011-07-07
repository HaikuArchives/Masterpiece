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
	previewTextView->MakeSelectable(true);
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
			IdeaParser(rawText, previewTextView);
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
void MPPreview::IdeaParser(BString inputText, BTextView* displayTextView)
{
	displayTextView->SelectAll();
	displayTextView->GetSelection(&startPos, &endPos);
	displayTextView->GetFontAndColor(&parseFont, &sameProperties);
	parseFont.SetSize(24.0);
	displayTextView->SetFontAndColor(startPos, endPos, &parseFont, B_FONT_SIZE);
	
	// create parser here
	//return displayTextView;
	
	// TEST REGEX PARSER - this works and handles the parser conversion from BString to C string and back...
	/*
	string s;
	int i;
	pcrecpp::RE re("(\\w+):(\\d+)");
	re.FullMatch("ruby:1234 ruby:123", &s, &i);
	printf("\r\n%d\r\n", i);
	reTester = s.c_str();
	eAlert = new ErrorAlert(reTester);
	eAlert->Launch();
	s = "yabba dabba doo";
	pcrecpp::RE("b+").GlobalReplace("d", &s);
	eAlert = new ErrorAlert(s.c_str());
	eAlert->Launch();
	*/
	
	// TEST REGEX PARSER
	
	// 1.  it seems to do this, i will need to copy the entire string from the textview
	// 2.  split the string into substrings based on the parsing... (possibly capture the length)
	// 2.4 strlen to get the length.
	// 2.5 get the length, then use offset=0 + length for each substring as i go...
	// 3.  add the formatting requirements to the substrings based on the parsing
	// 3.  use InsertText(text, length, offset, runs) to add the formatted text
	
	// EXAMPLE TEXT "Test of some *bold* text and more *bold* text."
	
	// ACTUAL REGEX PARSER TEST
	
	// 1.  use global replace to search for a pattern in the textview string
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
