#include <iostream>

struct LNode
{
    int data;
    LNode * next;
};

using LinkStack = LNode*;

bool Stack_Init(LinkStack& pStack)
{
    pStack = new LNode;
    if(!pStack) return false;
    pStack->data = 0;
    pStack->next = nullptr;
    return true;
}

bool Stack_Empty(const LinkStack pStack)
{
    return pStack->next == nullptr;
}

int Stack_Length(const LinkStack pStack)
{
    return pStack->data;
}

LinkStack Stack_Create(LinkStack& pStack)
{
    int x;
    while(std::cin >> x && x != 999)
    {
        LNode* node = new LNode{x, pStack->next};
        pStack->next = node;
        pStack->data += 1;
    }
}

bool Stack_Push(LinkStack& pStack, int e)
{
    LNode* node = new LNode{e, pStack->next};
    pStack->next = node;
    pStack->data += 1;
    return true;
}

bool Stack_Pop(LinkStack& pStack, int& e)
{
    if(pStack->next == nullptr) return false;
    LNode* top = pStack->next;
    pStack->next = top->next;
    e = top->data;
    delete top;
    pStack->data -= 1;
    return true;
}

LNode* Stack_Get_Top(const LinkStack pStack)
{
    return pStack->next;
}

void Stack_Show(const LinkStack pStack)
{
    std::cout << "value: ";
    for (LNode* node = pStack->next; node != nullptr; node = node->next)
    {
        std::cout << node->data;
        if(node->next) std::cout << ",";
    }
    std::cout << "\n";
}

void Stack_Destroy(LinkStack& pStack)
{
    if(!pStack) return;
    LNode* cur = pStack;
    while(cur)
    {
        LNode* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    pStack = nullptr;
}

int main() {
    LinkStack s = nullptr;
    if (!Stack_Init(s)) {
        std::cerr << "初始化失败\n";
        return 1;
    }

    std::cout << "栈判空：" << (Stack_Empty(s) ? "空" : "非空") << '\n';

    std::cout << "输入若干整数入栈，999 结束：\n";
    Stack_Create(s);
    Stack_Show(s);
    std::cout << "栈长度：" << Stack_Length(s) << "\n\n";

    std::cout << "入栈：0\n";
    Stack_Push(s, 0);
    Stack_Show(s);
    std::cout << "栈长度：" << Stack_Length(s) << "\n\n";

    std::cout << "入栈：99\n";
    Stack_Push(s, 99);
    Stack_Show(s);
    std::cout << "栈长度：" << Stack_Length(s) << "\n\n";

    int x = -1;
    if (Stack_Pop(s, x)) {
        std::cout << "出栈：" << x << '\n';
    }
    Stack_Show(s);
    std::cout << "栈长度：" << Stack_Length(s) << "\n\n";

    if (Stack_Pop(s, x)) {
        std::cout << "出栈：" << x << '\n';
    }
    Stack_Show(s);
    std::cout << "栈长度：" << Stack_Length(s) << "\n\n";

    LNode* top = Stack_Get_Top(s);
    std::cout << "获取栈顶元素："
    << (top ? top->data : -1) << "\n\n";

    std::cout << "栈判空：" << (Stack_Empty(s) ? "空" : "非空") << '\n';

    Stack_Destroy(s);
    return 0;
}