#include <iostream>

using ElemType = int;

// 合并：把 A[low..mid] 与 A[mid+1..high] 两段有序区间合并回 A
void Merge(ElemType A[], ElemType B[], int low, int mid, int high) noexcept
{
    // 1) 复制到辅助数组 B
    for (int k = low; k <= high; ++k) {
        B[k] = A[k];
    }

    // 2) 双指针归并
    int i = low;       // 左段指针
    int j = mid + 1;   // 右段指针
    int k = low;       // 写回 A 的位置

    while (i <= mid && j <= high) {
        if (B[i] <= B[j]) {          // <= 保证稳定性
            A[k++] = B[i++];
        } else {
            A[k++] = B[j++];
        }
    }

    // 3) 把剩余部分拷回去（右边剩余其实也可以不拷：因为原本就在 A 里，但为了教材一致性保留）
    while (i <= mid)  A[k++] = B[i++];
    while (j <= high) A[k++] = B[j++];
}

void MergeSort(ElemType A[], ElemType B[], int low, int high) noexcept
{
    if (low >= high) return;

    const int mid = low + (high - low) / 2; // 防止 low+high 溢出（习惯写法）

    MergeSort(A, B, low, mid);
    MergeSort(A, B, mid + 1, high);
    Merge(A, B, low, mid, high);
}

int main()
{
    constexpr int n = 10;
    ElemType arr[n] = {49, 38, 65, 97, 76, 13, 27, 49, 55, 40};

    // 辅助数组：由 main 统一管理（不全局、不 malloc）
    ElemType buf[n]{};

    MergeSort(arr, buf, 0, n - 1);

    std::cout << "归并排序结果: ";
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << '\n';

    return 0;
}