#include <iostream>

using ElemType = int;

// ===== 桶中链表结点 =====
struct Node {
    ElemType data{};
    Node* next{nullptr};
};

// ===== 插入排序：链表有序插入 =====
void InsertSorted(Node*& head, ElemType x)
{
    Node* node = new Node{x, nullptr};

    // 插入到表头
    if (!head || x < head->data) {
        node->next = head;
        head = node;
        return;
    }

    // 找插入位置
    Node* cur = head;
    while (cur->next && cur->next->data <= x) {
        cur = cur->next;
    }
    node->next = cur->next;
    cur->next = node;
}

// ===== 桶排序 =====
void BucketSort(ElemType a[], int n, int bucketCount)
{
    if (n <= 1) return;

    // 1?? 求最大值和最小值
    int minVal = a[0], maxVal = a[0];
    for (int i = 1; i < n; ++i) {
        if (a[i] < minVal) minVal = a[i];
        if (a[i] > maxVal) maxVal = a[i];
    }

    // 2?? 创建桶（链表）
    Node** buckets = new Node*[bucketCount];
    for (int i = 0; i < bucketCount; ++i) {
        buckets[i] = nullptr;
    }

    // 3?? 分配元素到桶
    int range = maxVal - minVal + 1;
    for (int i = 0; i < n; ++i) {
        int index = (a[i] - minVal) * bucketCount / range;
        InsertSorted(buckets[index], a[i]);
    }

    // 4?? 收集桶中元素
    int idx = 0;
    for (int i = 0; i < bucketCount; ++i) {
        Node* cur = buckets[i];
        while (cur) {
            a[idx++] = cur->data;
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
    }

    delete[] buckets;
}

// ===== 测试 =====
int main()
{
    ElemType a[] = {42, 32, 33, 52, 37, 47, 51};
    int n = sizeof(a) / sizeof(a[0]);

    BucketSort(a, n, 5);

    std::cout << "桶排序结果: ";
    for (int i = 0; i < n; ++i) {
        std::cout << a[i] << " ";
    }
    std::cout << "\n";

    return 0;
}