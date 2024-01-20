#ifndef CTEST_H
#define CTEST_H

#include "ctest_types.h"

/* testrunner_new creates a new testrunner instance */
ctest_testrunner ctest_testrunner_new();

/* testrunner_drop drops the testrunner instance */
void ctest_testrunner_drop(ctest_testrunner *tr);

/*	testrunner_add subscribes a new test.
		return values:
			1: succes
			0: error	*/
int ctest_testrunner_addtest(ctest_testrunner *tr, const char *testname, testfunc fn);

/*	testrunner_run run all the tests.
		return values: the test report	*/
ctest_testreport ctest_testrunner_run(ctest_testrunner *tr);

/*	tstatus convert the test status to string	*/
const char *ctest_status_tostr(ctest_status s);

#endif /*	CTEST_H	*/
