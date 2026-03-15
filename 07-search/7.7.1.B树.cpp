#include <iostream>

using ElemType = int;

// ====== 参数：m 阶 B 树（考研常用定义：每结点最多 m-1 个关键字）======
constexpr int m   = 5;                 // B 树阶数（最大分叉数）
constexpr int Max = m - 1;             // 结点最大关键字数
constexpr int Min = (m - 1) / 2;       // 结点最小关键字数（非根）

// 为了写死数组长度（教材常见写法），给一个上限 MAXM >= m
constexpr int MAXM = 10;
static_assert(m <= MAXM, "m must be <= MAXM");

struct BTNode {
    int keynum{};                          // 当前关键字个数
    ElemType key[MAXM + 1]{};              // key[0] 不用，key[1..keynum]
    BTNode* parent{nullptr};
    BTNode* children[MAXM + 1]{};          // children[0..keynum]
};

using BTree = BTNode*;

struct Result {
    BTNode* pt{nullptr};  // 指向找到的结点；失败时指向应插入的叶子(或其父)
    int i{};              // 关键字位置（或插入位置）
    bool found{};         // 是否找到
};

// ====== 结点分配：保证字段“全初始化” ======
BTNode* NewNode(BTNode* parent = nullptr) {
    auto* p = new BTNode{};
    p->parent = parent;
    // keynum/key/children 已因 {} 全部置 0 / nullptr
    return p;
}

// 在结点 p 中查找位置 i：使得 key[i] <= k < key[i+1] （key[0] 视为 -∞）
int SearchBTNode(const BTNode* p, ElemType k) {
    int i = 0;
    // i 最大到 keynum；使用 key[i+1] 前保证 i < keynum
    while (i < p->keynum && p->key[i + 1] <= k) ++i;
    return i;
}

// 在 B 树 t 上查找 k：找到返回 found=true；否则返回插入位置（pt 指向最后访问结点）
Result SearchBTree(BTree t, ElemType k) {
    BTNode* p = t;
    BTNode* q = nullptr; // p 的父
    int i = 0;

    while (p != nullptr) {
        i = SearchBTNode(p, k);
        if (i > 0 && p->key[i] == k) {
            return Result{p, i, true};
        }
        q = p;
        p = p->children[i]; // 沿第 i 个孩子下降（注意：children[0..keynum]）
    }

    // 未找到：q 是应插入的叶子(或下降到空前的结点)，i 是插入位置
    return Result{q, i, false};
}

// 在结点 p 的第 i 个关键字之后插入 k；并把右子树指针 q 放到 children[i+1]
void InsertBTNode(BTNode* p, int i, ElemType k, BTNode* q) {
    // 关键字与孩子指针后移：从 keynum 开始，移动到 i+1
    for (int j = p->keynum; j > i; --j) {
        p->key[j + 1]      = p->key[j];
        p->children[j + 1] = p->children[j];
    }
    p->key[i + 1]      = k;
    p->children[i + 1] = q;
    if (q) q->parent = p;
    ++p->keynum;
}

// 把结点 p 分裂成 p(保留左半) + q(右半)，并返回提升到父的中间关键字 midKey
void SplitBTNode(BTNode* p, BTNode*& q, ElemType& midKey) {
    // s 为中间关键字下标（提升 key[s] 到父）
    const int s = (m + 1) / 2;

    q = NewNode(p->parent);

    midKey = p->key[s];

    // q 接收右半：key[s+1..oldKeynum] -> q.key[1..]
    // children[s..oldKeynum] -> q.children[0..]
    q->children[0] = p->children[s];
    if (q->children[0]) q->children[0]->parent = q;

    int qi = 1;
    for (int j = s + 1; j <= p->keynum; ++j, ++qi) {
        q->key[qi] = p->key[j];
        q->children[qi] = p->children[j];
        if (q->children[qi]) q->children[qi]->parent = q;
    }
    q->keynum = p->keynum - s;

    // p 保留左半：key[1..s-1]，children[0..s-1]
    p->keynum = s - 1;

    // 可选：把 p 的“废尾部”清一清（防调试时误读；不影响逻辑）
    for (int j = p->keynum + 1; j <= MAXM; ++j) {
        p->key[j] = 0;
        p->children[j] = nullptr;
    }
}

// 生成新根：包含一个关键字 k，两个孩子 p/q
void NewRoot(BTree& t, ElemType k, BTNode* p, BTNode* q) {
    BTNode* r = NewNode(nullptr);
    r->keynum = 1;
    r->key[1] = k;
    r->children[0] = p;
    r->children[1] = q;
    if (p) p->parent = r;
    if (q) q->parent = r;
    t = r;
}

