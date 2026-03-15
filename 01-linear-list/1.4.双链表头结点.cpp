#include <iostream>

struct DNode
{
    int data;
    DNode* prior;
    DNode* next;
};

using DLinkList = DNode*;

bool DList_Init(DLinkList& pHead)
{
    pHead = new DNode;
    if(!pHead) return false;
    pHead->prior = nullptr;
    pHead->next = nullptr;
    return true;
}

bool DList_Empty(const DLinkList pHead)
{
    return pHead->next == nullptr;
}

int DList_Length(const DLinkList pHead)
{
    int count = 0;
    for(DNode* p = pHead->next; p; p = p->next) ++count;
    return count;
}

DLinkList DList_Create(DLinkList& pHead)
{
    int x;
    while (std::cin >>x && x != 999)
    {
        DNode* pTemp = new DNode{x, pHead, pHead->next};
        if (pHead->next) pHead->next->prior = pTemp;
        pHead->next = pTemp;
    }
    return pHead;
}

bool DList_Insert(DLinkList& pHead, int i, int e)
{
    if (i < 1 || i > DList_Length(pHead) + 1) return false;
    int j = 0;
    DNode* p = pHead;
    while (p && j < i - 1)
    {
        p = p->next;
        ++j;
    }
    if(!p) return false;

    DNode* pTemp = new DNode{e, p, p->next};
    if(p->next) p->next->prior = pTemp;
    p->next = pTemp;
    return true;
}

DNode* DList_Get_Elem(const DLinkList pHead, int e, int& i)
{
    i = 1;
    for(DNode* p = pHead->next; p; p = p->next, ++ i)
    {
        if(p->data == e) return p;
    }
    i = -1;
    return nullptr;
}

bool DList_Del(DLinkList& pHead,int i)
{
    if(i < 1 || i > DList_Length(pHead)) return false;

    DNode* p = pHead;
    for(int j = 0; j < i - 1; ++j) p = p->next;
    DNode* q = p->next;
    if(!q) return false;

    p->next = q->next;
    if(q->next) q->next->prior = p;
    delete q;
    return true;
}

void DList_Show(const DLinkList pHead)
{
    std::cout << "链表值:" ;
    for(DNode* node = pHead->next; node; node = node->next)
    {
        std::cout << node->data;
        if (node->next) std::cout << ",";
    }
    std::cout << std::endl;
}

void DList_Show_Back(const DLinkList pHead)
{
    DNode* node = pHead;
    while(node->next) node = node->next;

    if(node == pHead)
    {
        std::cout << "空表" << std::endl;
    }

    std::cout << "链表值:";
    while (node != pHead)
    {
        std::cout << node->data;
        if(node->prior != pHead) std::cout << ",";
        node = node->prior;
    }

    std::cout << std::endl;
}

void DList_Destroy(DLinkList& pHead)
{
    DNode* p = pHead;
    while(p)
    {
        DNode* next = p->next;
        delete p;
        p = next;
    }
    pHead = nullptr;
}

int main() {
    DLinkList pHead = nullptr;
    if (!DList_Init(pHead)) {
        std::cerr << "初始化失败\n";
        return 1;
    }

    std::cout << "链表判空：" << (DList_Empty(pHead) ? "空" : "非空") << std::endl;

    std::cout << "头插法创建（输入若干整数，输入 999 结束）：\n";
    DList_Create(pHead);

    DList_Show(pHead);
    DList_Show_Back(pHead);
    std::cout << "链表长度：" << DList_Length(pHead) << std::endl << std::endl;

    std::cout << "在位序 1 插入 0\n";
    DList_Insert(pHead, 1, 0);
    DList_Show(pHead);

    std::cout << "在尾部插入 99\n";
    DList_Insert(pHead, DList_Length(pHead) + 1, 99);
    DList_Show(pHead);

    std::cout << "删除位序 1\n";
    DList_Del(pHead, 1);
    DList_Show(pHead);
    DList_Show_Back(pHead);

    int pos = -1;
    DNode* p5 = DList_Get_Elem(pHead, 5, pos);
    std::cout << "值 5 的位序：" << (p5 ? pos : -1) << std::endl;

    DList_Destroy(pHead);
    return 0;
}