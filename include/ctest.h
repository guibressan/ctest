#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include <sys/_types/_size_t.h>

typedef enum {
	CTEST_STATUS_PASS = 0,
	CTEST_STATUS_FAIL = -1,
	CTEST_STATUS_FATAL = -2,
} ctest_status;

typedef struct {
	const char *failreason;
	ctest_status status;
} ctest_result;

typedef ctest_result(*testfunc)(void);

ctest_result ctest_pass(void);
ctest_result ctest_fail(const char *failreason);
ctest_result ctest_fatal(const char *failreason);

typedef struct {
	const char *tname;
	testfunc fn;
} ctest_test;

typedef struct {
	int ntests;
	size_t alloc_size;
	ctest_test *tests;
} ctest_testrunner;

typedef struct {
	int nfailures;
	int npasses;
} ctest_testreport;

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

#endif /*	TESTRUNNER_H	*/
