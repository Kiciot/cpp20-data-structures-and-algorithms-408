#include <iostream>
#include <string>
#include <algorithm> // 用于 std::reverse

// 定义节点结构
struct HTNode
{
    int weight;  // 权值
    char data;   // 存的字符
    int parent;  // 父节点下标，-1 表示无父节点
    int lchild;  // 左孩子下标，-1 表示无左孩子
    int rchild;  // 右孩子下标，-1 表示无右孩子
};

using HuffmanTree = HTNode*;
using HuffmanCode = std::string*;

// 在 HT[0...range-1] 中选择两个 parent 为 -1 且 weight 最小的结点
// 将它们的下标分别传给 min1 和 min2
void Select(HuffmanTree HT, int range, int& min1, int& min2)
{
    int i;
    min1 = -1;
    min2 = -1;

    // 1. 寻找第一个 parent 为 -1 的节点作为 min1 的初始值
    for (i = 0; i < range; ++i)
    {
        if (HT[i].parent == -1)
        {
            min1 = i;
            break;
        }
    }

    // 2. 遍历剩下的，找到权值最小的 min1
    for (int k = i + 1; k < range; ++k)
    {
        if (HT[k].parent == -1)
        {
            if (HT[k].weight < HT[min1].weight)
            {
                min1 = k;
            }
        }
    }

    // 3. 寻找第一个 parent 为 -1 且不是 min1 的节点作为 min2 的初始值
    for (i = 0; i < range; ++i)
    {
        if (HT[i].parent == -1 && i != min1)
        {
            min2 = i;
            break;
        }
    }

    // 4. 遍历剩下的，找到权值次小的 min2
    for (int k = i + 1; k < range; ++k)
    {
        if (HT[k].parent == -1 && k != min1)
        {
            if (HT[k].weight < HT[min2].weight)
            {
                min2 = k;
            }
        }
    }
}

// 创建哈夫曼树
void Huffman_Create(HuffmanTree& HT, const int* w, const char* data, int n)
{
    if (n <= 0) return;

    int m = 2 * n - 1; // 总节点数
    HT = new HTNode[m]; // 分配内存

    // --- 初始化所有节点 ---
    // 将 parent, lchild, rchild 都置为 -1，避免 0 下标混淆
    for (int i = 0; i < m; ++i)
    {
        HT[i].parent = -1;
        HT[i].lchild = -1;
        HT[i].rchild = -1;
        HT[i].weight = 0;
        HT[i].data = '\0';
    }

    // --- 初始化叶子节点 (前 n 个) ---
    for (int i = 0; i < n; ++i)
    {
        HT[i].weight = w[i];
        HT[i].data = data[i];
    }

    // --- 构建哈夫曼树 (后 n-1 个节点) ---
    for (int i = n; i < m; ++i)
    {
        int min1, min2;
        Select(HT, i, min1, min2); // 在前 i 个节点中选

        HT[i].weight = HT[min1].weight + HT[min2].weight;
        HT[i].lchild = min1;
        HT[i].rchild = min2;

        // 更新孩子的父节点信息
        HT[min1].parent = i;
        HT[min2].parent = i;
    }
}

// 生成哈夫曼编码
void Huffman_Coding(HuffmanTree HT, HuffmanCode& HC, int n)
{
    if (HT == NULL || n <= 0) return;

    HC = new std::string[n];

    // 逐个字符求编码
    for (int i = 0; i < n; ++i)
    {
        std::string code = "";
        int j = i;              // 当前节点
        int p = HT[j].parent;   // 父节点

        // 向上回溯直到根节点 (根节点的 parent 为 -1)
        while (p != -1)
        {
            if (HT[p].lchild == j)
            {
                code.push_back('0'); // 左孩子编码 0
            }
            else
            {
                code.push_back('1'); // 右孩子编码 1
            }
            
            // 继续向上
            j = p;
            p = HT[j].parent;
        }

        // 因为是自底向上求的，所以需要反转字符串
        std::reverse(code.begin(), code.end());
        HC[i] = code; 
    }
}

// 打印哈夫曼树结构表
void HuffmanTree_Print(HuffmanTree HT, int totalNodes)
{
    if (HT == NULL) return;

    std::cout << "--- 哈夫曼树结构 (数组实现) ---\n";
    std::cout << "下标\t字符\t权值\t父节点\t左孩子\t右孩子\n";

    for (int i = 0; i < totalNodes; ++i)
    {
        std::cout << i << "\t"
                  << (HT[i].data == '\0' ? '-' : HT[i].data) << "\t"
                  << HT[i].weight << "\t"
                  << HT[i].parent << "\t"
                  << HT[i].lchild << "\t"
                  << HT[i].rchild << "\n";
    }
    std::cout << "\n";
}

// 打印编码结果
void HuffmanCode_Print(HuffmanTree HT, HuffmanCode HC, int n)
{
    if (HT == NULL || HC == NULL) return;

    std::cout << "--- 哈夫曼编码结果 ---\n";
    for (int i = 0; i < n; ++i)
    {
        std::cout << "字符: " << HT[i].data 
                  << "  权值: " << HT[i].weight 
                  << "  编码: " << HC[i] << "\n";
    }
}

// 销毁内存
void Huffman_Destroy(HuffmanTree& HT, HuffmanCode& HC)
{
    if (HT != NULL)
    {
        delete[] HT;
        HT = NULL;
    }
    
    if (HC != NULL)
    {
        delete[] HC;
        HC = NULL;
    }
}

int main()
{
    const int n = 4;
    int w[] = {7, 5, 2, 4};
    char data[] = {'a', 'b', 'c', 'd'};

    HuffmanTree HT = NULL;
    HuffmanCode HC = NULL;

    // 1. 创建哈夫曼树
    Huffman_Create(HT, w, data, n);

    // 2. 打印树的存储结构 (总节点数 = 2*n - 1)
    int totalNodes = 2 * n - 1;
    HuffmanTree_Print(HT, totalNodes);

    // 3. 生成并打印编码
    Huffman_Coding(HT, HC, n);
    HuffmanCode_Print(HT, HC, n);

    // 4. 释放资源
    Huffman_Destroy(HT, HC);

    return 0;
}