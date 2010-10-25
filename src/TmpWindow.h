#ifndef TMPWINDOW_H
#define TMPWINDOW_H

#include <Window.h>
#include <View.h>
#include <Button.h>

#define CLOSE_TMP		'ctmp'	// add new mp button message


class TmpWindow : public BWindow
{
public:
				TmpWindow(void);
private:
			BView	*tmpView;
			BButton	*tmpButton;
};


#endif
