#include <iostream>
using namespace std;

#define MAX_SIZE 7
#define END   -1   // 链表尾
#define FREE  -2   // 空闲

typedef int ElemType;

// 静态链表结点数组
typedef struct {
    ElemType data;
    int next; // 下一个结点的下标：END=-1；FREE=-2
} SLinkList[MAX_SIZE];

// 初始化：0 号位作“头结点”（不存有效数据）
void Initialize(SLinkList list) {
    list[0].next = END;
    for (int i = 1; i < MAX_SIZE; ++i) list[i].next = FREE;
}

// 计算当前长度（数据结点个数）
int Length(const SLinkList list) {
    int cnt = 0;
    for (int cur = list[0].next; cur != END; cur = list[cur].next) ++cnt;
    return cnt;
}

// 找空闲结点下标（找不到返回 -1）
int FindFree(const SLinkList list) {
    for (int i = 1; i < MAX_SIZE; ++i)
        if (list[i].next == FREE) return i;
    return -1;
}

// 按 0-based 位置插入（position ∈ [0, length]）
bool Insert(SLinkList list, int position, ElemType value) {
    int n = Length(list);
    if (position < 0 || position > n) {
        cout << "插入失败：位置无效\n";
        return false;
    }
    int freeNode = FindFree(list);
    if (freeNode == -1) {
        cout << "插入失败：静态链表已满\n";
        return false;
    }

    // 从头结点出发，走到要插入位置的“前驱”下标
    int prev = 0;           // 0 号为头结点
    for (int k = 0; k < position; ++k) {
        prev = list[prev].next;
    }

    // 摘链插入：freeNode -> prev.next
    list[freeNode].data = value;
    list[freeNode].next = list[prev].next;
    list[prev].next = freeNode;
    return true;
}

// 按值查找：返回 0-based 位置（未找到返回 -1）
int FindByValue(const SLinkList list, ElemType value) {
    int pos = 0;
    for (int cur = list[0].next; cur != END; cur = list[cur].next, ++pos) {
        if (list[cur].data == value) return pos;
    }
    return -1;
}

// 删除 0-based 位置（position ∈ [0, length-1]）
bool Delete(SLinkList list, int position) {
    int n = Length(list);
    if (position < 0 || position >= n) {
        cout << "删除失败：位置无效\n";
        return false;
    }

    // 找到前驱
    int prev = 0;
    for (int k = 0; k < position; ++k) {
        prev = list[prev].next;
    }

    int toDel = list[prev].next;       // 待删结点
    list[prev].next = list[toDel].next; // 跨过它

    // 标记空闲（数据域无需特意清零）
    list[toDel].next = FREE;
    return true;
}

// 输出链表
void PrintList(const SLinkList list) {
    for (int cur = list[0].next; cur != END; cur = list[cur].next) {
        cout << list[cur].data << ' ';
    }
    cout << '\n';
}

int main() {
    SLinkList list;
    Initialize(list);

    // 统一按 0-based 的位置插入
    Insert(list, 0, 10); // [10]
    Insert(list, 1, 20); // [10,20]
    Insert(list, 1, 30); // [10,30,20]
    Insert(list, 3, 40); // [10,30,20,40]

    cout << "静态链表内容：\n";
    PrintList(list);

    // 删除位置 1（即“第二个元素”）
    Delete(list, 1);
    cout << "删除第 2 个元素后的静态链表内容：\n";
    PrintList(list);

    // 查找
    cout << "值 40 的位置（0-based）：" << FindByValue(list, 40) << '\n';
    cout << "长度：" << Length(list) << '\n';
    return 0;
}
