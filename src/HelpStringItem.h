#ifndef HELPSTRINGITEM_H
#define HELPSTRINGITEM_H

#include <ListView.h>
#include <String.h>

class HelpStringItem : public BStringItem
{
	public:
						HelpStringItem(BString itemText, int ideaid = -1);
						HelpStringItem(BString ideaname, BString ideatext, int ideaid = -1);
						HelpStringItem(BString ideaname, BString ideatext, int ismp, int mpid, int ordernumber, int ideaid = -1);
				int		ReturnID(void) const;
				int 	ReturnIsMP(void) const;
				int 	ReturnMpID(void) const;
				int 	ReturnOrderNumber(void) const;
				BString ReturnText(void) const;
				BString ReturnName(void) const;
	private:
				int 	ideaID;
				int		isMP;
				int		mpID;
				int		orderNumber;
				BString ideaText;
				BString ideaName;
};


#endif
