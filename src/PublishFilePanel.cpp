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
		BView* v = w->ChildAt(0);
		BView *cancelBtn = v->FindView("cancel button");
		
		if(cancelBtn)
		{
			BView* parentview;
			float charWidth;
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
			if(query.Fetch() == B_OK)
			{
				printf("Results of query \"%s\":\n", predicate.String());
				entry_ref ref;
				BEntry entry;
				BPath path;
				while (query.GetNextEntry(&entry) == B_OK)
				{
					entry.GetPath(&path);
					spath = path.Path();
					printf("\t%s\n", path.Path());
					if(spath.IFindFirst("/boot/common/lib"))
					{
						if(spath.IFindFirst("python"))
						{
							if(spath.IFindFirst("site-packages/docutils/core.py"))
							{
								printf("GOOD");
							}
						}
					}
				}
			}
			
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
			SetMessage(pubMsg);
			SetTarget(*target);
			// htm, odt, pdf, tex, xml
			/*
			if(!docutilscheck.Exists())
			{
				printf(" nope");
				publishTypeMenu->FindItem("HTM")->SetEnabled(false);
				publishTypeMenu->FindItem("ODT")->SetEnabled(false);
				publishTypeMenu->FindItem("TEX")->SetEnabled(false);
				publishTypeMenu->FindItem("XML")->SetEnabled(false);
				if(!rst2pdfcheck.Exists()) publishTypeMenu->FindItem("PDF")->SetEnabled(false);
			}
			else
			{
				printf(" yup");
			}
			*/
			//if(!CheckExistingScripts("htm")) publishTypeMenu->FindItem("HTM")->SetEnabled(false);
			//if(!CheckExistingScripts("odt")) publishTypeMenu->FindItem("ODT")->SetEnabled(false);
			//if(!CheckExistingScripts("tex")) publishTypeMenu->FindItem("TEX")->SetEnabled(false);
			//if(!CheckExistingScripts("xml")) publishTypeMenu->FindItem("XML")->SetEnabled(false);
		}
		
		w->Unlock();
	}
}
