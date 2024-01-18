#ifndef TESTRUNNER_H
#define TESTRUNNER_H
	extern const char *terrdetails;

	typedef enum {
		PASS = 0,
		FAIL = -1,
		FATAL = -2,
	} tstatus;

	typedef tstatus(*testfunc)(void);

	typedef struct {
		const char *tname;
		testfunc fn;
	} test;

	typedef struct {
		int ntests;
		int alloc_size;
		test *tests;
	} testrunner;

	typedef struct {
		int nfailures;
		int npasses;
	} testreport;

	// test_new creates a new test instance
	test test_new(const char *tname, testfunc fn);

	// testrunner_new creates a new testrunner instance
	testrunner testrunner_new();

	// testrunner_drop drops the testrunner instance
	void testrunner_drop(testrunner *trunner);

	// testrunner_add subscribes a new test
	// returns <0 on error, 0 on success
	int testrunner_add(testrunner *trunner, test t);

	// testrunner_run run all the tests
	// returns the test report
	testreport testrunner_run(testrunner *trunner);

	// tstatus convert the test status to string
	const char *tstatustostr(tstatus s);

#endif
