#ifndef ARRAYQUEUE_H
#define ARRAYQUEUE_H
#include "ArrayList.h"

template<typename T>
struct ArrayQueue
{
    ArrayList<T> List;

    void Free()
    {
        List.Free();
    }

    void Push(T element)
    {
        List.AddAtIndex(0, element);
    }

    T Pop()
    {
        T popResult = {0};
        if (List.Size > 0)
        {
            popResult = List.Data[List.Size - 1];
            List.DeleteLast();
        }
        
        return popResult;

    }

    T Peek()
    {
        return List.Data[List.Size -1];
    }
};

#ifdef ARRAYQUEUE_EXAMPLE
void ArrayQueueExample()
{
    ArrayQueue queue = ArrayQueue<int>();

    int pop = 0;

    queue.Push(4);
    queue.Push(2);
    queue.Push(1);

    pop = queue.Peek();
    pop = queue.Pop();
    pop = queue.Pop();
    pop = queue.Pop();

    queue.Free();
}
#endif

#endif