// 插入关键字 k（s.pt 是应插入结点，s.i 是插入位置）
void InsertBTree(BTree& t, int i, ElemType k, BTNode* p) {
    if (p == nullptr) {
        // 空树：直接建根
        NewRoot(t, k, nullptr, nullptr);
        return;
    }

    ElemType x = k;
    BTNode* q = nullptr;  // 分裂产生的新右结点
    bool finished = false;
    bool needNewRoot = false;

    while (!finished && !needNewRoot) {
        InsertBTNode(p, i, x, q);

        if (p->keynum <= Max) {
            finished = true;
        } else {
            ElemType mid{};
            SplitBTNode(p, q, mid); // p 左，q 右，mid 上升
            x = mid;

            if (p->parent) {
                BTNode* parent = p->parent;
                i = SearchBTNode(parent, x);
                p = parent;
            } else {
                needNewRoot = true;
            }
        }
    }

    if (needNewRoot) {
        NewRoot(t, x, p, q);
    }
}

// ====== 删除（教材版）——保留“结构调整”的套路 ======

// 从结点 p 删除 key[i]（并同步左侧 children 指针）
// 这里按“关键字下标 1..keynum”约定
void RemoveKey(BTNode* p, int i) {
    for (int j = i + 1; j <= p->keynum; ++j) {
        p->key[j - 1]      = p->key[j];
        p->children[j - 1] = p->children[j];
    }
    --p->keynum;
}

// 用“右子树最小关键字”替换 p->key[i]
void Substitution(BTNode* p, int i) {
    BTNode* q = p->children[i];
    while (q && q->children[0]) q = q->children[0];
    // q 一定是叶子（或最左路径终点）
    p->key[i] = q->key[1];
}

// 从父结点 p 处，把左兄弟 (i-1) 借一个到右孩子 i（右旋借）
void MoveRight(BTNode* p, int i) {
    BTNode* right = p->children[i];
    BTNode* left  = p->children[i - 1];

    // right 的 key/children 整体后移一格，为插入腾位置
    for (int j = right->keynum; j >= 1; --j) {
        right->key[j + 1]      = right->key[j];
        right->children[j + 1] = right->children[j];
    }
    right->children[1] = right->children[0];

    // 父关键字下沉到 right.key[1]
    right->key[1] = p->key[i];
    right->children[0] = left->children[left->keynum];
    if (right->children[0]) right->children[0]->parent = right;
    ++right->keynum;

    // left 最大关键字上升到父
    p->key[i] = left->key[left->keynum];
    --left->keynum;
}

// 从父结点 p 处，把右兄弟 i 借一个到左孩子 (i-1)（左旋借）
void MoveLeft(BTNode* p, int i) {
    BTNode* left  = p->children[i - 1];
    BTNode* right = p->children[i];

    // 父关键字下沉到 left 的末尾
    ++left->keynum;
    left->key[left->keynum] = p->key[i];
    left->children[left->keynum] = right->children[0];
    if (left->children[left->keynum]) left->children[left->keynum]->parent = left;

    // right 的最小关键字上升到父
    p->key[i] = right->key[1];

    // right 删除 key[1]：整体前移
    right->children[0] = right->children[1];
    for (int j = 1; j < right->keynum; ++j) {
        right->key[j]      = right->key[j + 1];
        right->children[j] = right->children[j + 1];
    }
    --right->keynum;
}

// 合并：把父 p 的 key[i] 下沉到左孩子(i-1)，并把右孩子 i 合并进左孩子；最后删除父的 key[i]
void Combine(BTNode* p, int i) {
    BTNode* right = p->children[i];
    BTNode* left  = p->children[i - 1];

    // 父关键字下沉
    ++left->keynum;
    left->key[left->keynum] = p->key[i];
    left->children[left->keynum] = right->children[0];
    if (left->children[left->keynum]) left->children[left->keynum]->parent = left;

    // right 全部搬过来
    for (int j = 1; j <= right->keynum; ++j) {
        ++left->keynum;
        left->key[left->keynum] = right->key[j];
        left->children[left->keynum] = right->children[j];
        if (left->children[left->keynum]) left->children[left->keynum]->parent = left;
    }

    // 父删除 key[i]，并把 children 指针左移
    for (int j = i; j < p->keynum; ++j) {
        p->key[j] = p->key[j + 1];
        p->children[j] = p->children[j + 1];
    }
    --p->keynum;

    delete right;
}

// 删除后若某孩子关键字数 < Min，则在父 p 处调整（借/并）
void AdjustBTree(BTNode* p, int i) {
    // i 表示“发生下溢的孩子在父中的孩子下标”
    if (i == 0) {
        // 最左孩子：只能向右兄弟借或并
        if (p->children[1]->keynum > Min) MoveLeft(p, 1);
        else Combine(p, 1);
    } else if (i == p->keynum) {
        // 最右孩子：只能向左兄弟借或并
        if (p->children[i - 1]->keynum > Min) MoveRight(p, i);
        else Combine(p, i);
    } else {
        // 中间孩子：优先向“关键字多”的兄弟借
        if (p->children[i - 1]->keynum > Min) MoveRight(p, i);
        else if (p->children[i + 1]->keynum > Min) MoveLeft(p, i + 1);
        else Combine(p, i); // 约定：和左兄弟合并
    }
}

// 在结点 p 中找 k：若找到，i=位置(1..keynum)；否则 i=下降孩子下标(0..keynum)
bool FindBTNode(const BTNode* p, ElemType k, int& i) {
    if (k < p->key[1]) {
        i = 0;
        return false;
    }
    i = p->keynum;
    while (i > 1 && k < p->key[i]) --i;
    if (k == p->key[i]) return true;
    // 未找到：下降到 children[i]
    return false;
}

