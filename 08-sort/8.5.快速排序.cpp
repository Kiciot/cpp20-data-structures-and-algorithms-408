#include <iostream>

using ElemType = int;

// 交换：不依赖 <utility>/<algorithm>
inline void swap_elem(ElemType& a, ElemType& b) noexcept {
    const ElemType t = a;
    a = b;
    b = t;
}

// Hoare 风格“挖坑填数”分区：以 A[low] 为枢轴
// 返回枢轴最终落点下标 pos，使得：A[low..pos-1] <= pivot，A[pos+1..high] >= pivot
int Partition(ElemType A[], int low, int high) noexcept
{
    const ElemType pivot = A[low]; // 枢轴备份（坑位在 low）
    while (low < high) {
        // 从右往左找 < pivot 的，填到左坑
        while (low < high && A[high] >= pivot) --high;
        A[low] = A[high];

        // 从左往右找 > pivot 的，填到右坑
        while (low < high && A[low] <= pivot) ++low;
        A[high] = A[low];
    }
    A[low] = pivot; // 枢轴填回最终坑位
    return low;
}

// 快排（递归版）
// 注意：递归边界写成 if (low >= high) return; 更直观，且避免 pivotPos-1 变成 -1 还继续算的误读
void QuickSort(ElemType A[], int low, int high) noexcept
{
    if (low >= high) return;

    const int pivotPos = Partition(A, low, high);

    // 左半边
    if (pivotPos - 1 > low) QuickSort(A, low, pivotPos - 1);
    else if (pivotPos - 1 == low) {/*单元素无需排*/}

    // 右半边
    if (pivotPos + 1 < high) QuickSort(A, pivotPos + 1, high);
    else if (pivotPos + 1 == high) {/*单元素无需排*/}
}

//小区间递归，大区间迭代

static void PrintArray(const ElemType A[], int n)
{
    for (int i = 0; i < n; ++i) std::cout << A[i] << ' ';
    std::cout << '\n';
}

int main()
{
    ElemType arr[9] = {20, 60, 30, 10, 40, 90, 80, 70, 50};

    QuickSort(arr, 0, 9 - 1);

    std::cout << "快速排序结果：";
    PrintArray(arr, 9);
    return 0;
}