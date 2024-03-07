// Construct an NFA that accepts a particular regex.
#ifndef AUTOMATATEST_H
#define AUTOMATATEST_H

#include "ArrayQueue.hpp"
#include "ArrayStack.hpp"
#include "ArrayQueue.hpp"
#define BIT_COUNT(x) (sizeof(x) * 8)

// returns values > 0 if key is a regex op, 0 if key is not a regex op.
int IsRegexOperator(char key)
{
    int precedence = 0;
    
    switch (key)
    {
        case '*':      
        case '?':
        case '+':
        {
            precedence = 3;
        } break;
        
        case '-':
        {
            precedence = 2;
        } break;
        
        case '|':
        {
            precedence = 1;
        } break;
        
        default:
            break;
    }

    return precedence;
}

char* Regex2Postfix(char* regex)
{
    ArrayList<char> postfix = {0};
    ArrayStack<char> operators = {0};

    if(!regex)
    {
        return 0;
    }

    bool implicitOperatorFound = false;
    bool escapeCharacterFound = false;

    for (int i = 0; regex[i]; i++)
    {
        char currentChar = regex[i];
        char nextChar = regex[i + 1];
        int currentPrecedence = IsRegexOperator(currentChar) == 2 ? 0: IsRegexOperator(currentChar); 
        // it is impossible to read a concat operator because it is an implicit operator.
        int nextPrecedence = IsRegexOperator(nextChar) == 2 ? 0 : IsRegexOperator(nextChar);
                
        if (implicitOperatorFound) // concat operator is the implicit operator.
        {
            nextChar = currentChar;
            currentChar = '-';
            currentPrecedence = IsRegexOperator(currentChar);
            nextPrecedence = IsRegexOperator(nextChar) == 2 ? 0 : IsRegexOperator(nextChar);
            i--;
            implicitOperatorFound = false;
        }

        if (currentPrecedence) // if current char is an operator
        {
            // if the current operator has higher precedence than the top element of the operators push it on stack
            if (operators.List.Size == 0 || currentPrecedence > IsRegexOperator(operators.Peek()))
            {
                operators.Push(currentChar);
            }
            else // current operator has less precedence then top of stack pop from stack until lower precedence is found
            {
                while (operators.List.Size && currentPrecedence <= IsRegexOperator(operators.Peek()))
                {
                    char op = operators.Pop();
                    postfix.Add(op);
                }
                operators.Push(currentChar);
            }
            
            bool unaryOperator = currentPrecedence > 2;

            if (unaryOperator && !nextPrecedence)
            {
                implicitOperatorFound = true;
            }
            
        }
        else if (escapeCharacterFound)
        {
            postfix.Add(regex[i]);
            
            // nextchar is not an operator, zero char, right parenthesis
            if (!nextPrecedence && nextChar && nextChar != ')')
            {
                implicitOperatorFound = true;
            }

            escapeCharacterFound = false;
        }
        else if (currentChar == '(')
        {
            operators.Push('(');
        }
        else if (currentChar == '-')
        {
            postfix.Add('\\');
            postfix.Add('-');

            // nextchar is not an operator, zero char, right parenthesis
            if (!nextPrecedence && nextChar && nextChar != ')')
            {
                implicitOperatorFound = true;
            }
        }
        else if (currentChar == '\\')
        {
            postfix.Add(regex[i]);
            escapeCharacterFound = true;
        }
        else if (currentChar == ')')
        {
            while (operators.Peek() != '(')
            {
                char op = operators.Pop();
                postfix.Add(op);
            }
            operators.Pop();              
        }
        else // if current char is an operand
        {
            postfix.Add(regex[i]);
            
            // nextchar is not an operator, zero char, right parenthesis
            if (!nextPrecedence && nextChar && nextChar != ')')
            {
                implicitOperatorFound = true;
            }
        }
    }

    while (operators.List.Size)
    {
        char op = operators.Pop();
        postfix.Add(op);
    }
    
    operators.Free();
    postfix.Add('\0');
    
    return postfix.Data;
}

enum StateStatus
{
    STATE_NONE = 0,
    STATE_START,
    STATE_END,
    STATE_STATUS_COUNT,
};

