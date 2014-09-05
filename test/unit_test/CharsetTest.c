#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "CuTest.h"
#include "../../lib/charset.h"




/*-------------------------------------------------------------------------*
 * ValidCode Test
 *-------------------------------------------------------------------------*/

void TestValidCode(CuTest *tc) {

    char *input1 = "DD3abC";
    char *input2 = "AB8mb9";
    char *input3 = "yhj83A";
    char *input4 = "Cabcmn";
    char *input5 = "222289";
    char *input6 = "ab789D";
    char *input7 = "gfrtX6";
    char *input8 = "4frtXu";
    char *input9 = "2frtX6";
    char *input10 = "yDXyr8";

    int response1 = validCode(input1);
    int response2 = validCode(input2);
    int response3 = validCode(input3);
    int response4 = validCode(input4);
    int response5 = validCode(input5);
    int response6 = validCode(input6);
    int response7 = validCode(input7);
    int response8 = validCode(input8);
    int response9 = validCode(input9);
    int response10 = validCode(input10);

    CuAssertTrue(tc, response1);
    CuAssertTrue(tc, response2);
    CuAssertTrue(tc, response3);
    CuAssertTrue(tc, response4);
    CuAssertTrue(tc, response5);
    CuAssertTrue(tc, response6);
    CuAssertTrue(tc, response7);
    CuAssertTrue(tc, response8);
    CuAssertTrue(tc, response9);
    CuAssertTrue(tc, response10);
}

void TestValidCode_Exceptions(CuTest *tc) {

    char *input1 = "0abcmn";
    char *input2 = "OfrtX6";
    char *input3 = "1frtXu";
    char *input4 = "1frtX6";
    char *input5 = "dfr8X5";
    char *input6 = "yDSyr8";

    int response1 = validCode(input1);
    int response2 = validCode(input2);
    int response3 = validCode(input3);
    int response4 = validCode(input4);
    int response5 = validCode(input5);
    int response6 = validCode(input6);

    CuAssertTrue(tc, !response1);
    CuAssertTrue(tc, !response2);
    CuAssertTrue(tc, !response3);
    CuAssertTrue(tc, !response4);
    CuAssertTrue(tc, !response5);
    CuAssertTrue(tc, !response6);
}

void TestValidCode_Invalid_Chars(CuTest *tc) {

    char *input1 = "*abcmn";
    char *input2 = "Ofrt_6";
    char *input3 = "******";
    char *input4 = "'rptX6";
    char *input5 = "df¿?-g";
    char *input6 = "<ñsu 3";

    int response1 = validCode(input1);
    int response2 = validCode(input2);
    int response3 = validCode(input3);
    int response4 = validCode(input4);
    int response5 = validCode(input5);
    int response6 = validCode(input6);

    CuAssertTrue(tc, !response1);
    CuAssertTrue(tc, !response2);
    CuAssertTrue(tc, !response3);
    CuAssertTrue(tc, !response4);
    CuAssertTrue(tc, !response5);
    CuAssertTrue(tc, !response6);
}

void TestValidCode_Short(CuTest *tc) {

    char *input1 = "";
    char *input2 = "rtde6";
    char *input3 = "***";
    char *input4 = "m";

    int response1 = validCode(input1);
    int response2 = validCode(input2);
    int response3 = validCode(input3);
    int response4 = validCode(input4);

    CuAssertTrue(tc, !response1);
    CuAssertTrue(tc, !response2);
    CuAssertTrue(tc, !response3);
    CuAssertTrue(tc, !response4);
}

void TestValidCode_Long(CuTest *tc) {

    char *input1 = "fsdgdfhgfhghkkjhkkghtryt67rhrtu";
    char *input2 = "rtde886";
    char *input3 = "*********************************************";
    char *input4 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

    int response1 = validCode(input1);
    int response2 = validCode(input2);
    int response3 = validCode(input3);
    int response4 = validCode(input4);

    CuAssertTrue(tc, !response1);
    CuAssertTrue(tc, !response2);
    CuAssertTrue(tc, !response3);
    CuAssertTrue(tc, !response4);
}



/*-------------------------------------------------------------------------*
 * CharsetSuite Test
 *-------------------------------------------------------------------------*/


CuSuite* CharsetGetSuite() {
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestValidCode);
    SUITE_ADD_TEST(suite, TestValidCode_Exceptions);
    SUITE_ADD_TEST(suite, TestValidCode_Invalid_Chars);
    SUITE_ADD_TEST(suite, TestValidCode_Short);
    SUITE_ADD_TEST(suite, TestValidCode_Long);

    return suite;
}
