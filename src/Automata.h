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
        {
            precedence = 3;
        } break;
        
        case '?':
        {
            precedence = 3;
        } break;
        
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
    ArrayList<char> postfix;
    ArrayStack<char> operators;

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
            while (operators.List.Size)
            {
                char op = operators.Pop();
                postfix.Add(op);
            }
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
                else // concat operator has less precedence then top of stack pop from stack until lower precedence is found
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


#endif