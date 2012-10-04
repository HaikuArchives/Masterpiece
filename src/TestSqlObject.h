#ifndef TESTSQLOBJECT_H
#define TESTSQLOBJECT_H

#include "TestCase.h"
#include "TestSuite.h"
#include "TestCaller.h"
#include "TestRunner.h"

#include "CommonFunctions.h"

class SqlTestCase :  public TestCase
{
	SqlTestCase() : TestCase() {}
	
	void testConstructor();
	
	void testReturnValue();
	
	static Test *suite();

};

#endif
