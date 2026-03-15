#include <iostream>

using VertexType = char;
using EdgeType   = int;

constexpr int MAXVEX = 100;
constexpr EdgeType INF = 65535;
constexpr bool DIRECTED = true;

constexpr int V = 3; // 顶点个数（为了方便固定数组大小）

struct MGraph
{
    VertexType vexs[MAXVEX]{};
    EdgeType arc[MAXVEX][MAXVEX]{};
    int vexNum{};
    int edgeNum{};
};

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

// 递归打印路径：start -> ... -> end
// next[start][end] 表示从 start 到 end 的“下一跳”顶点（更直观，也更不容易错）
void PrintPath(const int next[V][V], int start, int end)
{
    if (next[start][end] == -1)
    {
        std::cout << "No Path";
        return;
    }

    std::cout << start;
    int cur = start;
    while (cur != end)
    {
        cur = next[cur][end];
        std::cout << " -> " << cur;
    }
}

// Floyd-Warshall：O(V^3)
// dist: 最短路长度
// next: 路径恢复（下一跳矩阵）
void Floyd(const MGraph& G)
{
    EdgeType dist[V][V];
    int next[V][V];

    // 初始化 dist 和 next
    for (int i = 0; i < V; ++i)
    {
        for (int j = 0; j < V; ++j)
        {
            dist[i][j] = G.arc[i][j];

            if (i == j)
            {
                dist[i][j] = 0;      // 自己到自己 0
                next[i][j] = j;      // 路径就是自己
            }
            else if (G.arc[i][j] != INF)
            {
                next[i][j] = j;      // i 到 j 的下一跳就是 j（直达）
            }
            else
            {
                next[i][j] = -1;     // 不可达
            }
        }
    }

    // 三重循环更新最短路
    for (int k = 0; k < V; ++k)
    {
        for (int i = 0; i < V; ++i)
        {
            for (int j = 0; j < V; ++j)
            {
                // ? 防 INF 乱加（很关键）
                if (dist[i][k] == INF || dist[k][j] == INF) continue;

                EdgeType cand = dist[i][k] + dist[k][j];
                if (cand < dist[i][j])
                {
                    dist[i][j] = cand;
                    next[i][j] = next[i][k]; // i->j 的第一步 = i->k 的第一步
                }
            }
        }
    }

    // 输出最短路矩阵
    std::cout << "最短路径矩阵：\n";
    for (int i = 0; i < V; ++i)
    {
        for (int j = 0; j < V; ++j)
        {
            if (dist[i][j] == INF) std::cout << "INF ";
            else std::cout << dist[i][j] << "  ";
        }
        std::cout << '\n';
    }

    // 输出路径
    std::cout << "\n最短路径信息：\n";
    for (int i = 0; i < V; ++i)
    {
        for (int j = 0; j < V; ++j)
        {
            std::cout << "Path from " << i << " to " << j << ": ";
            PrintPath(next, i, j);
            std::cout << '\n';
        }
    }
}

int main()
{
    MGraph G;
    G.vexNum = V;
    G.edgeNum = 5;

    G.vexs[0] = '0';
    G.vexs[1] = '1';
    G.vexs[2] = '2';

    // 初始化为 INF
    for (int i = 0; i < G.vexNum; ++i)
        for (int j = 0; j < G.vexNum; ++j)
            G.arc[i][j] = INF;

    // 设边（有向图）
    G.arc[0][1] = 6;
    G.arc[0][2] = 13;
    G.arc[1][0] = 10;
    G.arc[1][2] = 4;
    G.arc[2][0] = 5;

    Graph_Show(G);
    Floyd(G);

    return 0;
}