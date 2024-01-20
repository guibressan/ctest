#include <stdio.h>
#include <unistd.h>
#include "ctest.h"

ctest_status test1(void) {
	sleep(1);
	return CTEST_STATUS_PASS;
}
ctest_status test2(void) {
	ctest_errdetails = "test2 err description";
	return CTEST_STATUS_FAIL;
}
ctest_status test3(void) {
	ctest_errdetails = "test3 err description";
	return CTEST_STATUS_FATAL;
}
ctest_status test4(void) {
	ctest_errdetails = "test4 err description";
	return CTEST_STATUS_PASS;
}

int testing_1(void){
	ctest_testrunner t = ctest_testrunner_new();
	ctest_testrunner_addtest(&t, ctest_test_new("test 1", test1));
	ctest_testrunner_addtest(&t, ctest_test_new("test 2", test2));
	ctest_testrunner_addtest(&t, ctest_test_new("test 3", test3));
	ctest_testrunner_addtest(&t, ctest_test_new("test 4", test4));
	ctest_testreport r = ctest_testrunner_run(&t);
	ctest_testrunner_drop(&t);
	if (r.nfailures != 2 && r.npasses != 1) {
		fprintf(
				stderr,
				"testing_1 failed. nfailures: %d, npasses: %d",
				r.nfailures,
				r.npasses
		);
		return -1;
	}
	return 0;
}

int testing_2(void){
	ctest_testrunner t = ctest_testrunner_new();
	ctest_testrunner_addtest(&t, ctest_test_new("test 1", test1));
	ctest_testrunner_addtest(&t, ctest_test_new("test 2", test2));
	ctest_testrunner_addtest(&t, ctest_test_new("test 4", test4));
	ctest_testreport r = ctest_testrunner_run(&t);
	ctest_testrunner_drop(&t);
	if (r.nfailures != 1 && r.npasses != 2) {
		fprintf(
				stderr, "testing_2 failed. nfailures: %d, npasses: %d",
				r.nfailures,
				r.npasses
		);
		return -1;
	}
	return 0;
}

int main(void) {
	if (testing_1() || testing_2()) return 1;
	return 0;
}
