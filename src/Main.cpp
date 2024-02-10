#include <crtdbg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Automata.h"

#define ArrayCount(x) sizeof((x)) / sizeof((x)[0])

int main ()
{
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
    char* regexesForTest[] =
    {
      "a", // single char
      "abc", // just concat
      "a|b", // just alternation
      "a*", // just zero or more
      "a+", // just one or more
      "a?", // just zero or one
      "(a)", // just paranthesis
      "(abc|bca|cab)", // multi alternation
      "(abc|bca|cab)*nak?klm+pnb+(gan*|ang?|nga+)", // everything
    };

    char* calculatedPostfixes[] = 
    {
        "a",
        "ab-c-",
        "ab|",
        "a*",
        "a+",
        "a?",
        "a",
        "ab-c-bc-a-|ca-b-|",
        "ab-c-bc-a-|ca-b-|*n-a-k?-k-l-m+-p-n-b+-ga-n*-an-g?-|ng-a+-|-",
    };

    for (int i = 0; i < ArrayCount(regexesForTest); i++)
    {
        char* postfix = Regex2Postfix(regexesForTest[i]);

        if (strcmp(postfix, calculatedPostfixes[i]) == 0)
        {
            printf("%s regex %d matches with its postfix \n", "T", i);
        }
        else
        {
            printf("%s regex %d failed to match with its postfix \n", "F", i);
        }

        free(postfix);
    }
    

    return 0;
}
