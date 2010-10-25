#ifndef TMPWINDOW_H
#define TMPWINDOW_H

#include <Window.h>
#include <View.h>

class TmpWindow : public BWindow
{
public:
				TmpWindow(void);
private:
			BView	*tmpView;
};


#endif
