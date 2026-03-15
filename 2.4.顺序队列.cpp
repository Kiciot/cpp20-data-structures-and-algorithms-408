#include <iostream>

constexpr int MAX_SIZE = 100;
using ElemType = int;

struct SqQueue
{
    ElemType data[MAX_SIZE];
    int front;
    int rear;
    int size;
};

void Queue_Init(SqQueue& Q)
{
    Q.front = Q.rear = Q.size = 0;
}

bool Queue_IsEmpty(const SqQueue& Q) { return Q.size == 0;}
bool Queue_IsFull(const SqQueue& Q) { return Q.size == MAX_SIZE;}

bool Queue_En(SqQueue& Q, ElemType x)
{
    if(Queue_IsFull(Q))
    {
        std::cout << "queue is full\n";
        return false; 
    }
    Q.data[Q.rear] = x;
    Q.rear = (Q.rear + 1) % MAX_SIZE;
    ++Q.size;
    return true;
}

bool Queue_De(SqQueue& Q, ElemType& x)
{
    if(Queue_IsEmpty(Q))
    {
        std::cout << "queue is empty";
        return false;
    }
    x = Q.data[Q.front];
    Q.front = (Q.front + 1) % MAX_SIZE;
    --Q.size;
    return true;
}

void Queue_Print(const SqQueue& Q)
{
    for(int i = 0, idx = Q.front; i < Q.size; ++i, idx = (idx + 1) % MAX_SIZE)
    {
        std::cout << Q.data[idx] << (i + 1 == Q.size ? '\n' : ' ');
    }
    if(Q.size == 0) std::cout << '\n';
}

int main() {
    SqQueue Q;
    Queue_Init(Q);

    Queue_En(Q, 1);
    Queue_En(Q, 2);
    Queue_En(Q, 3);

    std::cout << "队列元素: ";
    Queue_Print(Q);

    ElemType x;
    if (Queue_De(Q, x)) std::cout << "出队元素：" << x << '\n';

    std::cout << "队列元素: ";
    Queue_Print(Q);

    // 再入队，验证循环可复用空间
    Queue_En(Q, 4);
    Queue_En(Q, 5);
    std::cout << "再次入队后: ";
    Queue_Print(Q);

    return 0;
}