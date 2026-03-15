#include <iostream>
#include <span>     // C++20

#define MAX_SIZE 100

struct SqList {
    int data[MAX_SIZE];
    int length;
};

void InitList(SqList& L) { L.length = 0; }

bool ListInsert(SqList& L, int i, int e) {
    if (i < 1 || i > L.length + 1) return false;
    if (L.length >= MAX_SIZE) return false;
    for (int j = L.length; j >= i; --j) {
        L.data[j] = L.data[j - 1];
    }
    L.data[i - 1] = e;
    ++L.length;
    return true;
}

bool ListDelete(SqList& L, int i, int& e) {
    if (i < 1 || i > L.length) return false;
    e = L.data[i - 1];
    for (int j = i; j < L.length; ++j) {
        L.data[j - 1] = L.data[j];
    }
    --L.length;
    return true;
}

// 用 span 只查看有效区间 / Use span to view only [0, length)
int ListLocateElem(const SqList& L, int e) {
    std::span<const int> s(L.data, L.length);
    for (size_t idx = 0; idx < s.size(); ++idx) {
        if (s[idx] == e) return static_cast<int>(idx) + 1; // 1-based
    }
    return -1;
}

int ListGetElem(const SqList& L, int i) {
    if (i < 1 || i > L.length) return -1;
    return L.data[i - 1];
}

// 关键：用 span 构造视图，再范围 for
void ListPrint(const SqList& L) {
    std::span<const int> s(L.data, L.length);
    for (const auto& elem : s) {
        std::cout << elem << std::endl;
    }
}

bool ListInsertTail(SqList& L, int e) {
    if (L.length >= MAX_SIZE) return false;
    L.data[L.length++] = e;
    return true;
}

int main() {
    SqList L;
    InitList(L);
    ListInsert(L, 1, 1);
    ListInsert(L, 1, 2);
    ListInsert(L, 1, 3);
    ListInsert(L, 1, 4);

    // 不会插入（位置越界）/ Will fail (index out of range)
    ListInsert(L, 8, 7);

    ListPrint(L);
    int delValue;
    ListDelete(L, 1, delValue);
    ListPrint(L);
    return 0;
}