struct Transition;

struct State     
{
    int Id;
    int ListId;
    StateStatus Status;
    int NumberOfTransitions;
    Transition* Transitions;
};

union CharSet
{
    // Character set for ASCII
    unsigned long long Set[4];
};

struct Transition
{
    State* Out;
    CharSet ReadableChars;
};

State* StateInit(int transitionNumber)
{
    State* result = (State*)malloc(sizeof(State));
    *result = {0};
    result->NumberOfTransitions = transitionNumber;
    result->Transitions = (Transition*)malloc(sizeof(Transition) * transitionNumber);

    for (int i = 0; i < result->NumberOfTransitions; i++)
    {
        result->Transitions[i] = {0};
    }

    return result;
}

void StateFree(State* state)
{
    if (state->Transitions)
    {
        free(state->Transitions);
    }
    
    free(state);
}

struct TransitionList
{
    Transition** Transitions;
    int Size;
};

TransitionList* TransitionListInit(Transition* transition)
{
    TransitionList* list = (TransitionList*)malloc(sizeof(TransitionList));
    list->Size = 1;
    list->Transitions = (Transition**)malloc(sizeof(Transition*) * 1);
    list->Transitions[0] = transition;

    return list;
}

TransitionList* TransitionListAppend(TransitionList* list1, TransitionList* list2)
{
    TransitionList* list = (TransitionList*)malloc(sizeof(TransitionList));
    list->Size = list1->Size + list2->Size;
    list->Transitions = (Transition**)malloc(sizeof(Transition*) * list->Size);

    for (int i = 0; i < list1->Size; i++)
    {
        list->Transitions[i] = list1->Transitions[i];
    }
    
    for (int i = list1->Size; i < list2->Size + list1->Size; i++)
    {   
        list->Transitions[i] = list2->Transitions[i - list1->Size];
    }
    
    free(list1->Transitions);
    free(list2->Transitions);
    free(list1);
    free(list2);

    return list;
}

void TransitionListPatch(TransitionList* list, State* state)
{
    for (int i = 0; i < list->Size; i++)
    {
        list->Transitions[i]->Out = state;
    }
}

void TransitionListFree(TransitionList* list)
{
    free(list->Transitions);
    free(list);
}

struct NFAFragment
{
    State* Start;
    TransitionList* DanglingTransitions; // a dangling transition is a transition with no out state specified.
};

// NFA is an automata(graph) built with NFAFragments
struct NFA
{
    State* Start;
    int Size;
};

void NFAFree(NFA graph)
{
    ArrayList<bool> visitedList = {0};
    visitedList.Init(graph.Size, graph.Size);
    ArrayList<State*> freeList = {0};
    ArrayStack<State*> stack = {0};
    stack.Push(graph.Start);

    for (int i = 0; stack.List.Size; i++)
    {
        State* current = stack.Pop();
        visitedList.Data[current->Id] = true;
        freeList.Add(current);   

        for (int j = 0; j < current->NumberOfTransitions; j++)
        {
            if (!visitedList.Data[current->Transitions[j].Out->Id])
            {  
                stack.Push(current->Transitions[j].Out);    
            }
            
        }
        
    }
    
    visitedList.Free();
    stack.Free();

    for (int i = 0; i < freeList.Size; i++)
    {
        State* state = freeList.Data[i];
        StateFree(state);
    }
    
    freeList.Free();
}