bool BTNodeDelete(BTNode* p, ElemType k) {
    if (!p) return false;

    int i = 0;
    bool found = FindBTNode(p, k, i);

    if (found) {
        if (p->children[i - 1] != nullptr) {
            // 非叶：用后继替换，再去子树删
            Substitution(p, i);
            return BTNodeDelete(p->children[i], p->key[i]);
        } else {
            // 叶：直接删
            RemoveKey(p, i);
        }
    } else {
        // 未命中：沿 children[i] 继续
        bool ok = BTNodeDelete(p->children[i], k);
        if (!ok) return false;
    }

    // 回溯调整：对可能下溢的孩子做修复
    // 这里对所有孩子做“局部检查”会比较重；教材常按路径孩子做检查。
    // 为了保持清晰：只检查与本次操作相关的 children[i]（若存在）
    if (p->children[i] && p->children[i]->keynum < Min) {
        AdjustBTree(p, i);
    }
    return true;
}

void BTreeDelete(BTree& t, ElemType k) {
    if (!t) {
        std::cout << "B树为空\n";
        return;
    }
    bool ok = BTNodeDelete(t, k);
    if (!ok) {
        std::cout << "关键字 " << k << " 不在B树中\n";
        return;
    }
    // 根缩减：若根无关键字且有唯一孩子，根下移
    if (t->keynum == 0) {
        BTNode* old = t;
        t = t->children[0];
        if (t) t->parent = nullptr;
        delete old;
    }
}

// ====== 层序打印：不用 STL 的链式队列 ======
struct QNode {
    BTNode* data{nullptr};
    QNode* next{nullptr};
};

struct Queue {
    QNode* front{nullptr}; // 头结点
    QNode* rear{nullptr};
};

void Queue_Init(Queue& q) {
    q.front = q.rear = new QNode{}; // 带头结点
}

bool Queue_Empty(const Queue& q) {
    return q.front == q.rear;
}

void Queue_Push(Queue& q, BTNode* x) {
    auto* n = new QNode{};
    n->data = x;
    q.rear->next = n;
    q.rear = n;
}

bool Queue_Pop(Queue& q, BTNode*& out) {
    if (Queue_Empty(q)) return false;
    QNode* first = q.front->next;
    out = first->data;
    q.front->next = first->next;
    if (q.rear == first) q.rear = q.front;
    delete first;
    return true;
}

void Queue_Destroy(Queue& q) {
    while (q.front) {
        QNode* nxt = q.front->next;
        delete q.front;
        q.front = nxt;
    }
    q.rear = nullptr;
}

// 打印一棵 B 树（按层输出，每个结点打印其关键字）
void PrintBTree(BTree t) {
    if (!t) {
        std::cout << "B树为空\n";
        return;
    }

    Queue q;
    Queue_Init(q);
    Queue_Push(q, t);

    // “按层换行”：用当前层结点数计数（不用额外容器）
    int curLevel = 1;
    int nextLevel = 0;

    while (!Queue_Empty(q)) {
        BTNode* p = nullptr;
        Queue_Pop(q, p);
        --curLevel;

        std::cout << "[ ";
        for (int i = 1; i <= p->keynum; ++i) std::cout << p->key[i] << ' ';
        std::cout << "] ";

        for (int i = 0; i <= p->keynum; ++i) {
            if (p->children[i]) {
                Queue_Push(q, p->children[i]);
                ++nextLevel;
            }
        }

        if (curLevel == 0) {
            std::cout << '\n';
            curLevel = nextLevel;
            nextLevel = 0;
        }
    }

    Queue_Destroy(q);
}

// 递归释放整棵树
void DestroyBTree(BTree& t) {
    if (!t) return;
    for (int i = 0; i <= t->keynum; ++i) {
        DestroyBTree(t->children[i]);
    }
    delete t;
    t = nullptr;
}

// ====== Demo ======
int main() {
    BTree t = nullptr;

    ElemType a[] = {25, 38, 49, 60, 80, 90, 99, 88, 83, 87, 70, 92, 93, 94, 73, 74, 75};
    const int n = static_cast<int>(sizeof(a) / sizeof(a[0]));

    std::cout << "创建一棵 " << m << " 阶B树:\n";
    for (int i = 0; i < n; ++i) {
        Result s = SearchBTree(t, a[i]);
        if (!s.found) InsertBTree(t, s.i, a[i], s.pt);
        std::cout << "第 " << (i + 1) << " 步, 插入 " << a[i] << ":\n";
        PrintBTree(t);
    }

    std::cout << "\n删除操作:\n";
    int k = 25;
    BTreeDelete(t, k);
    std::cout << "删除 " << k << " 后:\n";
    PrintBTree(t);

    k = 1;
    BTreeDelete(t, k);
    std::cout << "删除 " << k << " 后:\n";
    PrintBTree(t);

    std::cout << "\n递归释放B树\n";
    DestroyBTree(t);
    PrintBTree(t);
    return 0;
}