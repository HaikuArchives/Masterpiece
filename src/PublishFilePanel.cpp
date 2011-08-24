#include "PublishFilePanel.h"

PublishFilePanel::PublishFilePanel(BMessenger* target)
	:	BFilePanel(B_OPEN_PANEL, target, NULL, B_DIRECTORY_NODE, false, NULL, NULL)
{
	BWindow *w;
	w = Window();
	if(w->Lock())
	{
		BRect btnrect;
		BView* v = w->ChildAt(0);
		BView *cancelBtn = v->FindView("cancel button");
		
		if(cancelBtn)
		{
			BView* parentview;
			float charWidth;
			
			charWidth = cancelBtn->StringWidth("Select Current Directory");
			btnrect = cancelBtn->Frame();
			btnrect.right = btnrect.left - 10;
			btnrect.left = btnrect.right - charWidth - 40;
			fCurrentDirBtn = new BButton(btnrect, "current dir button", "Select Current Directory", NULL, B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
			
			fCurrentDirBtn->SetTarget(*target);
			
			parentview = cancelBtn->Parent();
			parentview->AddChild(fCurrentDirBtn);
			
			entry_ref ref;
			BMessage* msg = new BMessage('slct');
			GetPanelDirectory(&ref);
			msg->AddRef("refs", &ref);
			fCurrentDirBtn->SetMessage(msg);
		}
		
		w->Unlock();
	}
}

void PublishFilePanel::SelectionChanged()
{
  BWindow* wind;

  wind = Window();

  if (wind->Lock()) {
    entry_ref currRef;

    GetPanelDirectory(&currRef);

    //  modify the btn's msg

    BMessage* msg = new BMessage('slct');
    msg->AddRef("refs",&currRef);
    fCurrentDirBtn->SetMessage(msg);

    wind->Unlock();
  }
}
/*
class TDirFilter : public BRefFilter {

public:
  bool Filter(const entry_ref* e, BNode* n, struct stat* s,
              const char* mimetype);
};

bool
TDirFilter::Filter(const entry_ref* e, BNode* n,
                   struct stat* s, const char* mimetype)
{
  if (strcmp("application/x-vnd.Be-directory",
             mimetype) == 0)
    return true;
  else if (strcmp("application/x-vnd.Be-volume",
                  mimetype) == 0)
    return true;
  else
    return false;
}
*/
