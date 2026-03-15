#include <iostream>

using ElemType = int;

struct Node {
    ElemType data{};
    Node* next{nullptr};
};

struct Bucket {
    Node* head{nullptr};
    Node* tail{nullptr};
};

// 求最大位数（十进制）
int GetMaxDigits(const ElemType a[], int n) noexcept
{
    int maxNum = a[0];
    for (int i = 1; i < n; ++i) {
        if (a[i] > maxNum) maxNum = a[i];
    }

    int digits = 0;
    do {                 // maxNum==0 时也应返回 1 位
        maxNum /= 10;
        ++digits;
    } while (maxNum > 0);

    return digits;
}

// 桶尾插：O(1)
void BucketPush(Bucket& b, ElemType x)
{
    Node* node = new Node{x, nullptr};
    if (!b.head) {
        b.head = b.tail = node;
    } else {
        b.tail->next = node;
        b.tail = node;
    }
}

// 把桶里的元素按链表顺序取出写回数组，并释放节点
void BucketDrain(Bucket& b, ElemType a[], int& idx) noexcept
{
    Node* cur = b.head;
    while (cur) {
        a[idx++] = cur->data;
        Node* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    b.head = b.tail = nullptr;
}

// LSD 基数排序（默认：非负整数）
void RadixSort(ElemType a[], int n)
{
    if (n <= 1) return;

    const int maxDigits = GetMaxDigits(a, n);

    Bucket buckets[10]{};

    int exp = 1; // 1, 10, 100, ...
    for (int d = 1; d <= maxDigits; ++d, exp *= 10) {

        // 分配：按当前位入桶
        for (int i = 0; i < n; ++i) {
            const int digit = (a[i] / exp) % 10;
            BucketPush(buckets[digit], a[i]);
        }

        // 收集：从 0..9 依次出桶写回
        int idx = 0;
        for (int k = 0; k < 10; ++k) {
            BucketDrain(buckets[k], a, idx);
        }
    }
}

int main()
{
    ElemType arr[] = {170, 45, 75, 90, 802, 24, 2, 66};
    const int n = static_cast<int>(sizeof(arr) / sizeof(arr[0]));

    RadixSort(arr, n);

    std::cout << "基数排序结果: ";
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << '\n';
    return 0;
}