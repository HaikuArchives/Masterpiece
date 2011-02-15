#ifndef IDEASTRINGITEM_H
#define IDEASTRINGITEM_H

#include <ListView.h>
#include <String.h>

class IdeaStringItem : public BStringItem
{
	public:
					IdeaStringItem(BString itemText, int ideaid = -1);
				int	ReturnID(void) const;
	private:
				int ideaID;
};


#endif
