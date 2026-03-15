#include <iostream>
#include <cstdlib> // std::abs (int 版本)

int main() {
    constexpr int n = 5;

    // 三对角矩阵（'0' 表示非三对角元素）
    char A[n][n] = {
        {'a','c','0','0','0'},
        {'b','a','c','0','0'},
        {'0','b','a','c','0'},
        {'0','0','b','a','c'},
        {'0','0','0','b','a'}
    };

    // 压缩存储：三条对角线共 3n - 2 个元素
    char B[3 * n - 2];

    // 0 基：当 |i - j| <= 1 时，索引映射 k = 2*i + j
    auto idx = [](int i, int j) -> int { return 2 * i + j; };

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (std::abs(i - j) <= 1) {
                int k = idx(i, j);
                B[k] = A[i][j];
            }
        }
    }

    std::cout << "压缩后的矩阵B：\n";
    for (int i = 0; i < 3 * n - 2; ++i) {
        std::cout << B[i] << (i + 1 == 3 * n - 2 ? '\n' : ' ');
    }
    return 0;
}
