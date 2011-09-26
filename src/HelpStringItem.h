#ifndef HELPSTRINGITEM_H
#define HELPSTRINGITEM_H

#include <ListView.h>
#include <String.h>

class HelpStringItem : public BStringItem
{
	public:
						HelpStringItem(BString topicstring, BString contentstring);
				BString ReturnTopic(void) const;
				BString ReturnContent(void) const;
	private:
				BString topicString;
				BString contentString;
};


#endif
