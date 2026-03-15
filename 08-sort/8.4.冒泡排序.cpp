#include <iostream>

using ElemType = int;

// 冒泡：把最小值往前“冒”（从后往前扫）
// 稳定；最好 O(n)，最坏/平均 O(n^2)，额外空间 O(1)
void Bubble_MinToFront(ElemType a[], int n) noexcept
{
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = n - 1; j > i; --j) {
            if (a[j - 1] > a[j]) {
                const ElemType tmp = a[j];
                a[j] = a[j - 1];
                a[j - 1] = tmp;
                swapped = true;
            }
        }
        if (!swapped) return; // 本趟无交换，已整体有序
    }
}

// 冒泡：把最大值往后“冒”（从前往后扫）
// 稳定；最好 O(n)，最坏/平均 O(n^2)，额外空间 O(1)
void Bubble_MaxToEnd(ElemType a[], int n) noexcept
{
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; ++j) {
            if (a[j] > a[j + 1]) {
                const ElemType tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
                swapped = true;
            }
        }
        if (!swapped) return;
    }
}

static void PrintArray(const ElemType a[], int n)
{
    for (int i = 0; i < n; ++i) std::cout << a[i] << ' ';
    std::cout << '\n';
}

int main()
{
    ElemType arr1[9] = {20, 60, 30, 10, 40, 90, 80, 70, 50};
    Bubble_MinToFront(arr1, 9);
    std::cout << "冒泡排序(最小的往前冒)：";
    PrintArray(arr1, 9);

    ElemType arr2[9] = {20, 60, 30, 10, 40, 90, 80, 70, 50};
    Bubble_MaxToEnd(arr2, 9);
    std::cout << "冒泡排序(最大的往后冒)：";
    PrintArray(arr2, 9);

    return 0;
}