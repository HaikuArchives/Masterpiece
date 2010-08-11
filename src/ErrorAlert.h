#ifndef ERRORALERT_H
#define ERRORALERT_H

#include <Alert.h>
#include <String.h>

class ErrorAlert
{
public:
				ErrorAlert(BString tmpText);
				~ErrorAlert(void);
				int Launch(void);
				
		BAlert	*tmpAlert;
};

#endif
