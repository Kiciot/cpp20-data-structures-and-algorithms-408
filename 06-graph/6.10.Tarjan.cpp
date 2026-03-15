#include <iostream>

constexpr int MaxV = 26;
// 无向图最多边数：如果你需要更大，改这个
constexpr int MaxE = 200;

using VertexType = char;

struct ArcNode {
    int to{};
    int eid{};              // 这条“无向边”的编号（两个方向共用同一个 eid）
    ArcNode* next{nullptr};
};

struct VNode {
    VertexType data{};
    ArcNode* first{nullptr};
};

struct ALGraph {
    VNode adj[MaxV]{};
    int n{};                // 顶点数
    int undirectedEdges{};  // 无向边条数（每条边会建两个 ArcNode）
};

// ===== 加无向边：同一个 eid 会写进两个方向的 ArcNode =====
bool AddEdgeUndirected(ALGraph& G, int u, int v, int eid)
{
    if (u < 0 || u >= G.n || v < 0 || v >= G.n) return false;

    auto* a = new ArcNode{v, eid, G.adj[u].first};
    G.adj[u].first = a;

    auto* b = new ArcNode{u, eid, G.adj[v].first};
    G.adj[v].first = b;

    return true;
}

void Destroy(ALGraph& G)
{
    for (int i = 0; i < G.n; ++i) {
        ArcNode* p = G.adj[i].first;
        while (p) {
            ArcNode* nxt = p->next;
            delete p;
            p = nxt;
        }
        G.adj[i].first = nullptr;
    }
}

// ===== Tarjan：割点 & 割边 =====
struct TarjanResult {
    bool isCut[MaxV]{};
    int  bridgeU[MaxE]{};
    int  bridgeV[MaxE]{};
    int  bridgeCnt{};
};

inline int Min(int a, int b) { return a < b ? a : b; }

void DFS_Tarjan(
    const ALGraph& G,
    int u,
    int parent,        // 父顶点（根的 parent = -1）
    int parentEid,     // “从父到 u 的那条边”的 eid（根用 -1）
    int& timer,
    int dfn[MaxV],
    int low[MaxV],
    TarjanResult& res
) {
    dfn[u] = low[u] = ++timer;

    int childCnt = 0; // 统计 DFS 树孩子数（根节点判断割点用）

    for (ArcNode* p = G.adj[u].first; p; p = p->next) {
        int v = p->to;
        int eid = p->eid;

        if (dfn[v] == 0) {
            // 树边
            ++childCnt;
            DFS_Tarjan(G, v, u, eid, timer, dfn, low, res);

            low[u] = Min(low[u], low[v]);

            // ===== 割边判定：low[v] > dfn[u] =====
            if (low[v] > dfn[u]) {
                // 记录割边 u-v
                if (res.bridgeCnt < MaxE) {
                    res.bridgeU[res.bridgeCnt] = u;
                    res.bridgeV[res.bridgeCnt] = v;
                    ++res.bridgeCnt;
                }
            }

            // ===== 割点判定（非根）：low[v] >= dfn[u] =====
            if (parent != -1 && low[v] >= dfn[u]) {
                res.isCut[u] = true;
            }
        } else {
            // 返祖边 / 回边（注意要排除“父边的反向边”）
            if (eid != parentEid) {
                low[u] = Min(low[u], dfn[v]);
            }
        }
    }

    // ===== 根节点割点判定：孩子数 >= 2 =====
    if (parent == -1 && childCnt >= 2) {
        res.isCut[u] = true;
    }
}

TarjanResult FindCutsAndBridges(const ALGraph& G)
{
    TarjanResult res;

    int dfn[MaxV]{};
    int low[MaxV]{};
    int timer = 0;

    // 可能是非连通图：对每个连通分量都跑一遍
    for (int i = 0; i < G.n; ++i) {
        if (dfn[i] == 0) {
            DFS_Tarjan(G, i, -1, -1, timer, dfn, low, res);
        }
    }

    return res;
}

// ===== 演示 =====
int main()
{
    ALGraph G;
    G.n = 7;
    for (int i = 0; i < G.n; ++i) G.adj[i].data = char('A' + i);

    // 构造一个有割点/割边的无向图
    // 0:A 1:B 2:C 3:D 4:E 5:F 6:G
    // 边：
    // A-B, B-C, C-A （三角形）
    // C-D （割边）
    // D-E, E-F, F-D （三角形）
    // E-G （割边）
    int eid = 0;
    AddEdgeUndirected(G, 0, 1, eid++); // A-B
    AddEdgeUndirected(G, 1, 2, eid++); // B-C
    AddEdgeUndirected(G, 2, 0, eid++); // C-A

    AddEdgeUndirected(G, 2, 3, eid++); // C-D  bridge

    AddEdgeUndirected(G, 3, 4, eid++); // D-E
    AddEdgeUndirected(G, 4, 5, eid++); // E-F
    AddEdgeUndirected(G, 5, 3, eid++); // F-D

    AddEdgeUndirected(G, 4, 6, eid++); // E-G  bridge

    auto res = FindCutsAndBridges(G);

    std::cout << "Cut vertices (articulation points): ";
    bool anyCut = false;
    for (int i = 0; i < G.n; ++i) {
        if (res.isCut[i]) {
            std::cout << G.adj[i].data << ' ';
            anyCut = true;
        }
    }
    if (!anyCut) std::cout << "(none)";
    std::cout << "\n";

    std::cout << "Bridges (cut edges):\n";
    if (res.bridgeCnt == 0) {
        std::cout << "(none)\n";
    } else {
        for (int i = 0; i < res.bridgeCnt; ++i) {
            int u = res.bridgeU[i];
            int v = res.bridgeV[i];
            std::cout << G.adj[u].data << " - " << G.adj[v].data << "\n";
        }
    }

    Destroy(G);
    return 0;
}