// Construct an NFA that accepts a particular regex.
#ifndef AUTOMATATEST_H
#define AUTOMATATEST_H

#include "ArrayStack.h"

// returns values > 0 if key is a regex op, 0 if key is not a regex op.
int IsBinaryRegexOperator(char key)
{
    int precedence = 0;
    
    switch (key)
    {
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

    for (int i = 0; regex[i]; i++)
    {
        char currentChar = regex[i];
        char nextChar = regex[i + 1];
        int currentCharPrecedence = IsBinaryRegexOperator(currentChar); 
        int nextCharIsOperator = IsRegexOperator(nextChar);
        
        if (currentCharPrecedence) // if current char is a binary operator
        {
            // if the current operator has higher precedence than the top element of the operators push it on stack
            if (operators.List.Size == 0 || currentCharPrecedence > IsBinaryRegexOperator(operators.Peek()))
            {
                operators.Push(currentChar);
            }
            else // current operator has less precedence then top of stack pop from stack until lower precedence is found
            {
                while (operators.List.Size && currentCharPrecedence <= IsBinaryRegexOperator(operators.Peek()))
                {
                    char op = operators.Pop();
                    postfix.Add(op);
                }
                operators.Push(currentChar);
            }
            
        }
        else if (currentChar == '(')
        {
            // while (operators.List.Size)
            // {
            //     char op = operators.Pop();
            //     postfix.Add(op);
            // }
            operators.Push('(');
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
            // if next char is an operand and not the zero character and not right parentheses
            if (!nextCharIsOperator && nextChar && nextChar != ')') 
            {
                // push concat operator onto the operator stack.
                // if the concat operator has higher precedence than the top element of the operators push it onto stack
                if (operators.List.Size == 0 || IsBinaryRegexOperator('-') > IsBinaryRegexOperator(operators.Peek()))
                {
                    operators.Push('-');
                }
                else // concat operator has less precedence then pop from top of stack stack until lower precedence is found
                {
                    while (operators.List.Size && IsBinaryRegexOperator('-') <= IsBinaryRegexOperator(operators.Peek()))
                    {
                        char op = operators.Pop();
                        postfix.Add(op);
                    }
                    operators.Push('-');
                }
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

struct Transition
{
    State* Out;
    unsigned int CharToBeRead;
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
    TransitionList* DanglingTransitions;
    //Transition* DanglingTransitions[2]; // a dangling transition is a transition with no out state specified.
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

	for (int i = 0; postfix[i]; i++)
    {
        switch (postfix[i])
        {
            case '-':
            {
                e2 = stack.Pop();
                e1 = stack.Pop();
                
                // e1.DanglingTransitions[0]->Out = e2.Start;
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

                state->Id = counter++;

                NFAFragment frag = {0};
                frag.Start = state;
                frag.DanglingTransitions = TransitionListAppend(e1.DanglingTransitions, e2.DanglingTransitions);
                // frag.DanglingTransitions[0] = e1.DanglingTransitions[0];
                // frag.DanglingTransitions[1] = e2.DanglingTransitions[0];

                stack.Push(frag);
            } break;
            
            case '?':
            {
                e0 = stack.Pop();
                state = StateInit(2);

                state->Id = counter++;

                state->Transitions[0].Out = e0.Start;

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

                state->Id = counter++;

                TransitionListPatch(e0.DanglingTransitions, state);
                TransitionListFree(e0.DanglingTransitions);
                // e0.DanglingTransitions[0]->Out = state;

                NFAFragment frag = {0};
                frag.Start = state;
                frag.DanglingTransitions = TransitionListInit(&state->Transitions[1]);
                // frag.DanglingTransitions[0] = &state->Transitions[1];

                stack.Push(frag);
            } break;
            
            case '+':
            {
                e0 = stack.Pop();

                state = StateInit(2);
                state->Transitions[0].Out = e0.Start;

                state->Id = counter++;

                TransitionListPatch(e0.DanglingTransitions, state);
                TransitionListFree(e0.DanglingTransitions);

                NFAFragment frag = {0};
                frag.Start = e0.Start;
                frag.DanglingTransitions = TransitionListInit(&state->Transitions[1]);

                stack.Push(frag);

            } break;

            default:
            {
                state = StateInit(1);
                state->Transitions[0].CharToBeRead = postfix[i];
            
                state->Id = counter++;

                NFAFragment frag = {0};
                frag.Start = state;
                frag.DanglingTransitions = TransitionListInit(&state->Transitions[0]);
                // frag.DanglingTransitions[0] = &state->Transitions[0];

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
    if (!state || state->ListId == listId)
    {
        return;
    }
    state->ListId = listId;
    if (state->NumberOfTransitions == 2)
    {
        AddState(list, state->Transitions[0].Out, listId);
        AddState(list, state->Transitions[1].Out, listId);
        return;
    }
    list->Add(state);
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
        listId++;
        nextStates.Size = 0;
        for (int i = 0; i < currentStates.Size; i++)
        {
            State* currentState = currentStates.Data[i];
            for (int j = 0; j < currentState->NumberOfTransitions; j++)
            {
                Transition currentTransition = currentState->Transitions[j];
                if (currentTransition.CharToBeRead == *input)
                {
                    charRead = true;
                    AddState(&nextStates, currentTransition.Out, listId);
                }
                else if (currentTransition.CharToBeRead == '\0')
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