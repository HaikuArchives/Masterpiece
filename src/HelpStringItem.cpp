#include "HelpStringItem.h"

HelpStringItem::HelpStringItem(BString topicstring, BString contentstring)
	:	BStringItem(topicstring)
{
	topicString = topicstring;
	contentString = contentstring;
}
BString HelpStringItem::ReturnTopic(void) const
{
	return topicString;
}
BString HelpStringItem::ReturnContent(void) const
{
	return contentString;
}
