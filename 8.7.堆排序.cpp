#include <iostream>

using ElemType = int;

// 将以 k 为根的子树调整为最大堆（1-based，下标范围 [1..len]）
// A[0] 作为暂存单元（哨兵/临时变量）
void HeapAdjust(ElemType A[], int k, int len) noexcept
{
    A[0] = A[k]; // 暂存根

    for (int i = 2 * k; i <= len; i *= 2) {
        // 选更大的孩子
        if (i < len && A[i] < A[i + 1]) ++i;

        // 根已经比最大孩子大，不用再下滤
        if (A[0] >= A[i]) break;

        // 孩子上移，继续往下走
        A[k] = A[i];
        k = i;
    }

    A[k] = A[0]; // 放回最终位置
}

// 堆排序：升序（先建最大堆，再把堆顶依次换到末尾）
void HeapSort(ElemType A[], int len) noexcept
{
    // 1) 建堆：从最后一个非叶子结点开始下滤
    for (int i = len / 2; i >= 1; --i) {
        HeapAdjust(A, i, len);
    }

    // 2) 交换堆顶与末尾，缩小堆，再下滤
    for (int i = len; i > 1; --i) {
        // 正确交换：A[1] <-> A[i]
        const ElemType tmp = A[1];
        A[1] = A[i];
        A[i] = tmp;

        HeapAdjust(A, 1, i - 1);
    }
}

static void Print1Based(const ElemType A[], int len)
{
    for (int i = 1; i <= len; ++i) {
        std::cout << A[i] << ' ';
    }
    std::cout << '\n';
}

int main()
{
    // 1-based 堆：A[0] 是暂存单元，不是有效数据
    ElemType A[9] = {0, 87, 45, 78, 32, 17, 65, 53, 9};
    constexpr int len = 8;

    HeapSort(A, len);

    std::cout << "堆排序结果: ";
    Print1Based(A, len);
    return 0;
}