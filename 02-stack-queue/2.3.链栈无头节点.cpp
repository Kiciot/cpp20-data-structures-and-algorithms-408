#include <iostream>

struct LNode
{
    int data;
    LNode* next;
};

using LinkStack = LNode*;

bool Stack_Init(LinkStack& pStack)
{
    pStack = nullptr;
    return true;
}

bool Stack_Empty(const LinkStack pStack)
{
    return pStack == nullptr;
}

int Stack_Length(const LinkStack pStack)
{
    int Length = 0;
    for(LNode* cur = pStack; cur != nullptr; cur = cur->next) ++Length;
    return Length;
}

LinkStack Stack_Create(LinkStack& pStack)
{
    int x;
    while(std::cin >> x && x != 999)
    {
        LNode* node = new LNode{x, pStack};
        pStack = node;
    }
    return pStack;
}

bool Stack_Push(LinkStack& pStack, int e)
{
    LNode* node = new LNode{e, pStack};
    pStack = node;
    return true;
}

bool Stack_Pop(LinkStack& pStack, int& e)
{
    if(!pStack) return false;
    LNode* top = pStack;
    e = top->data;
    pStack = top->next;
    delete top;
    return true;
}

LNode* Stack_Get_Top(const LinkStack pStack)
{
    return pStack;
}

void Stack_Show(const LinkStack pStack)
{
    std::cout << "value: ";
    for(LNode* node = pStack; node != nullptr; node = node->next)
    {
        std::cout << node->data;
        if(node->next) std::cout << ",";
    }
    std::cout << "\n";
}

void Stack_Destroy(LinkStack& pStack)
{
    while(pStack)
    {
        LNode* next = pStack->next;
        delete pStack;
        pStack = next;
    }
}

int main() {
    LinkStack pStack;
    Stack_Init(pStack);

    std::cout << "栈判空：" << (Stack_Empty(pStack) ? "空" : "非空") << '\n';

    // 示例：手动入栈
    Stack_Push(pStack, 1);
    Stack_Push(pStack, 2);
    Stack_Push(pStack, 3);
    Stack_Push(pStack, 4);
    Stack_Show(pStack);

    int i = -1;
    Stack_Pop(pStack, i);
    std::cout << "出栈：" << i << '\n';
    Stack_Show(pStack);

    Stack_Pop(pStack, i);
    std::cout << "出栈：" << i << '\n';
    Stack_Show(pStack);

    Stack_Pop(pStack, i);
    std::cout << "出栈：" << i << '\n';
    Stack_Show(pStack);

    std::cout << "栈长度：" << Stack_Length(pStack) << "\n\n";
    std::cout << "栈判空：" << (Stack_Empty(pStack) ? "空" : "非空") << '\n';

    // 如果需要从输入批量创建（999 结束），取消下面两行注释：
    // std::cout << "输入若干整数入栈，999 结束：\n";
    // Stack_Create(pStack);

    Stack_Destroy(pStack);
    return 0;
}
