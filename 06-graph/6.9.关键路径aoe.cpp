#include <iostream>

using VertexType = char;
using EdgeType   = int;

constexpr int MAXVEX = 100;
constexpr EdgeType INF = 65535;
constexpr bool DIRECTED = true;

// ===== 图（邻接矩阵）=====
struct MGraph {
    VertexType vexs[MAXVEX]{};
    EdgeType arc[MAXVEX][MAXVEX]{};
    int vexNum{};
    int edgeNum{};
};

// ===== 手写顺序栈（存顶点下标）=====
struct SqStack {
    int data[MAXVEX]{};
    int top{-1};
};

void Stack_Init(SqStack& s) noexcept { s.top = -1; }
bool Stack_Empty(const SqStack& s) noexcept { return s.top == -1; }
bool Stack_Full(const SqStack& s) noexcept { return s.top == MAXVEX - 1; }

bool Stack_Push(SqStack& s, int x) noexcept {
    if (Stack_Full(s)) return false;
    s.data[++s.top] = x;
    return true;
}

bool Stack_Pop(SqStack& s, int& x) noexcept {
    if (Stack_Empty(s)) return false;
    x = s.data[s.top--];
    return true;
}

// ===== 打印图 =====
void Graph_Show(const MGraph& G) {
    std::cout << "顶点：";
    for (int i = 0; i < G.vexNum; ++i) std::cout << G.vexs[i] << ' ';
    std::cout << "\n邻接矩阵：\n";
    for (int i = 0; i < G.vexNum; ++i) {
        for (int j = 0; j < G.vexNum; ++j) {
            if (G.arc[i][j] == INF) std::cout << "   ∞";
            else std::cout << (G.arc[i][j] < 10 ? "    " : "   ") << G.arc[i][j];
        }
        std::cout << '\n';
    }
}

// ===== 关键路径（AOE）=====
// 返回 false 表示图有环（不是 DAG）
bool Critical_Path(const MGraph& G) {
    // 1) 拓扑排序（Kahn，栈实现）
    int indegree[MAXVEX]{};
    for (int u = 0; u < G.vexNum; ++u) {
        for (int v = 0; v < G.vexNum; ++v) {
            if (G.arc[u][v] != INF) {
                ++indegree[v];
            }
        }
    }

    SqStack S;
    Stack_Init(S);
    for (int i = 0; i < G.vexNum; ++i) {
        if (indegree[i] == 0) Stack_Push(S, i);
    }

    int topo[MAXVEX]{};
    int topoLen = 0;

    while (!Stack_Empty(S)) {
        int u;
        Stack_Pop(S, u);
        topo[topoLen++] = u;

        for (int v = 0; v < G.vexNum; ++v) {
            if (G.arc[u][v] != INF) {
                if (--indegree[v] == 0) Stack_Push(S, v);
            }
        }
    }

    std::cout << "\n拓扑序（下标）：";
    for (int i = 0; i < topoLen; ++i) std::cout << topo[i] << ' ';
    std::cout << '\n';

    if (topoLen != G.vexNum) {
        std::cout << "图存在回路（不是 DAG），无法求关键路径。\n";
        return false;
    }

    // 2) 事件最早发生时间 ve（按拓扑序，扫描出边更新）
    int ve[MAXVEX]{};
    for (int i = 0; i < G.vexNum; ++i) ve[i] = 0;

    for (int idx = 0; idx < topoLen; ++idx) {
        int u = topo[idx];
        for (int v = 0; v < G.vexNum; ++v) {
            EdgeType w = G.arc[u][v];
            if (w != INF) {
                if (ve[u] + w > ve[v]) ve[v] = ve[u] + w;
            }
        }
    }

    int veMax = 0;
    for (int i = 0; i < G.vexNum; ++i) if (ve[i] > veMax) veMax = ve[i];

    // 3) 事件最迟发生时间 vl（逆拓扑序）
    int vl[MAXVEX]{};
    for (int i = 0; i < G.vexNum; ++i) vl[i] = veMax;

    for (int idx = topoLen - 1; idx >= 0; --idx) {
        int u = topo[idx];
        for (int v = 0; v < G.vexNum; ++v) {
            EdgeType w = G.arc[u][v];
            if (w != INF) {
                if (vl[v] - w < vl[u]) vl[u] = vl[v] - w;
            }
        }
    }

    // 4/5/6/7) 对每条活动边 u->v 计算 e/l/d，d==0 为关键活动
    std::cout << "\n关键活动（关键边，d==0）：\n";
    std::cout << "u -> v  w  (e,l,d)\n";

    // 下面三个数组只是为了把 e/l/d 全打印出来（和你原版风格一致）
    int eArr[MAXVEX * MAXVEX]{};
    int lArr[MAXVEX * MAXVEX]{};
    int dArr[MAXVEX * MAXVEX]{};
    int edgeLen = 0;

    for (int u = 0; u < G.vexNum; ++u) {
        for (int v = 0; v < G.vexNum; ++v) {
            EdgeType w = G.arc[u][v];
            if (w == INF) continue;

            int e = ve[u];
            int l = vl[v] - w;
            int d = l - e;

            eArr[edgeLen] = e;
            lArr[edgeLen] = l;
            dArr[edgeLen] = d;
            ++edgeLen;

            if (d == 0) {
                std::cout << u << " -> " << v
                          << "  " << w
                          << "  (" << e << "," << l << "," << d << ")\n";
            }
        }
    }

    std::cout << "\nve：";
    for (int i = 0; i < G.vexNum; ++i) std::cout << ve[i] << ' ';
    std::cout << "\nvl：";
    for (int i = 0; i < G.vexNum; ++i) std::cout << vl[i] << ' ';

    std::cout << "\n\ne（按边扫描顺序）：";
    for (int i = 0; i < edgeLen; ++i) std::cout << eArr[i] << ' ';
    std::cout << "\nl（按边扫描顺序）：";
    for (int i = 0; i < edgeLen; ++i) std::cout << lArr[i] << ' ';
    std::cout << "\nd（按边扫描顺序）：";
    for (int i = 0; i < edgeLen; ++i) std::cout << dArr[i] << ' ';
    std::cout << "\n";

    return true;
}

int main() {
    MGraph G;

    G.vexs[0] = '1';
    G.vexs[1] = '2';
    G.vexs[2] = '3';
    G.vexs[3] = '4';
    G.vexs[4] = '5';
    G.vexs[5] = '6';
    G.vexNum = 6;

    // 初始化 INF
    for (int i = 0; i < G.vexNum; ++i)
        for (int j = 0; j < G.vexNum; ++j)
            G.arc[i][j] = INF;

    // AOE 边权=活动持续时间（有向）
    G.arc[0][1] = 3;
    G.arc[0][2] = 2;
    G.arc[1][3] = 2;
    G.arc[1][4] = 3;
    G.arc[2][3] = 4;
    G.arc[2][5] = 3;
    G.arc[3][5] = 2;
    G.arc[4][5] = 1;
    G.edgeNum = 8;

    Graph_Show(G);
    Critical_Path(G);
    return 0;
}