NFA Postfix2NFA(char* postfix)
{
    ArrayStack<NFAFragment> stack = {0};
	NFAFragment e0 = {0};
	NFAFragment e1 = {0};
	NFAFragment e2 = {0};
    NFA result = {0};

    State* state = {0};
    int counter = 0;

    unsigned int zeroSetIndex = '\0' / BIT_COUNT(long long); 
    unsigned long long zeroBitIndex = (long long) 1 << ('\0' % BIT_COUNT(long long)); 

	for (int i = 0; postfix[i]; i++)
    {
        switch (postfix[i])
        {
            case '-':
            {
                e2 = stack.Pop();
                e1 = stack.Pop();
                
                TransitionListPatch(e1.DanglingTransitions, e2.Start);
                TransitionListFree(e1.DanglingTransitions);

                NFAFragment frag = {0};
                frag.Start = e1.Start;
                frag.DanglingTransitions = e2.DanglingTransitions;             

                stack.Push(frag);

            } break;
            
            case '|':
            {
                e2 = stack.Pop();
                e1 = stack.Pop();
                
                state = StateInit(2);
                state->Transitions[0].Out = e1.Start;
                state->Transitions[1].Out = e2.Start;

                // setting up readable characters the new state can read
                // this new state reads the '\0' character
                state->Transitions[0].ReadableChars.Set[zeroSetIndex] = zeroBitIndex;
                state->Transitions[1].ReadableChars.Set[zeroSetIndex] = zeroBitIndex;

                state->Id = counter++;

                NFAFragment frag = {0};
                frag.Start = state;
                frag.DanglingTransitions = TransitionListAppend(e1.DanglingTransitions, e2.DanglingTransitions);

                stack.Push(frag);
            } break;
            
            case '?':
            {
                e0 = stack.Pop();

                state = StateInit(2);
                state->Transitions[0].Out = e0.Start;

                // setting up readable characters the new state can read
                // this new state reads the '\0' character
                state->Transitions[0].ReadableChars.Set[zeroSetIndex] = zeroBitIndex;
                state->Transitions[1].ReadableChars.Set[zeroSetIndex] = zeroBitIndex;

                state->Id = counter++;

                NFAFragment frag = {0};
                frag.Start = state;
                frag.DanglingTransitions = TransitionListAppend(e0.DanglingTransitions, TransitionListInit(&state->Transitions[1]));

                stack.Push(frag);
            } break;
            
            case '*':
            {
                e0 = stack.Pop();
                
                state = StateInit(2);
                state->Transitions[0].Out = e0.Start;

                // setting up readable characters the new state can read
                // this new state reads the '\0' character
                state->Transitions[0].ReadableChars.Set[zeroSetIndex] = zeroBitIndex;
                state->Transitions[1].ReadableChars.Set[zeroSetIndex] = zeroBitIndex;

                state->Id = counter++;

                TransitionListPatch(e0.DanglingTransitions, state);
                TransitionListFree(e0.DanglingTransitions);

                NFAFragment frag = {0};
                frag.Start = state;
                frag.DanglingTransitions = TransitionListInit(&state->Transitions[1]);

                stack.Push(frag);
            } break;
            
            case '+':
            {
                e0 = stack.Pop();

                state = StateInit(2);
                state->Transitions[0].Out = e0.Start;

                // setting up readable characters the new state can read
                state->Transitions[0].ReadableChars.Set[zeroSetIndex] = zeroBitIndex;
                state->Transitions[1].ReadableChars.Set[zeroSetIndex] = zeroBitIndex;

                state->Id = counter++;

                TransitionListPatch(e0.DanglingTransitions, state);
                TransitionListFree(e0.DanglingTransitions);

                NFAFragment frag = {0};
                frag.Start = e0.Start;
                frag.DanglingTransitions = TransitionListInit(&state->Transitions[1]);

                stack.Push(frag);

            } break;

            case '.':
            {
                unsigned long long maxUnsigned64 = (unsigned long long)(-1);
                state = StateInit(1);

                // setting up readable characters the new state can read
                // this state can read all characters as it is the any character operator
                // except for the '\0' character
                state->Transitions[0].ReadableChars.Set[0] = maxUnsigned64 ^ zeroBitIndex;
                state->Transitions[0].ReadableChars.Set[1] = maxUnsigned64;
                state->Transitions[0].ReadableChars.Set[2] = maxUnsigned64;
                state->Transitions[0].ReadableChars.Set[3] = maxUnsigned64;
            
                state->Id = counter++;

                NFAFragment frag = {0};
                frag.Start = state;
                frag.DanglingTransitions = TransitionListInit(&state->Transitions[0]);

                stack.Push(frag);
            } break;

            case '\\':
            {
                if (!IsRegexOperator(postfix[i + 1]) && postfix[i + 1] != '\\')
                {
                    continue;
                }
                
                unsigned int charSetIndex = postfix[i + 1] / BIT_COUNT(long long); 
                unsigned long long bitIndex = (long long) 1 << (postfix[i + 1] % BIT_COUNT(long long)); 
                state = StateInit(1);

                // setting up readable characters the new state can read
                // this state reads the specified character
                state->Transitions[0].ReadableChars.Set[charSetIndex] = bitIndex;
            
                state->Id = counter++;

                NFAFragment frag = {0};
                frag.Start = state;
                frag.DanglingTransitions = TransitionListInit(&state->Transitions[0]);

                stack.Push(frag);
                i++;
            } break;

            default:
            {
                unsigned int charSetIndex = postfix[i] / BIT_COUNT(long long); 
                unsigned long long bitIndex = (long long) 1 << (postfix[i] % BIT_COUNT(long long)); 
                state = StateInit(1);

                // setting up readable characters the new state can read
                // this state reads the specified character
                state->Transitions[0].ReadableChars.Set[charSetIndex] = bitIndex;
            
                state->Id = counter++;

                NFAFragment frag = {0};
                frag.Start = state;
                frag.DanglingTransitions = TransitionListInit(&state->Transitions[0]);

                stack.Push(frag);
            } break;
        }
    }
    
    e0 = stack.Pop();
    stack.Free();

    state = StateInit(0);
    state->Status = STATE_END;

    state->Id = counter;

    TransitionListPatch(e0.DanglingTransitions, state);
    TransitionListFree(e0.DanglingTransitions);

    result.Start = e0.Start;
    result.Size = counter + 1;

    return result;
}

