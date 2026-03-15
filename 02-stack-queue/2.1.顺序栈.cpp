#include <iostream>

#define MAX_SIZE 10

struct SqStack
{
    char data[MAX_SIZE];
    int top;
};

void Stack_Init(SqStack &S)
{
    S.top = -1;
}

bool Stack_Empty(SqStack &S)
{
    return S.top == -1;
}

bool Stack_Full(SqStack &S)
{
    return S.top == MAX_SIZE;
}

bool Stack_Push(SqStack &S, char x)
{
    if(Stack_Full(S))
    {
        std::cout << "Stack is full" << std::endl;
    }
    else
    {
        S.data[++S.top] = x;
        return true;
    }
}

bool Stack_Pop(SqStack &S, char &X)
{
    if(Stack_Empty(S))
    {
        std::cout << "stack is empty" << std::endl;
        return false;
    }
    else
    {
        X = S.data[S.top--];
        return true;
    }
}

