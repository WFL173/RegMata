#ifndef ARRAYSTACK_H
#define ARRAYSTACK_H
#include "ArrayList.h"

template<typename T>
struct ArrayStack
{
    ArrayList<T> List;

    ArrayStack()
    {

    }

    void Free()
    {
        List.Free();
    }

    void Push(T element)
    {
        List.Add(element);
    }

    T Pop()
    {

        T popResult = {0};
        if (List.Size > 0)
        {   
            popResult = List.Data[List.Size -1];
            List.DeleteLast();
        }
        return popResult;
    }

    T Peek()
    {
        return List.Data[List.Size -1];
    }

};

#ifdef ARRAYSTACK_EXAMPLE
void ArrayStackExample()
{
    ArrayStack stack = ArrayStack<int>();
    int element = 0;

    stack.Push(10);
    stack.Push(2);
    stack.Push(30);
    stack.Push(50);
    stack.Push(10);

    element = stack.Pop();
    element = stack.Pop();
    element = stack.Pop();
    element = stack.Pop();
    element = stack.Pop();
}
#endif

#endif