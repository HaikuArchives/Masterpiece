#ifndef IDEASTRINGITEM_H
#define IDEASTRINGITEM_H

#include <ListView.h>
#include <String.h>

class IdeaStringItem : public BStringItem
{
	public:
					IdeaStringItem(BString itemText, int ideaid = -1);
					IdeaStringItem(BString ideaname, int ideaid = -1, BString ideatext, int ismp, int mpid, int ordernumber);
				int	ReturnID(void) const;
				int ReturnIsMP(void) const;
				int ReturnMpID(void) const;
				int ReturnOrderNumber(void) const;
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
