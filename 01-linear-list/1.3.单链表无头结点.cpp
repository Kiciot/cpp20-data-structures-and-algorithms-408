#include <iostream>

struct LNode {
    int data;
    LNode* next;
};
using LinkList = LNode*;

bool List_Init(LinkList& pHead)
{
    pHead = nullptr;
    return true;
}

bool List_Empty(const LinkList pHead)
{
    return pHead == nullptr;
}

int List_Length(const LinkList pHead)
{
    int count = 0;
    for(LNode* p = pHead; p ; p = p->next) ++count;

    return count;
}

LinkList List_Create(LinkList& pHead)
{
    int x;
    if(!(std::cin) >> x) return pHead;

    while(x != 999)
    {
        LNode* node = new LNode{x, pHead};
        pHead = node;
        if(!(std::cin >> x)) break;
    }
    return pHead;
}

bool List_Insert(LinkList& pHead, int i, int e)
{
    const int n = List_Length(pHead);
    if(i < 1 || i > n + 1) return false;

    LNode* node = new LNode{e, nullptr};
    if(i == 1)
    {
        node->next = pHead;
        pHead = node;
        return true;
    }

    LNode* p = pHead;
    for(int j = 1; j < i - 1; ++j) p = p->next;
    node->next = p->next;
    p->next = node;
    return true;
}

LNode* List_Get_Elem(const LinkList pHead, int e, int& i)
{
    i = 1;
    for(LNode* p = pHead; p; p = p->next, ++i)
    {
        if(p->data == e) return p;
    }

    i = -1;
    return nullptr;
}

bool List_Del(LinkList& pHead, int i)
{
    const int n = List_Length(pHead);
    if (i < 1 || i > n) return false;

    if (i == 1)
    {
        LNode* q = pHead;
        pHead = pHead->next;
        delete q;
        return true;
    }

    LNode* p = pHead;
    for(int j = 1; j < i - 1; ++j) p = p->next;
    LNode* q = p->next;
    p->next = q->next;
    delete q;
    return true;
}

void List_Show(const LinkList pHead)
{
    std::cout << "链表值:";
    LNode* p = pHead;
    while(p != nullptr)
    {
        std::cout << p->data;
        p = p->next;
        if(p != nullptr) std::cout << "->";
    }
    std::cout << "\n";
}

void List_Destroy(LinkList& pHead)
{
    LNode* p = pHead;
    while(p)
    {
        LNode* next = p->next;
        delete p;
        p = next;
    }
    pHead = nullptr;
}

int main() {
    LinkList pHead;
    List_Init(pHead);
    std::cout << "链表判空：" << (List_Empty(pHead) ? "空" : "非空") << '\n';

    // 示例输入：1 2 3 4 999
    List_Create(pHead);
    List_Show(pHead);
    std::cout << "链表长度：" << List_Length(pHead) << '\n';

    std::cout << "位序为 1（头插），插入：0\n";
    List_Insert(pHead, 1, 0);
    List_Show(pHead);
    std::cout << "链表长度：" << List_Length(pHead) << '\n';

    const int tailPos = List_Length(pHead) + 1;
    std::cout << "位序为 " << tailPos << "（尾插），插入：99\n";
    List_Insert(pHead, tailPos, 99);
    List_Show(pHead);
    std::cout << "链表长度：" << List_Length(pHead) << '\n';

    std::cout << "删除位序为 1（头删）\n";
    List_Del(pHead, 1);
    List_Show(pHead);
    std::cout << "链表长度：" << List_Length(pHead) << '\n';

    const int tailPos2 = List_Length(pHead);
    std::cout << "删除位序为 " << tailPos2 << "（尾删）\n";
    List_Del(pHead, tailPos2);
    List_Show(pHead);
    std::cout << "链表长度：" << List_Length(pHead) << '\n';

    std::cout << "获取值为 5 的结点位序\n";
    int pos = -1;
    LNode* p5 = List_Get_Elem(pHead, 5, pos);
    std::cout << "值为 5 的结点位序为：" << (p5 ? pos : -1) << '\n';

    std::cout << "链表判空：" << (List_Empty(pHead) ? "空" : "非空") << '\n';

    List_Destroy(pHead);
    return 0;
}