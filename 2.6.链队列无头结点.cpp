#include <iostream>

struct LNode
{
    int data;
    LNode* next;
};

struct LinkQueue
{
    LNode* front;
    LNode* rear;
    int len;
};

bool Queue_Init(LinkQueue& Q)
{
    Q.front = Q.rear = nullptr;
    Q.len = 0;
    return true;
}

bool Queue_Empty(const LinkQueue& Q)
{
    return Q.front == nullptr;
}

int Queue_Length(const LinkQueue& Q)
{
    return Q.len;
}

LinkQueue& Queue_Create(LinkQueue& Q)
{
    int x;
    while(std::cin >> x && x != 999)
    {
        LNode* node = new LNode{x, nullptr};
        if (!Q.front)
        {
            Q.front = Q.rear = node;
        }
        else
        {
            Q.rear->next = node;
            Q.rear = node;
        }
        ++Q.len;
    }
    return Q;
}

bool Queue_En(LinkQueue& Q, int e) {
    LNode* node = new LNode{e, nullptr};
    if(!Q.front)
    {
        Q.front = Q.rear = node;
    }
    else
    {
        Q.rear->next = node;
        Q.rear = node;
    }
    ++Q.len;
    return true;
}

bool Queue_De(LinkQueue& Q, int& e)
{
    if(!Q.front) return false;
    LNode* p = Q.front;
    e = p->data;
    Q.front = p->next;
    if(!Q.front) Q.rear = nullptr;
    delete p;
    --Q.len;
    return true;
}

void Queue_Show(const LinkQueue& Q)
{
    std::cout << "value: ";
    for(LNode* cur = Q.front; cur != nullptr; cur = cur->next)
    {
        std::cout << cur->data;
        if(cur->next) std::cout << ",";
    }
    std::cout << '\n';
}

void Queue_Destroy(LinkQueue& Q)
{
    LNode* p = Q.front;
    while(p)
    {
        LNode* nxt = p->next;
        delete p;
        p = nxt;
    }
    Q.front = Q.rear = nullptr;
    Q.len = 0;
}

int main() {
    LinkQueue Q;
    Queue_Init(Q);
    std::cout << "队列判空：" << (Queue_Empty(Q) ? "空" : "非空") << '\n';

    // 演示：手动入队
    Queue_En(Q, 10);
    Queue_En(Q, 20);
    Queue_En(Q, 30);
    Queue_En(Q, 40);
    Queue_En(Q, 50);
    Queue_Show(Q);
    std::cout << "队列长度：" << Queue_Length(Q) << "\n\n";

    int x = -1;
    if (Queue_De(Q, x)) std::cout << "出队列值：" << x << '\n';
    Queue_Show(Q);
    std::cout << "队列长度：" << Queue_Length(Q) << "\n\n";

    if (Queue_De(Q, x)) std::cout << "出队列值：" << x << '\n';
    Queue_Show(Q);
    std::cout << "队列长度：" << Queue_Length(Q) << "\n\n";

    std::cout << "队列判空：" << (Queue_Empty(Q) ? "空" : "非空") << '\n';

    // 如需从输入批量创建（999 结束）：
    // std::cout << "输入若干整数，999 结束：\n";
    // Queue_Create(Q);
    // Queue_Show(Q);

    Queue_Destroy(Q);
    return 0;
}