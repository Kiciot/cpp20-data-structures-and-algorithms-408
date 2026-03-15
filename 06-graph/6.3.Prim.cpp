#include <iostream>
#include <limits>

using VertexType = char;
using EdgeType   = int;

constexpr int MAXVEX = 100;
constexpr EdgeType INF = 65535; // 不可达
constexpr bool DIRECTED = false; // Prim 用无向图更符合定义

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
        {
            G.arc[i][j] = INF;
        }
        G.arc[i][i] = 0; // 自己到自己距离为 0（对 Prim 非必需，但更规范）
    }
}

int Find_Min_V(const MGraph& G, const EdgeType cost[], const bool visited[])
{
    EdgeType minCost = INF;
    int minVertex = -1;
    for (int v = 0; v < G.vexNum; ++v)
    {
        if (!visited[v] && cost[v] < minCost)
        {
            minCost = cost[v];
            minVertex = v;
        }
    }
    return minVertex;
}

// 从 start 开始构建 MST，输出 parent/cost
void Prim(const MGraph& G, int start)
{
    int parent[MAXVEX];
    EdgeType cost[MAXVEX];
    bool visited[MAXVEX];

    for (int i = 0; i < G.vexNum; ++i)
    {
        parent[i] = -1;
        cost[i] = INF;
        visited[i] = false;
    }

    cost[start] = 0;

    // 每次把一个“距离 MST 最近”的顶点加入 MST
    for (int step = 0; step < G.vexNum; ++step)
    {
        int u = Find_Min_V(G, cost, visited);
        if (u == -1) break;          // 图不连通：剩下的点到 MST 都是 INF
        visited[u] = true;

        // 用 u 去松弛其它顶点：尝试更新 cost[v] = min(cost[v], w(u,v))
        for (int v = 0; v < G.vexNum; ++v)
        {
            // ? 必须判断有边：w(u,v) != INF（而不是 if(w)）
            if (!visited[v] && G.arc[u][v] != INF && G.arc[u][v] < cost[v])
            {
                cost[v] = G.arc[u][v];
                parent[v] = u;
            }
        }
    }

    // 输出 MST（如果不连通，会有顶点 cost 仍为 INF）
    std::cout << "Vertices in the MST (or forest if disconnected):\n";
    for (int v = 0; v < G.vexNum; ++v)
    {
        std::cout << "v=" << v
                  << " p=" << parent[v]
                  << " c=" << cost[v] << '\n';
    }

    // 如果你要算 MST 总权值（仅连通图有效）
    long long total = 0;
    for (int v = 0; v < G.vexNum; ++v)
    {
        if (cost[v] != INF) total += cost[v];
    }
    std::cout << "Total weight (sum of chosen costs, INF ignored) = " << total << '\n';
}

void Graph_Show(const MGraph& G)
{
    for (int i = 0; i < G.vexNum; ++i) std::cout << G.vexs[i];
    std::cout << "\n";

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

    // 无向边：对称赋值
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
    Prim(G, 0);
    return 0;
}