#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "posixtest.h"

int main()
{
    char str[] = "ab,cd,ef";
    const char *str_sp[] = {"ab", "cd", "ef"};
    char *ptr;

    ptr = strtok(str, ",");

    int i;
    while (ptr != NULL)
    {
        if (strcmp(ptr, str_sp[i++]))
        {
            perror("strstr fail\n");
            return PTS_UNRESOLVED;
        }
        ptr = strtok(NULL, ",");
    }

    printf("{Test PASSED}\n");
    return PTS_PASS;
}