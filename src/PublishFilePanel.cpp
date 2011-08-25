#include "PublishFilePanel.h"
PublishFilePanel::PublishFilePanel(BMessenger* target)
	:	BFilePanel(B_SAVE_PANEL, target, NULL, B_FILE_NODE, false)
	//	BFilePanel(B_SAVE_PANEL, BMessenger*, const entry_ref*, long unsigned int, bool, BMessage*, BRefFilter*, bool, bool)
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
			btnrect.left = btnrect.right - charWidth - 0;
			//fCurrentDirBtn = new BButton(btnrect, "current dir button", "Select Current Directory", NULL, B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
			
			//fCurrentDirBtn->SetTarget(*target);
			parentview = cancelBtn->Parent();
			
			msg = new BMessage(PUBLISH_TYPE);
			msg->AddString("pubext", "ladidaladida");
			publishTypeMenu = new BMenu("Type");
			publishTypeMenu->AddItem(new BMenuItem("HTML", msg));
			publishTypeMenu->AddItem(new BMenuItem("LaTeX", msg));
			publishTypeMenu->AddItem(new BMenuItem("ODT", msg));
			publishTypeMenu->AddItem(new BMenuItem("PDF", msg));
			publishTypeMenu->AddItem(new BMenuItem("XML", msg));
			publishTypeMenu->SetLabelFromMarked(true);
			publishTypeMenu->ItemAt(0)->SetMarked(true);
			/*
			how to position item below something.
			r = fTargetText->Frame();
			r.OffsetBy(0,r.Height() + 10.0);
			*/
			publishTypeMenuField = new BMenuField(btnrect, "pubtype", "File Type:", publishTypeMenu, B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);
			publishTypeMenuField->SetDivider(publishTypeMenuField->StringWidth("File Type:") + 5.0);
			parentview->AddChild(publishTypeMenuField);
			//SetMessage(msg);
			//SetTarget(*target);
			//BMessage* msg = new BMessage('slct');
			//GetPanelDirectory(&ref);
			//msg->AddRef("refs", &ref);
			//fCurrentDirBtn->SetMessage(msg);
		}
		
		w->Unlock();
	}
}
/*
PublishFilePanel::~PublishFilePanel()
{
	//delete &ref;
}
*/
void PublishFilePanel::SelectionChanged()
{
	/*
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
  */
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
