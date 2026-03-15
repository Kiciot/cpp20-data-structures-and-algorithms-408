#include <iostream>

using ElemType = int;

// A[0] 是哨兵，A[1..n] 是待排序元素
void SentinelInsertSort(ElemType A[], int n) noexcept
{
    for (int i = 2; i <= n; ++i) {
        if (A[i] < A[i - 1]) {
            A[0] = A[i];      // 设置哨兵
            int j = i - 1;

            while (A[0] < A[j]) {
                A[j + 1] = A[j];
                --j;
            }
            A[j + 1] = A[0];
        }
    }
}

void InsertSort(ElemType A[], int n) noexcept
{
    for (int i = 1; i < n; ++i) {
        if (A[i] < A[i - 1]) {
            ElemType temp = A[i];
            int j = i - 1;

            while (j >= 0 && temp < A[j]) {
                A[j + 1] = A[j];
                --j;
            }
            A[j + 1] = temp;
        }
    }
}

int main()
{
    // ===== 哨兵版 =====
    // A[0] 是哨兵，不参与排序
    ElemType sentinelArr[10] = {-1, 20, 60, 30, 10, 40, 90, 80, 70, 50};
    SentinelInsertSort(sentinelArr, 9);

    std::cout << "哨兵模式排序结果：";
    for (int i = 1; i <= 9; ++i) {
        std::cout << sentinelArr[i] << " ";
    }

    // ===== 普通版 =====
    ElemType normalArr[9] = {20, 60, 30, 10, 40, 90, 80, 70, 50};
    InsertSort(normalArr, 9);

    std::cout << "\n普通模式排序结果：";
    for (int i = 0; i < 9; ++i) {
        std::cout << normalArr[i] << " ";
    }

    std::cout << '\n';
    return 0;
}