#include "IdeaStringItem.h"

IdeaStringItem::IdeaStringItem(BString itemText, int ideaid)
	:	BStringItem(itemText)
{
	ideaID = ideaid;
}
int IdeaStringItem::ReturnID(void) const
{
	return ideaID;
}
