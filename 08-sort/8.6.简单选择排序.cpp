#include <iostream>

using ElemType = int;

// 简单选择排序：每一趟从 [i, n) 中选最小元素，放到位置 i
void SelectSort(ElemType A[], int n) noexcept
{
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;

        // 在未排序区间中寻找最小值
        for (int j = i + 1; j < n; ++j) {
            if (A[j] < A[minIndex]) {
                minIndex = j;
            }
        }

        // 若找到更小元素，则交换
        if (minIndex != i) {
            const ElemType tmp = A[i];
            A[i] = A[minIndex];
            A[minIndex] = tmp;
        }
    }
}

int main()
{
    ElemType arr[9] = {20, 60, 30, 10, 40, 90, 80, 70, 50};

    SelectSort(arr, 9);

    std::cout << "简单选择排序结果: ";
    for (int i = 0; i < 9; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}