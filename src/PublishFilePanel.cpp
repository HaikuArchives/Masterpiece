#include "PublishFilePanel.h"
PublishFilePanel::PublishFilePanel(BMessenger* target)
	:	BFilePanel(B_SAVE_PANEL, target, NULL, B_FILE_NODE, false)
	//:	BFilePanel(B_SAVE_PANEL)
{
	BWindow *w;
	w = Window();
	w->ResizeBy(150, 0);
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
			parentview = cancelBtn->Parent();
			pubMsg = new BMessage(PUBLISH_TYPE);
			// need to use add string to determine which one is marked...
			//pubMsg->AddString("pubext", "ladidaladida");
			publishTypeMenu = new BMenu("Type");
			publishTypeMenu->AddItem(new BMenuItem("HTML", new BMessage(PUBLISH_TYPE)));
			publishTypeMenu->AddItem(new BMenuItem("LaTeX", new BMessage(PUBLISH_TYPE)));
			publishTypeMenu->AddItem(new BMenuItem("ODT", new BMessage(PUBLISH_TYPE)));
			publishTypeMenu->AddItem(new BMenuItem("XML", new BMessage(PUBLISH_TYPE)));
			publishTypeMenu->AddItem(new BMenuItem("PDF", new BMessage(PUBLISH_TYPE)));
			publishTypeMenu->SetLabelFromMarked(true);
			publishTypeMenu->ItemAt(0)->SetMarked(true);
			publishTypeMenuField = new BMenuField(btnrect, "pubtype", "File Type:", publishTypeMenu, B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);
			publishTypeMenuField->SetDivider(publishTypeMenuField->StringWidth("File Type:") + 5.0);
			parentview->AddChild(publishTypeMenuField);
		}
		
		w->Unlock();
			pubMsg->AddString("pubext", publishTypeMenu->FindMarked()->Label());
			SetMessage(pubMsg);
			SetTarget(*target);
	}
}
PublishFilePanel::~PublishFilePanel()
{
}