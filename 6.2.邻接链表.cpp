#include <iostream>
#include <string>
#include <string_view>

// ================== 循环队列（辅助 BFS） ==================

constexpr int MaxSize = 50;   // 队列最大元素个数

struct SqQueue {
    int data[MaxSize]{};
    int front{0};
    int rear{0};
};

void Queue_Init(SqQueue& Q) noexcept {
    Q.front = Q.rear = 0;
}

bool Queue_Empty(const SqQueue& Q) noexcept {
    return Q.front == Q.rear;
}

bool Queue_En(SqQueue& Q, int e) noexcept {
    if ((Q.rear + 1) % MaxSize == Q.front) return false; // 队满
    Q.data[Q.rear] = e;
    Q.rear = (Q.rear + 1) % MaxSize;
    return true;
}

bool Queue_De(SqQueue& Q, int& e) noexcept {
    if (Queue_Empty(Q)) return false; // 队空
    e = Q.data[Q.front];
    Q.front = (Q.front + 1) % MaxSize;
    return true;
}

// ================== 图的常量别名与结构定义 ==================

constexpr int MaxVertexNum = 26;
constexpr bool Directed = true;   // 是否为有向图
using VertexType = char;
using ArcWeight  = int;

// 边表结点
struct ArcNode {
    int adjvex{};          // 该边指向的顶点下标
    ArcWeight weight{};    // 权值
    ArcNode* next{nullptr}; // 指向下一条边的指针
};

// 顶点表结点
struct VNode {
    VertexType data{};     // 顶点信息
    ArcNode* first{nullptr}; // 指向该顶点的第一条边的指针
};

using AdjList = VNode[MaxVertexNum];

// 邻接表的图结构
struct ALGraph {
    AdjList adjList{};
    int vexnum{};          // 顶点数
    int arcnum{};          // 边数
};


// ================== 图的创建 ==================

void ALGraph_Create(ALGraph& G) {
    // 顶点表初始化
    for (int i = 0; i < MaxVertexNum; ++i) {
        G.adjList[i].first = nullptr;
    }

    std::cout << "输入顶点数 和 边数（空格间隔）：";
    std::cin >> G.vexnum >> G.arcnum;

    // 建立顶点表
    for (int i = 0; i < G.vexnum; ++i) {
        std::cout << "请输入第 " << (i + 1) << " 个顶点的值：";
        std::cin >> G.adjList[i].data;
        G.adjList[i].first = nullptr;
    }

    // 建立边表
    int vi, vj;
    ArcWeight weight{};
    std::cout << "下标为 -1 -1 则退出创建边\n";
    for (int i = 0; i < G.arcnum; ++i) {
        std::cout << "请输入要创建边的两个顶点下标 vi vj：";
        std::cin >> vi >> vj;
        if (vi == -1 && vj == -1) break;

        std::cout << "请输入边 [" << vi << " -> " << vj << "] 的权值 weight = ";
        std::cin >> weight;

        // 头插法插入 vi -> vj
        ArcNode* arcNode = new ArcNode{};
        arcNode->adjvex = vj;
        arcNode->weight = weight;
        arcNode->next   = G.adjList[vi].first;
        G.adjList[vi].first = arcNode;

        // 如果是无向图，还需要插一条 vj -> vi
        if (!Directed) {
            ArcNode* arcNode2 = new ArcNode{};
            arcNode2->adjvex = vi;
            arcNode2->weight = weight;
            arcNode2->next   = G.adjList[vj].first;
            G.adjList[vj].first = arcNode2;
        }
    }
}

// ================== 只遍历一次的 BFS/DFS（支持非连通图） ==================

void BFS_From(const ALGraph& G, int start, bool visited[]) {
    SqQueue Q;
    Queue_Init(Q);

    visited[start] = true;
    Queue_En(Q, start);

    // 输出包含起点
    std::cout << start << "(" << G.adjList[start].data << "), ";

    while (!Queue_Empty(Q)) {
        int u;
        Queue_De(Q, u);

        for (ArcNode* p = G.adjList[u].first; p != nullptr; p = p->next) {
            int v = p->adjvex;
            if (!visited[v]) {
                visited[v] = true;
                Queue_En(Q, v);
                std::cout << v << "(" << G.adjList[v].data << "), ";
            }
        }
    }
}

void BFS_Traverse(const ALGraph& G) {
    bool visited[MaxVertexNum]{false};

    std::cout << "\nBFS（整张图只遍历一次，自动覆盖非连通部分）：\n";
    for (int i = 0; i < G.vexnum; ++i) {
        if (!visited[i]) {
            std::cout << "component from " << i << "(" << G.adjList[i].data << "): ";
            BFS_From(G, i, visited);
            std::cout << '\n';
        }
    }
}

void DFS_From(const ALGraph& G, int u, bool visited[]) {
    visited[u] = true;
    std::cout << u << "(" << G.adjList[u].data << "), ";

    for (ArcNode* p = G.adjList[u].first; p != nullptr; p = p->next) {
        int v = p->adjvex;
        if (!visited[v]) {
            DFS_From(G, v, visited);
        }
    }
}

void DFS_Traverse(const ALGraph& G) {
    bool visited[MaxVertexNum]{false};

    std::cout << "\nDFS（整张图只遍历一次，自动覆盖非连通部分）：\n";
    for (int i = 0; i < G.vexnum; ++i) {
        if (!visited[i]) {
            std::cout << "component from " << i << "(" << G.adjList[i].data << "): ";
            DFS_From(G, i, visited);
            std::cout << '\n';
        }
    }
}

// ================== 打印邻接表 ==================

void ALGraph_Show(const ALGraph& G) {
    std::cout << "\n当前邻接表为：\n";
    for (int i = 0; i < G.vexnum; ++i) {
        std::cout << G.adjList[i].data;
        ArcNode* p = G.adjList[i].first;
        while (p != nullptr) {
            std::cout << "->" << p->adjvex;
            p = p->next;
        }
        std::cout << '\n';
    }
}

// ================== 释放图的边结点（可选，但建议养成习惯） ==================

void ALGraph_Destroy(ALGraph& G) {
    for (int i = 0; i < G.vexnum; ++i) {
        ArcNode* p = G.adjList[i].first;
        while (p != nullptr) {
            ArcNode* next = p->next;
            delete p;
            p = next;
        }
        G.adjList[i].first = nullptr;
    }
}

int main() {
    std::cout << "注意，当前创建图为有向图\n";

    ALGraph G;
    ALGraph_Create(G);
    ALGraph_Show(G);

    BFS_Traverse(G);
    DFS_Traverse(G);

    ALGraph_Destroy(G);
    return 0;
}