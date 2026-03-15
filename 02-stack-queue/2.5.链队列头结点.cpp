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
    Q.front = new LNode{0, nullptr};
    if(!Q.front) return false;
    Q.rear = Q.front;
    Q.len = 0;
    return;
}

bool Queue_Empty(const LinkQueue& Q)
{
    return Q.front == Q.rear;
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
        Q.rear->next = node;
        Q.rear = node;
        ++Q.len;
    }
    return Q;
}

bool Queue_En(LinkQueue& Q, int e)
{
    LNode* node = new LNode{e, nullptr};
    Q.rear->next = node;
    Q.rear = node;
    ++Q.len;
    return true;
}

bool Queue_De(LinkQueue& Q, int& e)
{
    if(Queue_Empty(Q)) return false;
    LNode* p = Q.front->next;
    e = p->data;
    if(Q.rear == p) Q.rear = Q.front;
    delete p;
    --Q.len;
    return true;
}

void Queue_Show(const LinkQueue& Q)
{
    std::cout << "value: ";
    for (LNode* node = Q.front->next; node != nullptr; node = node->next)
    {
        std::cout << node->data;
        if(node->next) std::cout << ",";
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
    if (!Queue_Init(Q)) {
        std::cerr << "初始化失败\n";
        return 1;
    }

    std::cout << "队列判空：" << (Queue_Empty(Q) ? "空" : "非空") << '\n';

    // 批量创建（输入若干整数，999 结束），如：1 2 3 999
    // std::cout << "请输入若干整数，999 结束：\n";
    // Queue_Create(Q);

    // 演示：手动入队
    Queue_En(Q, 1);
    Queue_En(Q, 2);
    Queue_En(Q, 3);
    std::cout << "队列长度：" << Queue_Length(Q) << '\n';
    Queue_Show(Q);

    int x;
    if (Queue_De(Q, x)) std::cout << "出队列值：" << x << '\n';
    Queue_Show(Q);
    std::cout << "队列长度：" << Queue_Length(Q) << '\n';

    if (Queue_De(Q, x)) std::cout << "出队列值：" << x << '\n';
    Queue_Show(Q);
    std::cout << "队列长度：" << Queue_Length(Q) << '\n';

    std::cout << "队列判空：" << (Queue_Empty(Q) ? "空" : "非空") << '\n';

    Queue_Destroy(Q);
    return 0;
}