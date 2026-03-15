// 智能指针
#include <iostream>
#include <memory>
#include <vector>

constexpr int MAX_SIZE = 50;

struct BiTNode
{
    int data;
    std::unique_ptr<BiTNode> lchild, rchild;
};


struct SqQueue
{
    std::vector<std::unique_ptr<BiTNode>> data;
    int front = 0, rear = 0;

    bool isEmpty() const
    {
        return front == rear;
    }

    bool isFull() const
    {
        return (rear + 1) % MAX_SIZE == front;
    }

    bool enqueue(std::unique_ptr<BiTNode> node)
    {
        if(isFull())
        {
            std::cout << "queue is full" << std::endl;
            return false;
        }

        data.push_back(std::move(node));
        rear = (rear + 1) % MAX_SIZE;
        return true;
    }

    bool dequeue(std::unique_ptr<BiTNode>& node)
    {
        if(isEmpty())
        {
            std::cout << "queue is empty" << std::endl;
            return false;
        }

        node = std::move(data[front]);
        front + (front + 1) % MAX_SIZE;
        return true;
    }
};

std::unique_ptr<BiTNode> createTree(const std::vector<int>& nums, int index)
{
    if(index >= nums.size() || nums[index] == 0) return nullptr;

    auto node =std::make_unique<BiTNode>();
    node->data = nums[index];
    node->lchild = createTree(nums, 2 * index + 1);
    node->rchild = createTree(nums, 2 * index + 2);
    return node;
}

void preOrder(const std::unique_ptr<BiTNode>& tree)
{
    if(tree == nullptr) return;
    std::cout << tree->data << ", ";
    preOrder(tree->lchild);
    preOrder(tree->rchild);
}

template <typename T>
void inOrder(const T& tree)
{
    if (tree == nullptr) return;
    inOrder(tree->lchild);
    std::cout << tree->data << ", ";
    inOrder(tree->rchild);
}

void postOrder(const std::unique_ptr<BiTNode>& tree)
{
    if(tree == nullptr) return;
    postOrder(tree->lchild);
    postOrder(tree->rchild);
    std::cout << tree->data << ", ";
}

void levelOrder(const std::unique_ptr<BiTNode>& tree)
{
    if(!tree) return;

    SqQueue queue;
    queue.enqueue(std::make_unique<BiTNode>(*tree));

    while(!queue.isEmpty())
    {
        std::unique_ptr<BiTNode> current;
        queue.dequeue(current);
        std::cout << current->data << ", ";

        if(current->lchild) queue.enqueue(std::make_unique<BiTNode>(*current->lchild));
        if(current->rchild) queue.enqueue(std::make_unique<BiTNode>(*current->rchild));
    }
}

int main() {
    std::vector<int> nums = {1, 2, 3, 0, 4, 0, 5, 0, 0, 6, 0};  // 二叉树数据（0表示空节点）

    // 创建二叉树
    auto tree = createTree(nums, 0);

    // 先序遍历
    std::cout << "先序遍历：";
    preOrder(tree);
    std::cout << std::endl;

    // 中序遍历
    std::cout << "中序遍历：";
    inOrder(tree);
    std::cout << std::endl;

    // 后序遍历
    std::cout << "后序遍历：";
    postOrder(tree);
    std::cout << std::endl;

    // 层次遍历
    std::cout << "层次遍历：";
    levelOrder(tree);
    std::cout << std::endl;

    return 0;
}