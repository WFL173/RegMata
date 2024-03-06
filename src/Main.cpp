#include <crtdbg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Automata.hpp"

#define ArrayCount(x) sizeof((x)) / sizeof((x)[0])

#define RE_OKAY(message, ...) printf("[+] " message "\n", ##__VA_ARGS__)
#define RE_ERROR(message, ...) printf("[-] " message "\n", ##__VA_ARGS__)
#define RE_WARNING(message, ...) printf("[!] " message "\n", ##__VA_ARGS__)
#define RE_INFO(message, ...) printf("[I] " message "\n", ##__VA_ARGS__)

int main ()
{
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

    char regex[] = "a--?-s*";

    char *postfix = Regex2Postfix(regex);

    NFA graph = Postfix2NFA(postfix);

    int matched = Match(graph, "a--sss");
    RE_INFO("match result: the regex: %s, %s with string: %s", regex, matched ? "matched":"did not match", "a--sss");
    matched = Match(graph, "a");
    RE_INFO("match result: the regex: %s, %s with string: %s", regex, matched ? "matched":"did not match", "a");
    matched = Match(graph, "a---sss");
    RE_INFO("match result: the regex: %s, %s with string: %s", regex, matched ? "matched":"did not match", "a---sss");

    free(postfix);
    NFAFree(graph);

    return 0;
}
