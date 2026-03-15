#include <iostream>

using ElemType = int;

// 版本1：自动增量（gap = n/2, n/4, ..., 1）
// A[0] 为暂存单元，A[1..n] 为待排序数据
void ShellSort_AutoGap(ElemType A[], int n) noexcept
{
    for (int gap = n / 2; gap >= 1; gap /= 2) {
        // 对每个 gap 做一趟“分组插入排序”
        for (int i = gap + 1; i <= n; ++i) {
            if (A[i] < A[i - gap]) {
                A[0] = A[i]; // 暂存
                int j = i - gap;
                while (j > 0 && A[0] < A[j]) {
                    A[j + gap] = A[j];
                    j -= gap;
                }
                A[j + gap] = A[0];
            }
        }
    }
}

// 版本2：指定增量序列 gaps[]（要求最后一个 gap 必须为 1）
// gaps: 外部传入的增量数组（不使用 vector）
// gapCount: gaps 的长度
void ShellSort_CustomGaps(ElemType A[], int n, const int gaps[], int gapCount) noexcept
{
    for (int idx = 0; idx < gapCount; ++idx) {
        const int gap = gaps[idx];
        if (gap <= 0) continue;

        for (int i = gap + 1; i <= n; ++i) {
            if (A[i] < A[i - gap]) {
                A[0] = A[i];
                int j = i - gap;
                while (j > 0 && A[0] < A[j]) {
                    A[j + gap] = A[j];
                    j -= gap;
                }
                A[j + gap] = A[0];
            }
        }
    }
}

static void PrintArray(const ElemType A[], int n)
{
    for (int i = 1; i <= n; ++i) {
        std::cout << A[i] << ' ';
    }
    std::cout << '\n';
}

int main()
{
    // A[0] 为暂存单元，不参与排序
    ElemType arr[10]  = {-1, 20, 60, 30, 10, 40, 90, 80, 70, 50};
    ElemType arr2[10] = {-1, 20, 60, 30, 10, 40, 90, 80, 70, 50};

    ShellSort_AutoGap(arr, 9);
    std::cout << "希尔排序（自动增量）结果：";
    PrintArray(arr, 9);

    const int gaps[3] = {3, 2, 1}; // 最后必须是 1
    ShellSort_CustomGaps(arr2, 9, gaps, 3);
    std::cout << "希尔排序（给定增量）结果：";
    PrintArray(arr2, 9);

    return 0;
}