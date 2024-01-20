#ifndef TESTRUNNER_H
#define TESTRUNNER_H
	extern const char *ctest_errdetails;

	typedef enum {
		CTEST_STATUS_PASS = 0,
		CTEST_STATUS_FAIL = -1,
		CTEST_STATUS_FATAL = -2,
	} ctest_status;

	typedef ctest_status(*testfunc)(void);

	typedef struct {
		const char *tname;
		testfunc fn;
	} ctest_test;

	typedef struct {
		int ntests;
		int alloc_size;
		ctest_test *tests;
	} ctest_testrunner;

	typedef struct {
		int nfailures;
		int npasses;
	} ctest_testreport;

	// test_new creates a new test instance
	ctest_test ctest_test_new(const char *tname, testfunc fn);

	// testrunner_new creates a new testrunner instance
	ctest_testrunner ctest_testrunner_new();

	// testrunner_drop drops the testrunner instance
	void ctest_testrunner_drop(ctest_testrunner *trunner);

	// testrunner_add subscribes a new test
	// returns <0 on error, 0 on success
	int ctest_testrunner_addtest(ctest_testrunner *trunner, ctest_test t);

	// testrunner_run run all the tests
	// returns the test report
	ctest_testreport ctest_testrunner_run(ctest_testrunner *trunner);

	// tstatus convert the test status to string
	const char *ctest_status_tostr(ctest_status s);

#endif
