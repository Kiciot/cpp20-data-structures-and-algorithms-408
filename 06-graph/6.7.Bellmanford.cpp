#include <iostream>

using VertexType = char;
using EdgeType   = int;

constexpr int MAXVEX = 100;
constexpr EdgeType INF = 65535;
constexpr bool DIRECTED = true;

struct MGraph
{
    VertexType vexs[MAXVEX]{};
    EdgeType arc[MAXVEX][MAXVEX]{};
    int vexNum{};
    int edgeNum{};
};

void Graph_Show(const MGraph& G)
{
    for (int i = 0; i < G.vexNum; ++i)
        std::cout << G.vexs[i];
    std::cout << '\n';

    for (int i = 0; i < G.vexNum; ++i)
    {
        for (int j = 0; j < G.vexNum; ++j)
        {
            if (G.arc[i][j] == INF) std::cout << "   ∞";
            else std::cout << (G.arc[i][j] < 10 ? "    " : "   ")
                           << G.arc[i][j];
        }
        std::cout << '\n';
    }
}

// 递归打印路径：src -> ... -> v
void PrintPath(const MGraph& G, const int parent[], int src, int v)
{
    if (v == -1) return;
    if (v == src)
    {
        std::cout << G.vexs[src];
        return;
    }
    PrintPath(G, parent, src, parent[v]);
    std::cout << " -> " << G.vexs[v];
}

// Bellman–Ford：邻接矩阵版
void BellmanFord(const MGraph& G, int src)
{
    EdgeType dist[MAXVEX];
    int parent[MAXVEX];

    for (int i = 0; i < G.vexNum; ++i)
    {
        dist[i] = INF;
        parent[i] = -1;
    }
    dist[src] = 0;

    // 松弛 V-1 次
    for (int round = 1; round < G.vexNum; ++round)
    {
        bool changed = false;

        for (int u = 0; u < G.vexNum; ++u)
        {
            for (int v = 0; v < G.vexNum; ++v)
            {
                EdgeType w = G.arc[u][v];
                if (w != INF &&
                    dist[u] != INF &&        // 防 INF + w 溢出
                    dist[u] + w < dist[v])
                {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    changed = true;
                }
            }
        }

        // 若一整轮无变化，提前结束
        if (!changed) break;
    }

    // 负权环检测
    for (int u = 0; u < G.vexNum; ++u)
    {
        for (int v = 0; v < G.vexNum; ++v)
        {
            EdgeType w = G.arc[u][v];
            if (w != INF &&
                dist[u] != INF &&
                dist[u] + w < dist[v])
            {
                std::cout << "图中存在负权环，无法求最短路径\n";
                return;
            }
        }
    }

    // 输出结果
    for (int v = 0; v < G.vexNum; ++v)
    {
        if (v == src) continue;

        std::cout << "从 " << G.vexs[src]
                  << " 到 " << G.vexs[v]
                  << " 的最短距离 = ";

        if (dist[v] == INF)
        {
            std::cout << "INF（不可达）\n";
            continue;
        }

        std::cout << dist[v] << "，路径：";
        PrintPath(G, parent, src, v);
        std::cout << '\n';
    }
}

int main()
{
    MGraph G;
    G.vexNum = 5;
    G.edgeNum = 8;

    G.vexs[0] = 'A';
    G.vexs[1] = 'B';
    G.vexs[2] = 'C';
    G.vexs[3] = 'D';
    G.vexs[4] = 'E';

    for (int i = 0; i < G.vexNum; ++i)
        for (int j = 0; j < G.vexNum; ++j)
            G.arc[i][j] = INF;

    // 有向图 + 负权边
    G.arc[0][1] = -1;
    G.arc[0][2] = 4;
    G.arc[1][2] = 3;
    G.arc[1][3] = 2;
    G.arc[1][4] = 2;
    G.arc[3][2] = 5;
    G.arc[3][1] = 1;
    G.arc[4][3] = -3;

    Graph_Show(G);
    BellmanFord(G, 0);
    return 0;
}