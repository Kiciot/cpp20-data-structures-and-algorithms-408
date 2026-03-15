#include <iostream>

constexpr int MaxVertexNum = 26;

using VertexType = char;
using ArcWeight  = int;

// ===== 邻接表结构 =====
struct ArcNode {
    int adjvex{};
    ArcWeight weight{};
    ArcNode* next{nullptr};
};

struct VNode {
    VertexType data{};
    ArcNode* first{nullptr};
};

struct ALGraph {
    VNode adjList[MaxVertexNum]{};
    int vexnum{};
    int arcnum{};
};

// ===== 手写栈（存顶点下标）=====
// DFS 拓扑排序里：
// “一个点的所有后继都处理完 -> 才 push 进栈”
// 最终拓扑序 = 栈从顶到底（或数组逆序输出）
struct Stack {
    int data[MaxVertexNum]{};
    int top{-1};
};

void Stack_Init(Stack& s) noexcept { s.top = -1; }
bool Stack_IsEmpty(const Stack& s) noexcept { return s.top == -1; }
bool Stack_IsFull(const Stack& s) noexcept { return s.top == MaxVertexNum - 1; }

bool Stack_Push(Stack& s, int v) noexcept {
    if (Stack_IsFull(s)) return false;
    s.data[++s.top] = v;
    return true;
}

bool Stack_Pop(Stack& s, int& out) noexcept {
    if (Stack_IsEmpty(s)) return false;
    out = s.data[s.top--];
    return true;
}

// ===== 添加边（头插法）=====
bool ALGraph_AddEdge(ALGraph& G, int source, int target, ArcWeight weight)
{
    if (source < 0 || source >= G.vexnum) return false;
    if (target < 0 || target >= G.vexnum) return false;

    auto* node = new ArcNode{};
    node->adjvex = target;
    node->weight = weight;
    node->next   = G.adjList[source].first;
    G.adjList[source].first = node;
    return true;
}

// ===== 释放边结点（防内存泄漏）=====
void ALGraph_Destroy(ALGraph& G)
{
    for (int i = 0; i < G.vexnum; ++i) {
        ArcNode* p = G.adjList[i].first;
        while (p) {
            ArcNode* nxt = p->next;
            delete p;
            p = nxt;
        }
        G.adjList[i].first = nullptr;
    }
}

void ALGraph_Show(const ALGraph& G)
{
    for (int i = 0; i < G.vexnum; ++i) {
        std::cout << G.adjList[i].data;
        for (ArcNode* p = G.adjList[i].first; p; p = p->next) {
            std::cout << "->" << G.adjList[p->adjvex].data;
        }
        std::cout << '\n';
    }
}

// ===== DFS 拓扑排序核心 =====
// state: 0=未访问, 1=正在递归栈上(回路检测), 2=已完成
// 返回 false 表示发现回路（不是 DAG）
bool DFS_Topo(int u, const ALGraph& G, int state[], Stack& out)
{
    state[u] = 1; // 进入递归栈（灰）

    for (ArcNode* p = G.adjList[u].first; p; p = p->next) {
        int v = p->adjvex;

        if (state[v] == 0) {
            if (!DFS_Topo(v, G, state, out)) return false;
        } else if (state[v] == 1) {
            // 碰到“正在递归栈上的点” => 有环
            return false;
        }
        // state[v]==2 已完成，跳过即可
    }

    state[u] = 2;          // 完成（黑）
    Stack_Push(out, u);    // 关键：回溯时入栈（逆后序）
    return true;
}

// DFS 型拓扑排序：遍历所有点，处理非连通情况
bool TopologicalSort_DFS(const ALGraph& G)
{
    int state[MaxVertexNum]{};
    for (int i = 0; i < G.vexnum; ++i) state[i] = 0;

    Stack st;
    Stack_Init(st);

    for (int i = 0; i < G.vexnum; ++i) {
        if (state[i] == 0) {
            if (!DFS_Topo(i, G, state, st)) {
                std::cout << "图存在回路（不是 DAG），无法拓扑排序。\n";
                return false;
            }
        }
    }

    // st 里从底到顶是“入栈顺序”，拓扑序应该从“栈顶往下”输出
    std::cout << "DFS拓扑序：";
    while (!Stack_IsEmpty(st)) {
        int v;
        Stack_Pop(st, v);
        std::cout << G.adjList[v].data;
    }
    std::cout << '\n';
    return true;
}

int main()
{
    std::cout << "DFS型拓扑排序（逆后序，回溯入栈）\n";

    ALGraph G;
    G.vexnum = 5;
    G.arcnum = 7;

    G.adjList[0].data = 'a';
    G.adjList[1].data = 'b';
    G.adjList[2].data = 'c';
    G.adjList[3].data = 'd';
    G.adjList[4].data = 'e';

    // 建边
    ALGraph_AddEdge(G, 0, 1, 0); // a->b
    ALGraph_AddEdge(G, 0, 3, 0); // a->d
    ALGraph_AddEdge(G, 1, 3, 0); // b->d
    ALGraph_AddEdge(G, 1, 2, 0); // b->c
    ALGraph_AddEdge(G, 3, 4, 0); // d->e
    ALGraph_AddEdge(G, 3, 2, 0); // d->c
    ALGraph_AddEdge(G, 2, 4, 0); // c->e

    ALGraph_Show(G);
    TopologicalSort_DFS(G);

    ALGraph_Destroy(G);
    return 0;
}