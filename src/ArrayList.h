#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include <stdlib.h>

template <typename T>
struct ArrayList
{
    T* Data;
    int Size;
    int Capacity;

    void Init(int size, int capacity)
    {
        Size = size;
        Capacity = capacity;
        Data = (T*)malloc(sizeof(T) * Capacity);
    }

    void Free()
    {
        free(Data);
        Data = 0;
        Size = 0;
        Capacity = 0;
    }
    
    ArrayList<T> Copy()
    {
        ArrayList<T> copy = *this;
        copy.Data = (T*)malloc(sizeof(T) * Capacity);
        for (int i = 0; i < Size; i++)
        {
            copy.Data[i] = Data[i];
        }
        
        return copy;
    }

    void Add(T element)
    {
        if (Size + 1 > Capacity)
        {
            Capacity = Capacity * 2 + 1;
            T* newData = (T*)malloc(sizeof(T) * Capacity);
            for (int i = 0; i < Size; i++)
            {
                newData[i] = Data[i];
            }
            free(Data);
            Data = newData;
            
        }
        
        Data[Size++] = element;

    }

    
    void AddAtIndex(int index, T element)
    {
        if (Size + 1 > Capacity)
        {
            Capacity = Capacity * 2 + 1;
            T* newData = (T*)malloc(sizeof(T) * Capacity);
            for (int i = 0; i < Size; i++)
            {
                newData[i] = Data[i];
            }
            free(Data);
            Data = newData;
            
        }
        Size++;

        for (int i = Size - 1; i > index; i--)
        {
            Data[i] = Data[i - 1];
        }
        
        Data[index] = element;

    }

    
    void DeleteLast()
    {
        Data[Size - 1] = {0};
        Size--;
    }

    
    void DeleteByIndex(int index)
    {
        if(index > Size || index < 0)
        {
            return;
        }

        Data[index] = 0;

        for (int i = index; i < Size - 1; i++)
        {
            Data[i] = Data[i + 1];
        }
        
        Data[Size - 1] = 0;
        Size--;
    }
};


#ifdef ARRAYLIST_EXAMPLE
void ArrayListExample()
{
    ArrayList list = ArrayList<double>();
    
    list.Add(1);
    list.Add(3);
    list.Add(4);
    list.Add(6);
    list.Add(9);
    list.Add(12);

    list.AddAtIndex(3, 5);
    list.AddAtIndex(3, 11);

    list.DeleteByIndex(3);
    list.DeleteLast();

    list.Free();
}
#endif

#endif