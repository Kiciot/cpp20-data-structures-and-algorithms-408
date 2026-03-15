#include <iostream>

struct BSTNode
{
    int data{};
    BSTNode* lchild{nullptr};
    BSTNode* rchild{nullptr};
};

using BSTree = BSTNode*;

constexpr int MaxSize = 50;

//------------队列实现, 便于层序遍历------------
struct SqQueue
{
    BSTree data[MaxSize]{};
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

bool Queue_En(SqQueue& Q, BSTree e) noexcept
{
    if ((Q.rear + 1) % MaxSize == Q.front)
    {
        return false;
    }
    Q.data[Q.rear] = e;
    Q.rear = (Q.rear + 1) % MaxSize;
    return true;
}

bool Queue_De(SqQueue& Q, BSTree& e) noexcept
{
    if (Queue_Empty(Q)) return false;
    e = Q.data[Q.front];
    Q.front = (Q.front + 1) % MaxSize;
    return true;
}
//------------------------------------


int BST_Insert(BSTree& tree, int s)
{
    if (nullptr == tree) {
        tree = new BSTNode{};
        tree->data = s;
        return 1;
    }
    else if (s == tree->data) {
        return 0;//不能插入相同的关键字
    }
    else if (s < tree->data) {
        return BST_Insert(tree->lchild, s);
    }
    else {
        return BST_Insert(tree->rchild, s);
    }
}

void BST_Create(BSTree& tree, const int* nums, int len)
{
    tree = nullptr;
    for (int i = 0; i < len; ++i){
        BST_Insert(tree, nums[i]);
    }
}

void BST_Delete(BSTree& tree, int e)
{
    BSTree p = tree;

    BSTree f = nullptr;

    while(p != nullptr)
    {
        if(p->data == e) {
            break;
        }
        f = p;
        if(e < p->data) {
            p = p->lchild;
        }
        else {
            p = p->rchild;
        }
    }

    if(p == nullptr) return;

    if (p->lchild == nullptr && p->rchild == nullptr) {
        if(f == nullptr) {
            tree = nullptr;
        }
        else if(p == f->lchild) {
            f->lchild = nullptr;
        }
        else {
            f->rchild = nullptr;
        }
        delete p;
    }
    
    else if (p->lchild == nullptr)
    {
        if (f == nullptr) {
            tree = p->rchild;
        }
        else if (p == f->lchild) {
            f->lchild = p->rchild;
        }
        else {
            f->rchild = p->rchild;
        }
        delete p;
    }

    else if (p->rchild == nullptr)
    {
        if (f == nullptr) {
            tree = p->lchild;
        }
        else if (p == f->lchild) {
            f->lchild = p->lchild;
        }
        else {
            f->rchild = p->lchild;
        }
        delete p;
    }

    else
    {
        BSTree s = p->lchild;
        BSTree sf = p;
        while(s->rchild != nullptr) {
            sf = s;
            s = s->rchild;
        }
        p->data = s->data;

        if (s == sf->lchild) {
            sf->lchild = s->lchild;
        }
        else {
            sf->rchild = s->lchild;
        }
        delete s;
    }
}

void Order_Pre(BSTree tree)
{
    if (tree == nullptr) return;
    std::cout << tree->data << ',';
    Order_Pre(tree->lchild);
    Order_Pre(tree->rchild);
}

// 中序遍历：左 根 右
void Order_In(BSTree tree)
{
    if (tree == nullptr) return;
    Order_In(tree->lchild);
    std::cout << tree->data << ',';
    Order_In(tree->rchild);
}

// 后序遍历：左 右 根
void Order_Post(BSTree tree)
{
    if (tree == nullptr) return;
    Order_Post(tree->lchild);
    Order_Post(tree->rchild);
    std::cout << tree->data << ',';
}

void Order_Level(BSTree tree)
{
    if (tree == nullptr) return;

    SqQueue Q;
    Queue_Init(Q);
    Queue_En(Q, tree);

    BSTree p = nullptr;
    while (!Queue_Empty(Q))
    {
        Queue_De(Q, p);
        std::cout << p->data << ',';
        if (p->lchild != nullptr)
        {
            Queue_En(Q, p->lchild);
        }
        if (p->rchild != nullptr)
        {
            Queue_En(Q, p->rchild);
        }
    }
}

void DestroyTree(BSTree tree)
{
    if (tree == nullptr) return;
    DestroyTree(tree->lchild);
    DestroyTree(tree->rchild);
    delete tree;
}

int main()
{
    // 用 int 更合理，不用 char
    int nums[] = {50, 30, 60, 40, 70, 10, 20, 80};
    int len = static_cast<int>(sizeof(nums) / sizeof(nums[0]));

    BSTree tree = nullptr;
    BST_Create(tree, nums, len);

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

    // 试试删除几个元素
    std::cout << "删除 60 后中序遍历：";
    BST_Delete(tree, 60);
    Order_In(tree);
    std::cout << '\n';

    std::cout << "删除 50（原根）后中序遍历：";
    BST_Delete(tree, 50);
    Order_In(tree);
    std::cout << '\n';

    DestroyTree(tree);
    return 0;
}