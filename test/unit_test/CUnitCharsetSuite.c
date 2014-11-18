#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <CUnit/CUnit.h>

#include "charset.h"





/*-------------------------------------------------------------------------*
 * ValidCode Test
 *-------------------------------------------------------------------------*/

void test_valid_code() 
{
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

    CU_ASSERT(validCode(input1));
    CU_ASSERT(validCode(input2));
    CU_ASSERT(validCode(input3));
    CU_ASSERT(validCode(input4));
    CU_ASSERT(validCode(input5));
    CU_ASSERT(validCode(input6));
    CU_ASSERT(validCode(input7));
    CU_ASSERT(validCode(input8));
    CU_ASSERT(validCode(input9));
    CU_ASSERT(validCode(input10));
}

void test_valid_code_exceptions() 
{
    char *input1 = "0abcmn";
    char *input2 = "OfrtX6";
    char *input3 = "1frtXu";
    char *input4 = "1frtX6";
    char *input5 = "dfr8X5";
    char *input6 = "yDSyr8";

    CU_ASSERT_FALSE(validCode(input1));
    CU_ASSERT_FALSE(validCode(input2));
    CU_ASSERT_FALSE(validCode(input3));
    CU_ASSERT_FALSE(validCode(input4));
    CU_ASSERT_FALSE(validCode(input5));
    CU_ASSERT_FALSE(validCode(input6));
}

void test_valid_code_invalid_chars()
{
    char *input1 = "*abcmn";
    char *input2 = "Ofrt_6";
    char *input3 = "******";
    char *input4 = "'rptX6";
    char *input5 = "df¿?-g";
    char *input6 = "<ñsu 3";

    CU_ASSERT_FALSE(validCode(input1));
    CU_ASSERT_FALSE(validCode(input2));
    CU_ASSERT_FALSE(validCode(input3));
    CU_ASSERT_FALSE(validCode(input4));
    CU_ASSERT_FALSE(validCode(input5));
    CU_ASSERT_FALSE(validCode(input6));
}

void test_valid_code_short()
{
    char *input1 = "";
    char *input2 = "rtde6";
    char *input3 = "***";
    char *input4 = "m";

    CU_ASSERT_FALSE(validCode(input1));
    CU_ASSERT_FALSE(validCode(input2));
    CU_ASSERT_FALSE(validCode(input3));
    CU_ASSERT_FALSE(validCode(input4));
}

void test_valid_code_long() 
{
    char *input1 = "fsdgdfhgfhghkkjhkkghtryt67rhrtu";
    char *input2 = "rtde886";
    char *input3 = "*********************************************";
    char *input4 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

    CU_ASSERT_FALSE(validCode(input1));
    CU_ASSERT_FALSE(validCode(input2));
    CU_ASSERT_FALSE(validCode(input3));
    CU_ASSERT_FALSE(validCode(input4));
}



/*-------------------------------------------------------------------------*
 * Charset Suite
 *-------------------------------------------------------------------------*/

int init_suiteCharset(void)
{
    return 0;
}

int clean_suiteCharset(void)
{
    return 0;
}