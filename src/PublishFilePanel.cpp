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
			BEntry rst2pdfcheck("/boot/common/bin/rst2pdf");
			BVolumeRoster volRoster;
			BVolume bootVolume;
			volRoster.GetBootVolume(&bootVolume);
			BString predicate("name==\"*[cC][oO][rR][eE].[pP][yY]\"");
			BQuery query;
			BString spath;
			query.SetVolume(&bootVolume);
			query.SetPredicate(predicate.String());
			// /boot/common/lib/python2.6/site-packages/docutils/core.py
			
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
			publishTypeMenu->FindItem("HTM")->SetEnabled(false);
			publishTypeMenu->FindItem("ODT")->SetEnabled(false);
			publishTypeMenu->FindItem("TEX")->SetEnabled(false);
			publishTypeMenu->FindItem("XML")->SetEnabled(false);

			// htm, odt, pdf, tex, xml
			if(query.Fetch() == B_OK)
			{
				//printf("Results of query \"%s\":\n", predicate.String());
				BEntry entry;
				BPath path;

				while (query.GetNextEntry(&entry) == B_OK)
				{
					entry.GetPath(&path);
					spath = path.Path();
					//printf("\t%s\n", path.Path());
					spath.RemoveSet("123456789.");
					if(spath == "/boot/common/lib/python/site-packages/docutils/corepy")
					{
						publishTypeMenu->FindItem("HTM")->SetEnabled(true);
						publishTypeMenu->FindItem("ODT")->SetEnabled(true);
						publishTypeMenu->FindItem("TEX")->SetEnabled(true);
						publishTypeMenu->FindItem("XML")->SetEnabled(true);
						//printf(spath);
						break;
					}
				}
			}
			if(!rst2pdfcheck.Exists()) publishTypeMenu->FindItem("PDF")->SetEnabled(false);
		}
		
		w->Unlock();
	}
}
