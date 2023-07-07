#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "posixtest.h"

#define uassert_int_equal(a, b) assert((a) == (b))

struct atol_data
{
    char string[15]; // long max 2147483647
    long int ret_num;
};

struct atol_data test_data1[] =
    {
        /* positive integer */
        {"0", 0},
        {"1", 1},
        {"1.123", 1},
        {"123", 123},
        {"98993489", 98993489},
        {"98993489.12", 98993489},
        {"2147483647", 2147483647},

        /* negtive integer */
        {"-1", -1},
        {"-1.123", -1},
        {"-123", -123},
        {"-98993489", -98993489},
        {"-98993489.12", -98993489},
        {"-2147483647", -2147483647},

        /* letters and numbers */
        {"12a45", 12},
        {"-12a45", -12},
        {"12/45", 12},
        {"-12/45", -12},

        /* cannot be resolved */
        {"", 0},
        {" ", 0},
        {"abc12", 0},
        {" abc12", 0},
        {NULL, -1}};

int atol_entry(void)
{
    int i = 0;
    long int res = 0;
    for (i = 0; i < sizeof(test_data1) / sizeof(test_data1[0]); i++)
    {
        res = atol(test_data1[i].string);
        printf("%ld\n", res);
        uassert_int_equal(res, test_data1[i].ret_num);
    }

    printf("{Test PASSED}\n");
    return PTS_PASS;
}

int main()
{
    atol_entry();
    return 0;
}
