#include <crtdbg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Automata.h"

#define ArrayCount(x) sizeof((x)) / sizeof((x)[0])

int main ()
{
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

    ArrayList<int> list = {0};
    list.Add(1);
    list.Add(2);
    list.Add(3);
    list.Add(4);

    char regex[] = "aaba*";

    char *postfix = Regex2Postfix(regex);

    NFA graph = Postfix2NFA(postfix);

    free(postfix);
    NFAFree(graph);

    return 0;
}
