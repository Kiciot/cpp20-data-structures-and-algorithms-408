#include <iostream>
#include <string>
#include <string_view>

// 使用 constexpr 替代宏定义
constexpr int MAXVEX = 100;    // 最大顶点数
constexpr int INF = 65535;     // 无穷大
constexpr bool DIRECTED = true; // 是否为有向图

// 类型别名
using VertexType = std::string; 
using EdgeType = int;

struct MGraph {
    VertexType vexs[MAXVEX];      // 顶点表
    EdgeType arc[MAXVEX][MAXVEX]; // 邻接矩阵
    int vexNum = 0;               // 顶点数
    int edgeNum = 0;              // 边数

    // 构造函数：初始化
    MGraph() {
        for (int i = 0; i < MAXVEX; ++i) {
            for (int j = 0; j < MAXVEX; ++j) {
                arc[i][j] = INF;
            }
        }
    }
};

// 创建邻接矩阵
void Graph_Create(MGraph& G) {
    std::cout << "请输入顶点数：";
    std::cin >> G.vexNum;
    std::cout << "请输入边数：";
    std::cin >> G.edgeNum;

    if (G.vexNum > MAXVEX) {
        std::cerr << "错误：顶点数超过最大限制\n";
        return;
    }

    // 输入顶点信息
    for (int i = 0; i < G.vexNum; i++) {
        std::cout << "请输入第 " << (i + 1) << " 个的顶点名称: ";
        std::cin >> G.vexs[i];
    }

    std::cout << "下标为 -1 -1 则退出创建边\n";
    
    // 输入边信息
    for (int i = 0; i < G.edgeNum; i++) {
        int vi, vj;
        EdgeType weight;

        std::cout << "请输入要创建边的下标 (v1 v2): ";
        std::cin >> vi >> vj;

        if (vi == -1 && vj == -1) break;

        if (vi < 0 || vi >= G.vexNum || vj < 0 || vj >= G.vexNum) {
            std::cout << "下标越界，请重新输入！\n";
            i--; 
            continue;
        }

        std::cout << "请输入边 [" << vi << "][" << vj << "] 的权值 weight = ";
        std::cin >> weight;

        G.arc[vi][vj] = weight;

        if (!DIRECTED) {
            G.arc[vj][vi] = weight;
        }
    }
}

// 打印图
void Graph_Show(const MGraph& G) {
    // 定义列宽
    constexpr int WIDTH = 8;

    // 手写一个 Lambda 来辅助打印对齐
    // 参数 s: 要打印的内容
    auto print_cell = [](std::string_view s) {
        int len = static_cast<int>(s.length());
        // 如果内容长度小于宽度，先打印空格（右对齐）
        // 如果想左对齐，就把打印空格放在打印内容后面
        if (len < WIDTH) {
            std::cout << std::string(WIDTH - len, ' ');
        }
        std::cout << s;
    };

    std::cout << "\n--- 邻接矩阵信息 ---\n";
    
    // 1. 打印表头（第一行空的 + 顶点名）
    print_cell(" "); // 左上角占位
    for (int i = 0; i < G.vexNum; i++) {
        print_cell(G.vexs[i]);
    }
    std::cout << "\n";

    // 2. 打印矩阵内容
    for (int i = 0; i < G.vexNum; i++) {
        // 打印行首顶点名
        print_cell(G.vexs[i]);
        
        for (int j = 0; j < G.vexNum; j++) {
            if (G.arc[i][j] == INF) {
                print_cell("∞");
            } else {
                // 将 int 转为 string 方便统一处理长度
                print_cell(std::to_string(G.arc[i][j]));
            }
        }
        std::cout << "\n";
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    MGraph G; 
    Graph_Create(G);
    Graph_Show(G);

    return 0;
}