#include <stdio.h>
#include <unistd.h>
#include "ctest.h"

ctest_result test1(void) {
	sleep(1);
	return ctest_pass();
}
ctest_result test2(void) {
	return ctest_fail("test2 err description");
}
ctest_result test3(void) {
	return ctest_fatal("test3 err description");
}
ctest_result test4(void) {
	return ctest_pass();
}

int testing_1(void){
	ctest_testrunner t = ctest_testrunner_new();
	ctest_testrunner_addtest(&t, "test 1", test1);
	ctest_testrunner_addtest(&t, "test 2", test2);
	ctest_testrunner_addtest(&t, "test 3", test3);
	ctest_testrunner_addtest(&t, "test 4", test4);
	ctest_testreport r = ctest_testrunner_run(&t);
	ctest_testrunner_drop(&t);
	if (r.nfailures != 2 && r.npasses != 1) {
		fprintf(
				stderr,
				"testing_1 failed. nfailures: %d, npasses: %d",
				r.nfailures,
				r.npasses
		);
		return 0;
	}
	return 1;
}

int testing_2(void){
	ctest_testrunner t = ctest_testrunner_new();
	ctest_testrunner_addtest(&t, "test 1", test1);
	ctest_testrunner_addtest(&t, "test 2", test2);
	ctest_testrunner_addtest(&t, "test 4", test4);
	ctest_testreport r = ctest_testrunner_run(&t);
	ctest_testrunner_drop(&t);
	if (r.nfailures != 1 && r.npasses != 2) {
		fprintf(
				stderr, "testing_2 failed. nfailures: %d, npasses: %d",
				r.nfailures,
				r.npasses
		);
		return 0;
	}
	return 1;
}

int main(void) {
	if (testing_1() && testing_2()) return 0;
	return 1;
}
