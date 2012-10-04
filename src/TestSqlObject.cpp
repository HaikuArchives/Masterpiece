#include "TestSqlObject.h"

void SqlTestCase::testConstructor()
{
}

void SqlTestCase::testReturnValue()
{
}

Test *SqlTestCase::suite()
{
	TestSuite *testSuite = new TestSuite("SqlTestCase");
	
	testSuite->addTest(new TestCaller("testConstructor", &SqlTestCase::testConstructor));
	testSuite->addTest(new TestCaller("testReturnValue", &SqlTestCase::testReturnValue));
	
	return testSuite;
}

int main (int argc, char* argv[]) {
	
	TestRunner runner;
	runner.addTest(argv[1], SqlTestCase::suite());
	runner.run(argc, argv);
	
	return 0;
}
