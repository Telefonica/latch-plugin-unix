    #include "CuTest.h"


    CuSuite* UtilGetSuite();
    CuSuite* CharsetGetSuite();


    int RunAllTests(void) {
        CuString *output = CuStringNew();
        CuSuite* suite = CuSuiteNew();

        CuSuiteAddSuite(suite, UtilGetSuite());
        CuSuiteAddSuite(suite, CharsetGetSuite());

        CuSuiteRun(suite);
        CuSuiteSummary(suite, output);
        CuSuiteDetails(suite, output);
        printf("%s\n", output->buffer);

        if(suite->failCount == 0){
            return 1;
        }else{
            return 0;
        }
    }

    int main(void) {
        int x = RunAllTests();
        if (x == 1){
            return 0;
        }
    }
