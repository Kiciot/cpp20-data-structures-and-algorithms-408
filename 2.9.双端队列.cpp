#include <iostream>

#define MAX_SIZE 100

struct Deque
{
    int data[MAX_SIZE];
    int front;
    int rear;
};

void Deque_Init(Deque& Q)
{
    Q.front = 0;
    Q.rear = 0;
}

bool Deque_Empty(const Deque& Q)
{
    return Q.front == Q.rear;
}

bool Deque_Full(const Deque& Q)
{
    return (Q.front == (Q.rear + 1) % MAX_SIZE);
}

int Deque_Size(const Deque& Q)
{
    return (Q.rear - Q.front + MAX_SIZE) % MAX_SIZE;
}

bool Deque_InsertFront(Deque& Q, int x)
{
    if(Deque_Full(Q))
    {
        std:: cout << "full";
        return false;
    }
    Q.front = (Q.front - 1 + MAX_SIZE) % MAX_SIZE;
    Q.data[Q.front] = x;
    return true;
}

bool Deque_InsertRear(Deque& Q, int x)
{
    if(Deque_Full(Q))
    {
        std::cout << "full";
        return false;
    }
    Q.rear = (Q.rear + 1 + MAX_SIZE) % MAX_SIZE;
    Q.data[Q.rear] = x;
    return true;
}

bool Deque_DeleteFront(Deque& Q, int& out)
{
    if(Deque_Empty(Q))
    {
        std::cout << "empty\n";
        return false;
    }

    out = Q.data[Q.front];
    Q.front = (Q.front + 1) % MAX_SIZE;
    return true;
}

bool Deque_DeleteRear(Deque& Q, int& out)
{
    if(Deque_Empty(Q))
    {
        std::cout << "empty\n";
        return false;
    }

    Q.rear = (Q.rear - 1 + MAX_SIZE) % MAX_SIZE;
    out = Q.data[Q.rear];
    return true;
}

void Deque_Print(const Deque& Q)
{
    std::cout << "value: ";
    for (int i = Q.front; i != Q.rear; i = (i + 1) % MAX_SIZE)
    {
        std::cout << Q.data[i] << (((i + 1) % MAX_SIZE == Q.rear) ? '\n' : ' ' );
    }
    if(Deque_Empty(Q)) std::cout << '\n';
}

int main() {
    Deque Q;
    Deque_Init(Q);

    // 在队头插入
    Deque_InsertFront(Q, 1);
    Deque_InsertFront(Q, 2);

    // 在队尾插入
    Deque_InsertRear(Q, 3);
    Deque_InsertRear(Q, 4);

    Deque_Print(Q);
    std::cout << "size = " << Deque_Size(Q) << '\n';

    int x;
    if (Deque_DeleteFront(Q, x)) std::cout << "从队头删除元素: " << x << '\n';
    Deque_Print(Q);

    if (Deque_DeleteRear(Q, x)) std::cout << "从队尾删除元素: " << x << '\n';
    Deque_Print(Q);

    return 0;
}