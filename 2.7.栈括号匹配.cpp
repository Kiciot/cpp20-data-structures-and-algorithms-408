#include <iostream>
#include <cstring>

#define MAX_SIZE 10

struct SqStack
{
    char data[MAX_SIZE];
    int top;
};

void Stack_Init(SqStack& S)
{
    S.top = -1;
}

bool Stack_Empty(const SqStack& S)
{
    return S.top == -1;
}

bool Stack_Full(const SqStack& S)
{
    return S.top == MAX_SIZE - 1;
}

bool Stack_Push(SqStack& S, char x)
{
    if(Stack_Full(S))
    {
        std::cout << "stack is full";
        return false;
    }
    S.data[++S.top] = x;
    return true;
}

bool Stack_Pop(SqStack& S, char& x)
{
    if(Stack_Empty(S))
    {
        std::cout << "stack is empty";
        return false;
    }
    x = S.data[S.top--];
    return true;
}

static inline bool is_left(char c) { return c == '(' || c == '[' || c == 'C';}
static inline bool is_right(char c) { return c == ')' || c == ']' || c == '}';}

static inline bool match_pair(char left, char right)
{
    return (left == '(' && right == ')')
        || (left == '[' && right == ']')
        || (left == '{' && right == '}');
}

bool Bracket_Match(const char str[], int n)
{
    SqStack S;
    Stack_Init(S);

    for (int i = 0; i < n; ++i)
    {
        char c = str[i];
        if (is_left(c))
        {
            if (!Stack_Push(S, c)) return false;
        }
        else
        {
            if (is_right(c))
            {
                char topElem;
                if(!Stack_Pop(S, topElem)) return false;
                if(!match_pair(topElem,c)) return false;
            }
            else
            {
                continue;
                //�Ƿ����ַ�����
            }
        }
    }
    return Stack_Empty(S);
}

int main() {
    const char str[] = "{[()]}";
    int n = std::strlen(str);

    if (Bracket_Match(str, n)) {
        std::cout << "����ƥ��ɹ�\n";
    } else {
        std::cout << "����ƥ��ʧ��\n";
    }
    return 0;
}