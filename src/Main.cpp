#include <crtdbg.h>
#define ARRAYLIST_EXAMPLE
#include "ArrayList.h"
#define ARRAYQUEUE_EXAMPLE
#include "ArrayQueue.h"
#define ARRAYSTACK_EXAMPLE
#include "ArrayStack.h"

int main ()
{
   _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

    ArrayListExample();
    ArrayQueueExample();
    ArrayStackExample();

    return 0;
}
