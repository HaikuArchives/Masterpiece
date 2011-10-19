#include "AboutWindow.h"

AboutWindow::AboutWindow(BRect frame, const char* title)
	:	BWindow(frame, title, B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS | B_NOT_ZOOMABLE, B_CURRENT_WORKSPACE)
{
	// initialize controls      s
	AddShortcut('q', B_COMMAND_KEY, new BMessage(B_QUIT_REQUESTED));
	BRect r = Bounds();
	creditView = new BTextView(BRect(0, 0, r.right, 100), NULL, BRect(10, 10, r.right, 100), B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);
	creditView->SetWordWrap(true);
	creditView->MakeEditable(false);
	creditView->SetInsets(5,5,5,5);
	creditView->SetStylable(true);
	backView = new BView(Bounds(), "backview", B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);
	backView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	backView->SetHighColor((rgb_color){0, 0, 0, 255});
	AddChild(backView);
	// gui layout builder
	backView->SetLayout(new BGroupLayout(B_HORIZONTAL, 0.0));
	backView->AddChild(BGridLayoutBuilder()
		.Add(new BScrollView("scroll_content", creditView, B_FOLLOW_ALL_SIDES, 0, false, true, B_NO_BORDER), 2, 0, 6, 10)
		.SetInsets(0, 0, 0, 0)
	);
	// generate content here....
	rgb_color darkGrey = {100, 100, 100, 255};
	rgb_color darkBlack = {50, 50, 50, 255};
	BFont font(be_bold_font);
	font.SetSize(font.Size() + 3);
	BFont font2(be_plain_font);
	font2.SetFace(B_ITALIC_FACE);
	font2.SetSize(be_bold_font->Size());
	creditView->SetFontAndColor(&font, B_FONT_ALL, &darkBlack);
	creditView->Insert("Masterpiece\n\n");
	creditView->SetFontAndColor(be_plain_font, B_FONT_ALL, &darkGrey);
	creditView->Insert("Copyright 2010 - 2011 Pasquale J. Rinaldi, Jr.\n");
	creditView->Insert("The copyright to the code is property of Pasquale J. Rinaldi, Jr.");
	creditView->Insert(" or of the respective authors where expressly noted.\n");
	creditView->Insert("Code that is written by me is licensed and distrubuted");
	creditView->Insert(" under the MIT license.\nCopyrights for third party code is below.");
	creditView->Insert("\n\n");
	creditView->SetFontAndColor(&font2, B_FONT_ALL, &darkBlack);
	creditView->Insert("Haiku Community\n\n");
	creditView->SetFontAndColor(be_plain_font, B_FONT_ALL, &darkGrey);
	creditView->Insert("I wish to thank all the haiku developers for making this OS a reality.");
	creditView->Insert("I would also like to thank them for all their help as I wrote this program.\n");
	creditView->Insert("I would like to specifically thank DarkWyrm, Axel, ...\n");
	creditView->Insert("for their answering all my questions when asked.\n\n");
	creditView->SetFontAndColor(&font2, B_FONT_ALL, &darkBlack);
	creditView->Insert("Pyembed\n");
	creditView->SetFontAndColor(be_plain_font, B_FONT_ALL, &darkGrey);
	creditView->Insert("");
}
void AboutWindow::MessageReceived(BMessage* msg)
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
bool AboutWindow::QuitRequested(void)
{
	return true;
}
