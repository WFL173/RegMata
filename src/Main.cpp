#include <crtdbg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Automata.h"

#define ArrayCount(x) sizeof((x)) / sizeof((x)[0])

int main ()
{
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

    char regex[] = "abcd+e";

    char *postfix = Regex2Postfix(regex);

    NFA graph = Postfix2NFA(postfix);

    int matched = Match(graph, "ab");
    matched = Match(graph, "abce");
    matched = Match(graph, "abcddddde");

    free(postfix);
    NFAFree(graph);

    return 0;
}
