#include "HelpStringItem.h"

HelpStringItem::HelpStringItem(BString itemText, int ideaid)
	:	BStringItem(itemText)
{
	ideaID = ideaid;
}
HelpStringItem::HelpStringItem(BString ideaname, BString ideatext, int ideaid)
	:	BStringItem(ideaname)
{
	ideaID = ideaid;
	ideaText = ideatext;
	ideaName = ideaname;
}
HelpStringItem::HelpStringItem(BString ideaname, BString ideatext, int ismp, int mpid, int ordernumber, int ideaid)
	:	BStringItem(ideaname)
{
	ideaID = ideaid;
	isMP = ismp;
	mpID = mpid;
	orderNumber = ordernumber;
	ideaText = ideatext;
	ideaName = ideaname;
}
int HelpStringItem::ReturnID(void) const
{
	return ideaID;
}
int HelpStringItem::ReturnIsMP(void) const
{
	return isMP;
}
int HelpStringItem::ReturnMpID(void) const
{
	return mpID;
}
int HelpStringItem::ReturnOrderNumber(void) const
{
	return orderNumber;
}
BString HelpStringItem::ReturnText(void) const
{
	return ideaText;
}
BString HelpStringItem::ReturnName(void) const
{
	return ideaName;
}
