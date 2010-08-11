#include "ErrorAlert.h"

ErrorAlert::ErrorAlert(BString tmpText)
{
	tmpAlert = new BAlert("Error:", tmpText, "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
	tmpAlert->MoveTo(350, 250);
	tmpAlert->SetShortcut(0, B_ESCAPE);
	//int tmpReturn = tmpAlert->Go();
}

ErrorAlert::~ErrorAlert(void)
{
}

int ErrorAlert::Launch(void)
{
	return tmpAlert->Go(NULL);
}
