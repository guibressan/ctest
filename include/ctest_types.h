#ifndef CTEST_TYPES_H
#define CTEST_TYPES_H

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
	ctest_test *tests;
	size_t alloc_size;
	int ntests;
} ctest_testrunner;

typedef struct {
	int nfailures;
	int npasses;
} ctest_testreport;

#endif	/*	CTEST_TYPES_H	*/
