#ifndef ERRORALERT_H
#define ERRORALERT_H

#include <Alert.h>
#include <String.h>

class ErrorAlert
{
public:
				ErrorAlert(BString tmpText);
				ErrorAlert(BString tmpText1, BString tmpText2);
				~ErrorAlert(void);
				int Launch(void);
				
		BAlert	*tmpAlert;
};

#endif
