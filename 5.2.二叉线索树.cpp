#include <iostream>

using ElemType = int;

//线索二叉树节点
//ltag, rtag = 0, 表示指向孩子
//ltag, rtag = 1, 表示指向前驱后继

struct ThreadNode
{
    ElemType data{};
    ThreadNode* lchild{nullptr};
    ThreadNode* rchild{nullptr};
    int ltag{0};
    int rtag{0};
};

using ThreadTree = ThreadNode*;

ThreadNode* pre = nullptr;

ThreadTree Thread_Create(const int* nums, int len, int i)
{
    if(i >= len || nums[i] == 0)
    {
        return nullptr;
    }

    ThreadTree node = new ThreadNode{};
    node->data = nums[i];

    node->lchild = Thread_Create(nums, len, 2 * i + 1);
    node->rchild = Thread_Create(nums, len, 2 * i + 2);

    return node;
}

void ThreadVisit(ThreadTree T)
{
    if (T->lchild == nullptr)
    {
        T->lchild = pre;
        T->ltag = 1;
    }
    if(pre != nullptr && pre->rchild == nullptr)
    {
        pre->rchild = T;
        pre->rtag = 1;
    }

    pre = T;
}

void PreThread(ThreadTree T)
{
    if (T != nullptr)
    {
        ThreadVisit(T);

        if(T->ltag == 0)
        {
            PreThread(T->lchild);
        }
        if(T->rtag == 0)
        {
            PreThread(T->rchild);
        }
    }
}

void CreatePreThread(ThreadTree T)
{
    pre = nullptr;
    if (T != nullptr)
    {
        PreThread(T);
        if(pre != nullptr && pre->rchild == nullptr)
        {
            pre->rtag = 1;
        }
    }
}

void InThread(ThreadTree T)
{
    if(T != nullptr)
    {
        InThread(T->lchild);
        ThreadVisit(T);
        InThread(T->rchild);
    }
}

void CreateInThread(ThreadTree T)
{
    pre = nullptr;
    if(T != nullptr)
    {
        InThread(T);
        if (pre != nullptr && pre->rchild == nullptr)
        {
            pre->rtag = 1;
        }
    }
}

void PostThread(ThreadTree T)
{
    if (T != nullptr)
    {
        PostThread(T->lchild);
        PostThread(T->rchild);

        ThreadVisit(T);
    }
}

void CreatePostThread(ThreadTree T)
{
    pre = nullptr;
    if(T != nullptr)
    {
        PostThread(T);
        if(pre != nullptr && pre->rchild ==nullptr)
        {
            pre->rtag = 1;
        }
    }
}

void PreThreadTraverse(ThreadTree T)
{
    if(T != nullptr)
    {
        std::cout << T->data << ' ';
        if(T->ltag == 0)
        {
            PreThreadTraverse(T->lchild);
        }
        if (T->rtag == 0)
        {
            PreThreadTraverse(T->rchild);
        }
    }
}

void InThreadTraverse(ThreadTree T)
{
    if(T != nullptr)
    {
        if (T->ltag == 0)
        {
            InThreadTraverse(T->lchild);
        }
        std::cout << T->data << ' ';
        if(T->rtag == 0)
        {
            InThreadTraverse(T->rchild);
        }
    }
}

void PostThreadTraverse(ThreadTree T)
{
    if(T != nullptr)
    {
        if(T->ltag == 0)
        {
            PostThreadTraverse(T->lchild);
        }
        if(T->rtag == 0)
        {
            PostThreadTraverse(T->rchild);
        }
        std::cout << T->data << ' ';
    }
}

void DestroyThreadTree(ThreadTree T)
{
    if(T == nullptr) return;
    {
        if (T->ltag == 0)
        {
            DestroyThreadTree(T->lchild);
        }
        if (T->rtag == 0)
        {
            DestroyThreadTree(T->rchild);
        }
        delete T;
    }
}

int main()
{
    // 用 int 更自然：0 仍然表示“空结点”
    int nums[11] = {1, 2, 3, 0, 4, 0, 5, 0, 0, 6, 0};

    ThreadTree preTree  = Thread_Create(nums, 11, 0);
    ThreadTree inTree   = Thread_Create(nums, 11, 0);
    ThreadTree postTree = Thread_Create(nums, 11, 0);

    // 前序线索化并遍历
    CreatePreThread(preTree);
    std::cout << "前序遍历结果：";
    PreThreadTraverse(preTree);
    std::cout << '\n';

    // 中序线索化并遍历
    CreateInThread(inTree);
    std::cout << "中序遍历结果：";
    InThreadTraverse(inTree);
    std::cout << '\n';

    // 后序线索化并遍历
    CreatePostThread(postTree);
    std::cout << "后序遍历结果：";
    PostThreadTraverse(postTree);
    std::cout << '\n';

    // 释放内存（根据线索标记，只沿“真孩子指针”往下走）
    DestroyThreadTree(preTree);
    DestroyThreadTree(inTree);
    DestroyThreadTree(postTree);

    return 0;
}
