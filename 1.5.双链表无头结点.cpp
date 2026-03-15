#include<iostream>

struct DNode
{
    int data;
    DNode* prior;
    DNode* next;
};

using DLinkList = DNode*;

bool DList_Init(DLinkList& pHead)
{
    pHead = nullptr;
    return true;
}

bool DList_Empty(const DLinkList pHead)
{
    return pHead == nullptr;
}

int DList_Length(const DLinkList pHead)
{
    int count = 0;
    for(DNode* p = pHead; p; p = p->next) ++count;
    
    return count;
}


DLinkList DList_Create(DLinkList& pHead)
{
    pHead == nullptr;
    int x;
    while(std::cin >> x && x != 999)
    {
        DNode* node = new DNode{x, nullptr, pHead};
        if(pHead) pHead->prior = node;
        pHead = node;
    }
    return pHead;
}

bool DList_Insert(DLinkList& pHead, int i, int e)
{
    const int n = DList_Length(pHead);
    if(i < 1 || i > n + 1) return false;

    DNode* node = new DNode{ e, nullptr, nullptr };

    if(i == 1)
    {
        node->next = pHead;
        if(pHead) pHead->next = node;
        pHead = node;
        return true;
    }

    DNode* p = pHead;
    for(int k = 1; k < i - 1; ++k) p = p->next;

    node->next = p->next;
    node->prior = p;
    if(p->next) p->next->prior = node;
    p->next = node;
    return true;
}

DNode* DList_Get_Elem(const DLinkList pHead, int e, int& i)
{
    i = 1;
    for(DNode* p = pHead; p; p = p->next, ++i)
    {
        if(p->data == e) return p;
    }

    i = -1;
    return nullptr;
}

bool DList_Del(DLinkList& pHead, int i)
{
    const int n = DList_Length(pHead);
    if(i < 1 || i > n) return false;

    if(i == 1)
    {
        DNode* q = pHead;
        pHead = pHead->next;
        if(pHead) pHead->prior = nullptr;
        delete q;
        return true;
    }

    DNode* p = pHead;
    for(int k = 1; k < i - 1; ++k) p = p->next;

    DNode* q = p->next;
    p->next = q->next;
    if(q->next) q->next->prior = p;
    delete q;
    return true;
}

void DList_Show(const DLinkList pHead)
{
    std::cout << "链表值";

    for(DNode* node = pHead; node; node = node->next)
    {
        std::cout << node->data;
        if(node->next) std::cout << ",";
    }
    std::cout << "\n";
}



void DList_Show_Back(const DLinkList pHead)
{
    std::cout << "链表值";
    if(!pHead){std::cout << "\n"; return;}

    DNode* tail = pHead;
    while(tail->next) tail = tail->next;

    for(DNode* node = tail; node; node = node->prior)
    {
        std::cout << node->data;
        if(node->prior) std::cout << ",";
    }
    std::cout << "\n";
}

void DList_Destroy(DLinkList& pHead) 
{
    DNode* p  = pHead;
    while(p)
    {
        DNode* next = p->next;
        delete p;
        p = next;
    }
    pHead = nullptr;
}

int main() {
    DLinkList pHead;
    DList_Init(pHead);

    std::cout << "链表判空：" << (DList_Empty(pHead) ? "空" : "非空") << '\n';

    std::cout << "输入若干整数，999 结束：\n";
    DList_Create(pHead);
    DList_Show(pHead);
    DList_Show_Back(pHead);
    std::cout << "链表长度：" << DList_Length(pHead) << "\n\n";

    std::cout << "位序为 1(头插)，插入 0\n";
    DList_Insert(pHead, 1, 0);
    DList_Show(pHead);
    std::cout << "链表长度：" << DList_Length(pHead) << "\n\n";

    std::cout << "位序为 " << (DList_Length(pHead) + 1) << " (尾插)，插入 99\n";
    DList_Insert(pHead, DList_Length(pHead) + 1, 99);
    DList_Show(pHead);
    std::cout << "链表长度：" << DList_Length(pHead) << "\n\n";

    std::cout << "删除位序为 1(头删)\n";
    DList_Del(pHead, 1);
    DList_Show(pHead);
    DList_Show_Back(pHead);
    std::cout << "链表长度：" << DList_Length(pHead) << "\n\n";

    std::cout << "删除位序为 " << DList_Length(pHead) << " (尾删)\n";
    DList_Del(pHead, DList_Length(pHead));
    DList_Show(pHead);
    DList_Show_Back(pHead);
    std::cout << "链表长度：" << DList_Length(pHead) << "\n\n";

    int pos = -1;
    DNode* p5 = DList_Get_Elem(pHead, 5, pos);
    std::cout << "值为 5 的结点位序：" << (p5 ? pos : -1) << "\n";

    DList_Destroy(pHead);
    return 0;
}
