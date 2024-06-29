#include "PublishFilePanel.h"

PublishFilePanel::PublishFilePanel(BMessenger* target)
	:	BFilePanel(B_SAVE_PANEL, target, NULL, B_FILE_NODE, false)
{
	BWindow *w;
	w = Window();
	w->ResizeTo(575, 350);
	if(w->Lock())
	{
		BRect btnrect;
		BRect viewrect;
		float charWidth;
		BView* v = w->ChildAt(0);
		BView *cancelBtn = v->FindView("cancel button");
		
		if(cancelBtn)
		{
			BView* poseView = v->FindView("DirMenuField");
			BView* parentview;
			BEntry rst2pdfcheck("/boot/system/bin/rst2pdf");
			BEntry rst2htmcheck("/boot/system/bin/rst2html");
			BEntry rst2xmlcheck("/boot/system/bin/rst2xml");
			BEntry rst2odtcheck("/boot/system/bin/rst2odt");
			BEntry rst2texcheck("/boot/system/bin/rst2latex");
			charWidth = cancelBtn->StringWidth("Select Current Directory");
			btnrect = cancelBtn->Frame();
			btnrect.right = btnrect.left - 10;
			btnrect.left = btnrect.right - charWidth - 0;
			parentview = cancelBtn->Parent();
			pubMsg = new BMessage(PUBLISH_TYPE);
			publishTypeMenu = new BMenu("Type");
			publishTypeMenu->AddItem(new BMenuItem("HTM", new BMessage(PUBLISH_TYPE)));
			publishTypeMenu->AddItem(new BMenuItem("TEX", new BMessage(PUBLISH_TYPE)));
			publishTypeMenu->AddItem(new BMenuItem("ODT", new BMessage(PUBLISH_TYPE)));
			publishTypeMenu->AddItem(new BMenuItem("XML", new BMessage(PUBLISH_TYPE)));
			publishTypeMenu->AddItem(new BMenuItem("PDF", new BMessage(PUBLISH_TYPE)));
			publishTypeMenu->SetLabelFromMarked(true);
			publishTypeMenu->ItemAt(0)->SetMarked(true);
			publishTypeMenuField = new BMenuField(btnrect, "pubtype", "File Type:", publishTypeMenu, B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);
			publishTypeMenuField->SetDivider(publishTypeMenuField->StringWidth("File Type:") + 5.0);
			parentview->AddChild(publishTypeMenuField);
			if(poseView)
			{
				charWidth = poseView->StringWidth("Open When Done");
				viewrect = poseView->Frame();
				viewrect.left = viewrect.right - 50;
				viewrect.right = viewrect.left + charWidth + 50;
			}
			openCheckBox = new BCheckBox(viewrect, NULL, "Open When Done", new BMessage(PUBLISH_TYPE), B_FOLLOW_NONE, B_WILL_DRAW);
			parentview->AddChild(openCheckBox);
			SetMessage(pubMsg);
			SetTarget(*target);
			if(!rst2pdfcheck.Exists()) publishTypeMenu->FindItem("PDF")->SetEnabled(false);
			if(!rst2htmcheck.Exists()) publishTypeMenu->FindItem("HTM")->SetEnabled(false);
			if(!rst2xmlcheck.Exists()) publishTypeMenu->FindItem("XML")->SetEnabled(false);
			if(!rst2odtcheck.Exists()) publishTypeMenu->FindItem("ODT")->SetEnabled(false);
			if(!rst2texcheck.Exists()) publishTypeMenu->FindItem("TEX")->SetEnabled(false);
		}
		
		w->Unlock();
	}
}
