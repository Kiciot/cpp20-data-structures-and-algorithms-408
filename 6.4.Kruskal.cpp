#include <iostream>
#include <algorithm> // std::sort

constexpr int MaxVertexNum = 26;

using VertexType = char;
using EdgeType   = int;

// ===== 邻接表 =====
struct ArcNode {
    int adjvex{};
    EdgeType weight{};
    ArcNode* next{nullptr};
};

struct VNode {
    VertexType data{};
    ArcNode* first{nullptr};
};

struct ALGraph {
    VNode adjList[MaxVertexNum]{};
    int vexnum{};
    int arcnum{}; // 逻辑边数（无向图一般计“无向边条数”）
};

bool AddEdgeUndirected(ALGraph& G, int u, int v, EdgeType w)
{
    if (u < 0 || u >= G.vexnum || v < 0 || v >= G.vexnum) return false;

    auto* a = new ArcNode{v, w, G.adjList[u].first};
    G.adjList[u].first = a;

    auto* b = new ArcNode{u, w, G.adjList[v].first};
    G.adjList[v].first = b;

    return true;
}

void Destroy(ALGraph& G)
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

void Show(const ALGraph& G)
{
    for (int i = 0; i < G.vexnum; ++i) {
        std::cout << G.adjList[i].data;
        for (ArcNode* p = G.adjList[i].first; p; p = p->next) {
            std::cout << "->" << G.adjList[p->adjvex].data << "(" << p->weight << ")";
        }
        std::cout << '\n';
    }
}

// ===== Kruskal 需要的边集 =====
struct Edge {
    int u{};
    int v{};
    EdgeType w{};
};

// 从邻接表抽边：无向图会存两条(u->v, v->u)，用 u<v 去重
Edge* BuildEdgeList_Undirected(const ALGraph& G, int& edgeCount)
{
    edgeCount = 0;

    // 先数边（只计 u<v 的那一半）
    for (int u = 0; u < G.vexnum; ++u) {
        for (ArcNode* p = G.adjList[u].first; p; p = p->next) {
            int v = p->adjvex;
            if (u < v) ++edgeCount;
        }
    }

    Edge* edges = new Edge[edgeCount];
    int k = 0;

    for (int u = 0; u < G.vexnum; ++u) {
        for (ArcNode* p = G.adjList[u].first; p; p = p->next) {
            int v = p->adjvex;
            if (u < v) {
                edges[k++] = Edge{u, v, p->weight};
            }
        }
    }

    return edges;
}

// ===== 并查集（路径压缩 + 按大小合并）=====
struct DSU {
    int parent[MaxVertexNum]{};
    int sz[MaxVertexNum]{};

    void init(int n) {
        for (int i = 0; i < n; ++i) { parent[i] = i; sz[i] = 1; }
    }

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }

    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) std::swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }
};

void Kruskal(const ALGraph& G)
{
    int edgeCount = 0;
    Edge* edges = BuildEdgeList_Undirected(G, edgeCount);

    std::sort(edges, edges + edgeCount,
              [](const Edge& a, const Edge& b) { return a.w < b.w; });

    DSU dsu;
    dsu.init(G.vexnum);

    int chosen = 0;
    long long total = 0;

    std::cout << "Edges chosen in MST:\n";
    for (int i = 0; i < edgeCount && chosen < G.vexnum - 1; ++i) {
        const int u = edges[i].u;
        const int v = edges[i].v;
        const int w = edges[i].w;

        if (dsu.unite(u, v)) {
            std::cout << "u=" << u << " v=" << v << " w=" << w << '\n';
            ++chosen;
            total += w;
        }
    }

    if (chosen != G.vexnum - 1) {
        std::cout << "Graph is disconnected: MST does not exist (got a forest).\n";
    }
    std::cout << "Total weight = " << total << '\n';

    delete[] edges;
}

int main()
{
    ALGraph G;
    G.vexnum = 6;

    G.adjList[0].data = 'a';
    G.adjList[1].data = 'b';
    G.adjList[2].data = 'c';
    G.adjList[3].data = 'd';
    G.adjList[4].data = 'e';
    G.adjList[5].data = 'f';

    // 无向边：AddEdgeUndirected 会插入两条 ArcNode
    AddEdgeUndirected(G, 0, 1, 6);
    AddEdgeUndirected(G, 0, 3, 5);
    AddEdgeUndirected(G, 0, 2, 1);
    AddEdgeUndirected(G, 1, 2, 5);
    AddEdgeUndirected(G, 2, 3, 5);
    AddEdgeUndirected(G, 1, 4, 3);
    AddEdgeUndirected(G, 2, 4, 6);
    AddEdgeUndirected(G, 2, 5, 4);
    AddEdgeUndirected(G, 3, 5, 2);
    AddEdgeUndirected(G, 4, 5, 6);

    std::cout << "AdjList:\n";
    Show(G);

    Kruskal(G);

    Destroy(G);
    return 0;
}