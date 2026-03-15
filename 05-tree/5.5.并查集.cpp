#include <iostream>
#include <string>
#include <string_view>

class UnionFind {
private:
    int* parent{nullptr}; // 存储父节点
    int* sz{nullptr};     // 存储树的大小 (Size)
    int count{};   // 节点总数

public:
    // 构造函数：初始化并查集
    // Explicit 禁止隐式转换，符合现代 C++ 规范
    explicit UnionFind(int n) : count(n) {
        parent = new int[n];
        sz = new int[n];
        for (int i = 0; i < n; ++i) {
            parent[i] = i; // 初始时，每个节点的父节点是自己
            sz[i] = 1;     // 初始时，每棵树的大小为 1
        }
    }

    // 析构函数：释放手动分配的内存 (RAII)
    ~UnionFind() {
        delete[] parent;
        delete[] sz;
    }

    // 禁用拷贝构造和拷贝赋值，防止双重释放 (Rule of Five)
    UnionFind(const UnionFind&) = delete;
    UnionFind& operator=(const UnionFind&) = delete;

    // 查找函数 (Find)
    // 包含：路径压缩 (Path Compression)
    // [[nodiscard]] 提醒调用者不要忽略返回值
    [[nodiscard]] int find(int i) {
        if (i < 0 || i >= count) return -1; // 简单的边界检查

        // 路径压缩：在递归查找的过程中，直接将当前节点指向根节点
        if (parent[i] != i) {
            parent[i] = find(parent[i]);
        }
        return parent[i];
    }

    // 合并函数 (Union)
    // 包含：按大小合并 (Union by Size / Small to Large)
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);

        if (root_i != root_j) {
            // 小树并入大树逻辑
            // 如果 i 所在的树比 j 所在的树大，则交换它们，确保 i 总是较小的那个
            // 这样我们只需要写一种合并逻辑：把 i 并入 j
            if (sz[root_i] > sz[root_j]) {
                // 手动 swap，因为不能用 <algorithm>
                int temp = root_i;
                root_i = root_j;
                root_j = temp;
            }

            // 此时 root_i 代表较小的树，root_j 代表较大的树
            parent[root_i] = root_j; // 小树指向大树
            sz[root_j] += sz[root_i]; // 更新大树的大小
        }
    }

    // 判断两个节点是否连通
    [[nodiscard]] bool connected(int i, int j) {
        return find(i) == find(j);
    }

    // 获取某个节点所在树的大小
    [[nodiscard]] int get_size(int i) {
        int root = find(i);
        if (root == -1) return 0;
        return sz[root];
    }
};

// 辅助函数：打印测试结果
void print_status(std::string_view msg, bool result) {
    std::cout << msg << ": " << (result ? "True" : "False") << "\n";
}

int main() {
    // 创建一个包含 10 个元素 (0-9) 的并查集
    const int N = 10;
    UnionFind uf(N);

    std::cout << "--- 初始化并查集 (0-" << N - 1 << ") ---\n";

    // 合并操作
    uf.unite(1, 2);
    uf.unite(3, 4);
    uf.unite(2, 3); // 此时 1, 2, 3, 4 应该连通

    uf.unite(6, 7);
    uf.unite(8, 9);
    uf.unite(7, 8); // 此时 6, 7, 8, 9 应该连通

    // 查找与验证
    print_status("1 和 4 是否连通", uf.connected(1, 4));
    print_status("1 和 6 是否连通", uf.connected(1, 6));
    
    std::cout << "1 所在树的大小: " << uf.get_size(1) << "\n"; // 应该是 4
    std::cout << "6 所在树的大小: " << uf.get_size(6) << "\n"; // 应该是 4
    std::cout << "0 所在树的大小: " << uf.get_size(0) << "\n"; // 应该是 1 (未合并)

    // 将两个大集合合并
    uf.unite(1, 6);
    std::cout << "--- 合并 (1,2,3,4) 和 (6,7,8,9) 后 ---\n";
    
    print_status("1 和 9 是否连通", uf.connected(1, 9));
    std::cout << "1 所在树的大小: " << uf.get_size(1) << "\n"; // 应该是 8

    return 0;
}