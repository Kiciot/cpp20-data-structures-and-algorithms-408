#include <iostream>

constexpr int MaxVertexNum = 26;
using VertexType = char;
using ArcWeight  = int;

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

bool IndexOK(const ALGraph& G, int idx) noexcept {
    return 0 <= idx && idx < G.vexnum;
}

bool ALGraph_AddEdge(ALGraph& G, int source, int target, ArcWeight weight)
{
    if (!IndexOK(G, source) || !IndexOK(G, target)) return false;

    auto* node = new ArcNode{};
    node->adjvex = target;
    node->weight = weight;
    node->next   = G.adjList[source].first;
    G.adjList[source].first = node;
    return true;
}

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

bool TopologicalSort(const ALGraph& G)
{
    int indegree[MaxVertexNum]{};

    // 1) 统计入度
    for (int u = 0; u < G.vexnum; ++u) {
        for (ArcNode* p = G.adjList[u].first; p; p = p->next) {
            ++indegree[p->adjvex];
        }
    }

    // 2) 入度为 0 的点入栈
    Stack S;
    Stack_Init(S);
    for (int i = 0; i < G.vexnum; ++i) {
        if (indegree[i] == 0) {
            if (!Stack_Push(S, i)) {
                std::cout << "栈满（不应发生），拓扑排序失败\n";
                return false;
            }
        }
    }

    // 3) 反复弹出、删除出边、更新入度
    int count = 0;
    while (!Stack_IsEmpty(S)) {
        int v;
        if (!Stack_Pop(S, v)) break;

        std::cout << G.adjList[v].data;
        ++count;

        for (ArcNode* p = G.adjList[v].first; p; p = p->next) {
            int to = p->adjvex;
            if (--indegree[to] == 0) {
                if (!Stack_Push(S, to)) {
                    std::cout << "\n栈满（不应发生），拓扑排序失败\n";
                    return false;
                }
            }
        }
    }

    if (count != G.vexnum) {
        std::cout << "\n图存在回路！\n";
        return false;
    }

    std::cout << '\n';
    return true;
}

int main()
{
    std::cout << "注意：当前创建图为有向图\n";

    ALGraph G;
    G.vexnum = 5;
    G.arcnum = 7;

    G.adjList[0].data = 'a';
    G.adjList[1].data = 'b';
    G.adjList[2].data = 'c';
    G.adjList[3].data = 'd';
    G.adjList[4].data = 'e';

    ALGraph_AddEdge(G, 0, 1, 0);
    ALGraph_AddEdge(G, 0, 3, 0);
    ALGraph_AddEdge(G, 1, 3, 0);
    ALGraph_AddEdge(G, 1, 2, 0);
    ALGraph_AddEdge(G, 3, 4, 0);
    ALGraph_AddEdge(G, 3, 2, 0);
    ALGraph_AddEdge(G, 2, 4, 0);

    ALGraph_Show(G);
    TopologicalSort(G);

    ALGraph_Destroy(G);
    return 0;
}