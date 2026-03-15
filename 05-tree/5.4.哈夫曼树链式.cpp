#include <iostream>
#include <string>
#include <algorithm> // std::reverse 可选

struct HuffmanNode
{
    int weight{};
    char ch{};
    HuffmanNode* left{nullptr};
    HuffmanNode* right{nullptr};
};

using HuffmanTree = HuffmanNode*;

// 在 forest[0..forestSize) 中找到权值最小的两棵树，返回它们的下标
void SelectTwoMin(HuffmanTree* forest, int forestSize, int& idx1, int& idx2)
{
    idx1 = -1;
    idx2 = -1;

    // 找最小
    for (int i = 0; i < forestSize; ++i)
    {
        if (forest[i] == nullptr) continue;
        if (idx1 == -1 || forest[i]->weight < forest[idx1]->weight)
        {
            idx1 = i;
        }
    }

    // 找第二小
    for (int i = 0; i < forestSize; ++i)
    {
        if (forest[i] == nullptr || i == idx1) continue;
        if (idx2 == -1 || forest[i]->weight < forest[idx2]->weight)
        {
            idx2 = i;
        }
    }

    // 为方便后面删元素，保证 idx1 < idx2（不是必须，但处理简单）
    if (idx1 > idx2)
    {
        int tmp = idx1;
        idx1 = idx2;
        idx2 = tmp;
    }
}

// 根据权值数组 w 和字符数组 data 构建链式哈夫曼树
HuffmanTree BuildHuffmanTree(const int* w, const char* data, int n)
{
    if (n <= 0) return nullptr;

    int maxNodes = 2 * n - 1;
    // 保存“当前森林中每棵树的根结点”
    HuffmanTree* forest = new HuffmanTree[maxNodes]{};

    int forestSize = n;

    // 初始化：每个字符一个叶子结点
    for (int i = 0; i < n; ++i)
    {
        HuffmanNode* node = new HuffmanNode{};
        node->weight = w[i];
        node->ch = data[i];
        node->left = nullptr;
        node->right = nullptr;
        forest[i] = node;
    }

    // 重复合并，直到只剩下一棵树
    for (int k = n; k < maxNodes; ++k)
    {
        int idx1 = -1;
        int idx2 = -1;
        SelectTwoMin(forest, forestSize, idx1, idx2);

        HuffmanNode* parent = new HuffmanNode{};
        parent->weight = forest[idx1]->weight + forest[idx2]->weight;
        parent->ch = '\0'; // 内部结点不存实际字符
        parent->left = forest[idx1];
        parent->right = forest[idx2];

        // 用 parent 取代 idx1 的树，删掉 idx2 的树
        forest[idx1] = parent;
        forest[idx2] = forest[forestSize - 1]; // 用最后一个覆盖
        forest[forestSize - 1] = nullptr;
        --forestSize;
    }

    HuffmanTree root = forest[0];
    delete[] forest;
    return root;
}

// 递归生成哈夫曼编码：
// 当前路径保存在 code 中，到叶子时写入 symbols/codes 数组
void GenerateCodes(
    HuffmanTree root,
    std::string& code,
    char* symbols,
    std::string* codes,
    int n
)
{
    if (root == nullptr) return;

    // 叶子结点：记录编码
    if (root->left == nullptr && root->right == nullptr)
    {
        // 特殊情况：只有一个字符，通常规定编码为 "0"
        if (code.empty())
        {
            code = "0";
        }

        // 找到这个字符在 symbols 里的位置
        for (int i = 0; i < n; ++i)
        {
            if (symbols[i] == root->ch)
            {
                codes[i] = code;
                break;
            }
        }
        return;
    }

    // 向左走，路径加 '0'
    if (root->left != nullptr)
    {
        code.push_back('0');
        GenerateCodes(root->left, code, symbols, codes, n);
        code.pop_back();
    }

    // 向右走，路径加 '1'
    if (root->right != nullptr)
    {
        code.push_back('1');
        GenerateCodes(root->right, code, symbols, codes, n);
        code.pop_back();
    }
}

void PrintCodes(const char* symbols, const std::string* codes, int n)
{
    for (int i = 0; i < n; ++i)
    {
        std::cout << "字符 " << symbols[i] << " 的编码为: " << codes[i] << '\n';
    }
}

// 释放整棵树
void DestroyTree(HuffmanTree root)
{
    if (!root) return;
    DestroyTree(root->left);
    DestroyTree(root->right);
    delete root;
}

int main()
{
    int n = 4;
    int  w[]    = {7, 5, 2, 4};
    char data[] = {'a', 'b', 'c', 'd'};

    // 构建链式哈夫曼树
    HuffmanTree root = BuildHuffmanTree(w, data, n);

    // 准备编码表
    char* symbols = data;               // 直接用原 data 数组
    std::string* codes = new std::string[n];

    std::string path;
    GenerateCodes(root, path, symbols, codes, n);

    PrintCodes(symbols, codes, n);

    delete[] codes;
    DestroyTree(root);

    return 0;
}