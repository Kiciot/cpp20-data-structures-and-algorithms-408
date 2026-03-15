#include <iostream>

using VertexType = char;
using EdgeType   = int;

constexpr int MAXVEX = 100;
constexpr EdgeType INF = 65535;

// 邻接矩阵图
struct MGraph
{
    VertexType vexs[MAXVEX]{};
    EdgeType arc[MAXVEX][MAXVEX]{};
    int vexNum{};
    int edgeNum{};
};

void InitGraph(MGraph& G)
{
    for (int i = 0; i < G.vexNum; ++i)
    {
        for (int j = 0; j < G.vexNum; ++j)
            G.arc[i][j] = INF;
        G.arc[i][i] = 0; // 自环距离 0，更规范
    }
}

void Graph_Show(const MGraph& G)
{
    for (int i = 0; i < G.vexNum; ++i) std::cout << G.vexs[i];
    std::cout << '\n';

    for (int i = 0; i < G.vexNum; ++i)
    {
        for (int j = 0; j < G.vexNum; ++j)
        {
            if (G.arc[i][j] == INF) std::cout << "   ∞";
            else std::cout << (G.arc[i][j] < 10 ? "    " : (G.arc[i][j] < 100 ? "   " : "  "))
                           << G.arc[i][j];
        }
        std::cout << '\n';
    }
}

// 从未加入最短路树的顶点中，选 dist 最小的那个
// 找不到则返回 -1
int MinDistance(const MGraph& G, const EdgeType dist[], const bool visited[])
{
    EdgeType best = INF;
    int idx = -1;

    for (int v = 0; v < G.vexNum; ++v)
    {
        if (!visited[v] && dist[v] < best)
        {
            best = dist[v];
            idx = v;
        }
    }
    return idx;
}

// 打印 src -> v 的路径（递归从 parent 链反推）
// 若不可达，不应调用
void PrintPath(const int parent[], int v)
{
    if (parent[v] == -1) return;
    PrintPath(parent, parent[v]);
    std::cout << " -> " << v;
}

// Dijkstra：邻接矩阵版，O(V^2)
// 注意：边权必须非负
void Dijkstra(const MGraph& G, int src)
{
    EdgeType dist[MAXVEX];
    bool visited[MAXVEX];
    int parent[MAXVEX];

    for (int i = 0; i < G.vexNum; ++i)
    {
        dist[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }
    dist[src] = 0;

    for (int step = 0; step < G.vexNum; ++step)
    {
        int u = MinDistance(G, dist, visited);

        // ? 图不连通：剩下所有点都是 INF，提前结束
        if (u == -1 || dist[u] == INF) break;

        visited[u] = true;

        // 松弛 u 的所有出边 u->v
        for (int v = 0; v < G.vexNum; ++v)
        {
            if (!visited[v] &&
                G.arc[u][v] != INF &&
                dist[u] + G.arc[u][v] < dist[v])
            {
                dist[v] = dist[u] + G.arc[u][v];
                parent[v] = u;
            }
        }
    }

    std::cout << "Vertex\tDistance\tPath\n";
    for (int v = 0; v < G.vexNum; ++v)
    {
        std::cout << v << "\t";

        if (dist[v] == INF)
        {
            std::cout << "INF\t\t" << src << " (unreachable)\n";
            continue;
        }

        std::cout << dist[v] << "\t\t" << src;
        PrintPath(parent, v);
        std::cout << '\n';
    }
}

int main()
{
    MGraph G;
    G.vexNum = 6;
    G.edgeNum = 10;

    G.vexs[0] = 'a';
    G.vexs[1] = 'b';
    G.vexs[2] = 'c';
    G.vexs[3] = 'd';
    G.vexs[4] = 'e';
    G.vexs[5] = 'f';

    InitGraph(G);

    // 无向边：对称赋值（若要有向图，只赋一边即可）
    G.arc[0][1] = G.arc[1][0] = 6;
    G.arc[0][3] = G.arc[3][0] = 5;
    G.arc[0][2] = G.arc[2][0] = 1;
    G.arc[1][2] = G.arc[2][1] = 5;
    G.arc[2][3] = G.arc[3][2] = 5;
    G.arc[1][4] = G.arc[4][1] = 3;
    G.arc[2][4] = G.arc[4][2] = 6;
    G.arc[2][5] = G.arc[5][2] = 4;
    G.arc[3][5] = G.arc[5][3] = 2;
    G.arc[4][5] = G.arc[5][4] = 6;

    Graph_Show(G);
    Dijkstra(G, 0);
    return 0;
}