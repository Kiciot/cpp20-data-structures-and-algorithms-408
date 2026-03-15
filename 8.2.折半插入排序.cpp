#include <iostream>

using ElemType = int;

// 折半插入排序（Binary Insertion Sort）
// A[0] 为哨兵（暂存区），A[1..n] 为待排序元素
void BinaryInsertSort(ElemType A[], int n) noexcept
{
    for (int i = 2; i <= n; ++i) {
        // 将待插入元素暂存到哨兵
        A[0] = A[i];

        int low  = 1;
        int high = i - 1;

        // 在 A[1..i-1] 中用二分查找插入位置
        while (low <= high) {
            int mid = (low + high) / 2;
            if (A[mid] > A[0]) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        // 统一后移元素，给 A[0] 腾位置
        for (int j = i - 1; j >= high + 1; --j) {
            A[j + 1] = A[j];
        }

        // 插入到最终位置
        A[high + 1] = A[0];
    }
}

int main()
{
    // A[0] 是哨兵，不参与排序
    ElemType arr[10] = {-1, 20, 60, 30, 10, 40, 90, 80, 70, 50};

    BinaryInsertSort(arr, 9);

    std::cout << "折半插入排序结果：";
    for (int i = 1; i <= 9; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}