#include "IdeaStringItem.h"

IdeaStringItem::IdeaStringItem(BString itemText, int ideaid)
	:	BStringItem(itemText)
{
	ideaID = ideaid;
}
IdeaStringItem::IdeaStringItem(BString ideaname, BString ideatext, int ideaid)
	:	BStringItem(ideaname)
{
	ideaID = ideaid;
	ideaText = ideatext;
	ideaName = ideaname;
}
IdeaStringItem::IdeaStringItem(BString ideaname, BString ideatext, int ismp, int mpid, int ordernumber, int ideaid)
	:	BStringItem(ideaname)
{
	ideaID = ideaid;
	isMP = ismp;
	mpID = mpid;
	orderNumber = ordernumber;
	ideaText = ideatext;
	ideaName = ideaname;
}
int IdeaStringItem::ReturnID(void) const
{
	return ideaID;
}
int IdeaStringItem::ReturnIsMP(void) const
{
	return isMP;
}
int IdeaStringItem::ReturnMpID(void) const
{
	return mpID;
}
int IdeaStringItem::ReturnOrderNumber(void) const
{
	return orderNumber;
}
BString IdeaStringItem::ReturnText(void) const
{
	return ideaText;
}
BString IdeaStringItem::ReturnName(void) const
{
	return ideaName;
}
