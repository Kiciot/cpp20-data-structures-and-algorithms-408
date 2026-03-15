#include <iostream>

struct LNode
{
    int data;
    LNode* next;
};

using LinkList = LNode*;

bool List_Init(LinkList& pHead)
{
    pHead = new LNode{0, nullptr};
    return true;
}

bool List_Empty(LinkList pHead)
{
    return pHead->next == nullptr;
}

int List_Length(LinkList pHead)
{
    int cnt = 0;
    for (LNode* p = pHead->next;p;p = p->next) ++cnt;
    return cnt;
}

LinkList List_Create_Head_Insert(LinkList& pHead)
{
    int x;
    while (std::cin >> x && x != 999)
    {
        LNode* node = new LNode{x, pHead->next};
        pHead->next = node;
    }
    return pHead;
}

LinkList List_Create_Tail_Insert(LinkList& pHead)
{
    LNode* tail = pHead;
    while(tail->next) tail = tail->next;

    int x;
    while (std::cin >> x && x != 999)
    {
        LNode* node = new LNode{x, nullptr};
        tail->next = node;
        tail = node;
    }
    return pHead;
}

bool List_Insert(LinkList& pHead, int i, int e)
{
    int n = List_Length(pHead);
    if(i < 1 || i > n + 1) return false;

    LNode* p = pHead;
    for(int k = 1; k < i; ++k) p = p->next;

    LNode* node = new LNode{e, p->next};
    p->next = node;
    return true;
}

bool List_Del(LinkList& pHead, int i)
{
    int n = List_Length(pHead);
    if(i < 1 || i > n) return false;

    LNode* p = pHead;
    for(int k = 1; k < i; ++k) p = p->next;
    LNode* q = p->next;
    p->next = q->next;
    delete q;
    return true;
}

LNode* List_Get_Elem(LinkList pHead, int e, int& i)
{
    i = 1;
    for(LNode* p = pHead->next; p; p = p->next, ++i)
    {
        if (p->data == e) return p;
    }

    i = -1;
    return nullptr;
}

void List_Show(const LinkList& pHead)
{
    for(LNode* p = pHead->next;p;p = p->next)
    {
        std::cout <<  p->data << (p->next ? " -> " : "");
    }
    std::cout << std::endl;
}

void List_Destroy(LinkList& pHead)
{
    if(!pHead) return;
    LNode* p = pHead;
    while (p)
    {
        LNode* next = p->next;
        delete p;
        p = next;
    }
    pHead = nullptr;
}