#include <iostream>

struct BiTNode
{
    int data{};
    BiTNode* lchild{nullptr};
    BiTNode* rchild{nullptr};
};

using BiTree = BiTNode*;

constexpr int MaxSize = 50;

struct SqQueue
{
    BiTree data[MaxSize]{};
    int front{0};
    int rear{0};
};

void Queue_Init(SqQueue& Q) noexcept
{
    Q.front = Q.rear = 0;
}

bool Queue_Empty(const SqQueue& Q) noexcept
{
    return Q.front == Q.rear;
}

bool Queue_En(SqQueue& Q, BiTree e)
{
    if ((Q.rear + 1) % MaxSize == Q.front)
    {
        return false;
    }
    Q.data[Q.rear] = e;
    Q.rear = (Q.rear + 1) % MaxSize;
    return true;
}

bool Queue_De(SqQueue& Q, BiTree& e)
{
    if (Queue_Empty(Q)) return false;
    e = Q.data[Q.front];
    Q.front = (Q.front + 1) % MaxSize;
    return true;
}
//顺序存储递归构造
BiTree BiTree_Create(const int* nums, int len, int i)
{
    if (i >= len || nums[i] == 0 ) return nullptr;

    BiTree node = new BiTNode{};
    node->data = nums[i];
    node->lchild = BiTree_Create(nums, len, 2 * i + 1);
    node->rchild = BiTree_Create(nums, len, 2 * i + 2);
    return node;
}

void Order_Pre(BiTree tree)
{
    if (nullptr == tree) return;
    std::cout << tree->data << ',';
    Order_Pre(tree->lchild);
    Order_Pre(tree->rchild);
}

void Order_In(BiTree tree)
{
    if(nullptr == tree) return;
    Order_In(tree->lchild);
    std::cout << tree->data << ',';
    Order_In(tree->rchild);
}

void Order_Post(BiTree tree)
{
    if (nullptr == tree) return;
    Order_Post(tree->lchild);
    Order_Post(tree->rchild);
    std::cout << tree->data << ',';
}

void Order_Level (BiTree tree)
{
    if (nullptr == tree) return;

    SqQueue Q;
    Queue_Init(Q);
    Queue_En(Q, tree);

    BiTree p = nullptr;
    while(!Queue_Empty(Q))
    {
        Queue_De(Q, p);
        std::cout << p->data << ',';
        if(p->lchild != nullptr)
        {
            Queue_En(Q, p->lchild);
        }
        if(p->rchild != nullptr)
        {
            Queue_En(Q, p->rchild);
        }
    }
}

void DestroyTree(BiTree tree)
{
    if (nullptr == tree) return;
    DestroyTree(tree->lchild);
    DestroyTree(tree->rchild);
    delete tree;
}

int main()
{
    int nums[11] = {1, 2, 3, 0, 4, 0, 5, 0, 0, 6, 0};

    BiTree tree = BiTree_Create(nums, 11, 0);

    std::cout << "先序遍历：";
    Order_Pre(tree);
    std::cout << '\n';

    std::cout << "中序遍历：";
    Order_In(tree);
    std::cout << '\n';

    std::cout << "后序遍历：";
    Order_Post(tree);
    std::cout << '\n';

    std::cout << "层次遍历：";
    Order_Level(tree);
    std::cout << '\n';

    DestroyTree(tree); // 手动释放

    return 0;
}