void AddState(ArrayList<State*>* list, State* state, int listId)
{
    ArrayQueue<State*> queue = {0};
    queue.Push(state);

    for (;queue.List.Size > 0;)
    {
        State* current = queue.Pop();

        if (!current || current->ListId == listId)
        {
            continue;
        }
        
        current->ListId = listId;
        
        if (current->NumberOfTransitions == 2)
        {
            queue.Push(current->Transitions[0].Out);
            queue.Push(current->Transitions[1].Out);
            continue;
        }
        
        list->Add(current);
    }

    queue.Free();
}

int IsMatch(ArrayList<State*>* currentStates)
{
    for (int i = 0; i < currentStates->Size; i++)
    {
        if (currentStates->Data[i]->Status == STATE_END)
        {
            return 1;
        }
    }
    
    return 0;
}

int Match(NFA graph, char* input)
{
    ArrayList<State*> currentStates = {0};
    ArrayList<State*> nextStates = {0};
    currentStates.Init(0, graph.Size);
    nextStates.Init(0, graph.Size);
    int match = 0;

    currentStates.Add(graph.Start);
    static int listId = 0;
    bool charRead = false;

    for (; *input; input++)
    {
        unsigned int charSetIndex = *input / BIT_COUNT(long long); 
        unsigned int zeroSetIndex = '\0' / BIT_COUNT(long long); 
        unsigned long long bitIndex = (long long) 1 << (*input % BIT_COUNT(long long)); 
        unsigned long long zeroIndex = (long long) 1 << ('\0' % BIT_COUNT(long long)); 
        listId++;
        nextStates.Size = 0;
        for (int i = 0; i < currentStates.Size; i++)
        {
            State* currentState = currentStates.Data[i];
            for (int j = 0; j < currentState->NumberOfTransitions; j++)
            {
                Transition currentTransition = currentState->Transitions[j];
                if (currentTransition.ReadableChars.Set[charSetIndex] & bitIndex)
                {
                    charRead = true;
                    AddState(&nextStates, currentTransition.Out, listId);
                }
                else if (currentTransition.ReadableChars.Set[zeroSetIndex] & zeroIndex)
                {
                    AddState(&nextStates, currentTransition.Out, listId);
                }
            }
        }

        if (!charRead)
        {
            input--;
        }
        charRead = false;

        ArrayList<State*> temp = currentStates;
        currentStates = nextStates;
        nextStates = temp;
    }

    match = IsMatch(&currentStates);
    
    currentStates.Free();
    nextStates.Free();

    return match;
}

#endif