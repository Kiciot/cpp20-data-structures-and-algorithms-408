#include <iostream>

using KeyType   = int;
using ValueType = int;

constexpr int m   = 5;            // B+树阶（最大孩子数）
constexpr int Max = m - 1;        // 最大关键字数
constexpr int Min = (m - 1) / 2;  // 最小关键字数（非根）
constexpr int MAXM = 10;          // 固定数组上限，要求 MAXM >= m

static_assert(m <= MAXM, "m must be <= MAXM");

struct BPTNode {
    bool isLeaf{false};
    int keynum{0};                        // 当前 key 数
    KeyType keys[MAXM + 1]{};             // keys[0..keynum-1] 使用
    BPTNode* parent{nullptr};

    // 内部结点：children[0..keynum] 共 keynum+1 个
    // 叶子结点：children 不用
    BPTNode* children[MAXM + 2]{};        // 多留一格，便于临时溢出

    // 叶子结点：values[0..keynum-1]
    ValueType values[MAXM + 1]{};

    // 叶子链表
    BPTNode* next{nullptr};
};

struct BPlusTree {
    BPTNode* root{nullptr};
};

// ====== new 一个结点：全初始化为 0/nullptr ======
BPTNode* NewNode(bool leaf) {
    auto* p = new BPTNode{};
    p->isLeaf = leaf;
    return p;
}

// ====== 工具：在 keys[] 中找到第一个 >= k 的位置（lower_bound）=====
int LowerBound(const KeyType* a, int n, KeyType k) {
    int l = 0, r = n; // [l, r)
    while (l < r) {
        int mid = (l + r) / 2;
        if (a[mid] < k) l = mid + 1;
        else r = mid;
    }
    return l;
}

// ====== 找叶子：沿内部结点下降 ======
BPTNode* FindLeaf(BPlusTree& T, KeyType k) {
    BPTNode* p = T.root;
    if (!p) return nullptr;
    while (!p->isLeaf) {
        // 内部结点：选择孩子 i，使 keys[i-1] <= k < keys[i]
        int i = LowerBound(p->keys, p->keynum, k);
        p = p->children[i];
    }
    return p;
}

// ====== 在叶子中查找 key，成功返回 true 并输出 value ======
bool Search(BPlusTree& T, KeyType k, ValueType& out) {
    BPTNode* leaf = FindLeaf(T, k);
    if (!leaf) return false;
    int i = LowerBound(leaf->keys, leaf->keynum, k);
    if (i < leaf->keynum && leaf->keys[i] == k) {
        out = leaf->values[i];
        return true;
    }
    return false;
}

// ====== 内部结点：重新设置某个孩子在父中的“分隔键” ======
// 约定：父节点 keys[i] 存的是 children[i+1] 子树的最小 key（常见实现之一）
KeyType SubtreeMinKey(BPTNode* p) {
    // 从 p 开始一直走到最左叶子
    while (p && !p->isLeaf) p = p->children[0];
    return p ? p->keys[0] : 0;
}

void FixParentSeparator(BPTNode* parent, int childIndexPlus1) {
    // parent->keys[childIndexPlus1 - 1] = minKey(children[childIndexPlus1])
    int ki = childIndexPlus1 - 1;
    parent->keys[ki] = SubtreeMinKey(parent->children[childIndexPlus1]);
}

// ====== 向父插入：把 newChild 插到 parent 的 children[pos+1]，并插入分隔 key ======
void InsertIntoInternal(BPTNode* parent, int pos, KeyType sepKey, BPTNode* newChild) {
    // parent 是内部结点，pos 是“插入 sepKey 的位置”，sepKey 对应 newChild 的最小 key
    // 插入 keys[pos]，并让 newChild 成为 children[pos+1]
    for (int j = parent->keynum; j > pos; --j) {
        parent->keys[j] = parent->keys[j - 1];
    }
    for (int j = parent->keynum + 1; j > pos + 1; --j) {
        parent->children[j] = parent->children[j - 1];
    }
    parent->keys[pos] = sepKey;
    parent->children[pos + 1] = newChild;
    newChild->parent = parent;
    ++parent->keynum;
}

// ====== 分裂内部结点 ======
void SplitInternal(BPlusTree& T, BPTNode* node) {
    // node 已经溢出（keynum = Max+1）
    // 把 node 分裂成 node(left) + right，并把 right 的最小 key 插入父
    int mid = node->keynum / 2; // 平均分
    auto* right = NewNode(false);
    right->parent = node->parent;

    // right 接收 keys[mid+1 .. end-1]，children[mid+1 .. end]
    // 注意：B+树内部结点的“分隔key”不下沉到子？这里采用常见做法：把 keys[mid] 作为提升 key
    KeyType upKey = node->keys[mid];

    // right 的孩子从 mid+1 开始
    int rk = 0;
    for (int j = mid + 1; j < node->keynum; ++j) {
        right->keys[rk++] = node->keys[j];
    }
    int rc = 0;
    for (int j = mid + 1; j <= node->keynum; ++j) {
        right->children[rc] = node->children[j];
        if (right->children[rc]) right->children[rc]->parent = right;
        ++rc;
    }
    right->keynum = rk;

    // 左边 node 保留 keys[0..mid-1]，children[0..mid]
    node->keynum = mid;
    // 清理 node 右侧指针（可选，便于调试）
    for (int j = node->keynum; j <= MAXM; ++j) {
        node->keys[j] = 0;
        node->children[j + 1] = nullptr;
    }

    if (!node->parent) {
        // node 是根：新建根
        auto* newRoot = NewNode(false);
        newRoot->keys[0] = upKey;
        newRoot->children[0] = node;
        newRoot->children[1] = right;
        newRoot->keynum = 1;
        node->parent = newRoot;
        right->parent = newRoot;
        T.root = newRoot;
        return;
    }

    // 插入到父：upKey 对应 right 子树
    BPTNode* parent = node->parent;
    int pos = LowerBound(parent->keys, parent->keynum, upKey);
    InsertIntoInternal(parent, pos, upKey, right);

    if (parent->keynum > Max) SplitInternal(T, parent);
}

// ====== 分裂叶子结点 ======
void SplitLeaf(BPlusTree& T, BPTNode* leaf) {
    // leaf 已溢出（keynum = Max+1）
    auto* right = NewNode(true);
    right->parent = leaf->parent;

    int mid = leaf->keynum / 2; // left: mid 个，right: rest
    int rcount = leaf->keynum - mid;

    for (int i = 0; i < rcount; ++i) {
        right->keys[i] = leaf->keys[mid + i];
        right->values[i] = leaf->values[mid + i];
    }
    right->keynum = rcount;
    leaf->keynum = mid;

    // 叶子链表
    right->next = leaf->next;
    leaf->next = right;

    // right 的最小 key 作为分隔 key 插到父
    KeyType sepKey = right->keys[0];

    if (!leaf->parent) {
        // leaf 是根：新建根（内部结点）
        auto* newRoot = NewNode(false);
        newRoot->keys[0] = sepKey;
        newRoot->children[0] = leaf;
        newRoot->children[1] = right;
        newRoot->keynum = 1;
        leaf->parent = newRoot;
        right->parent = newRoot;
        T.root = newRoot;
        return;
    }

    BPTNode* parent = leaf->parent;
    int pos = LowerBound(parent->keys, parent->keynum, sepKey);
    InsertIntoInternal(parent, pos, sepKey, right);

    if (parent->keynum > Max) SplitInternal(T, parent);
}

// ====== 插入/更新 ======
void Insert(BPlusTree& T, KeyType k, ValueType v) {
    if (!T.root) {
        T.root = NewNode(true);
        T.root->keys[0] = k;
        T.root->values[0] = v;
        T.root->keynum = 1;
        return;
    }

    BPTNode* leaf = FindLeaf(T, k);
    int i = LowerBound(leaf->keys, leaf->keynum, k);

    // 已存在：更新
    if (i < leaf->keynum && leaf->keys[i] == k) {
        leaf->values[i] = v;
        return;
    }

    // 插入到叶子：后移
    for (int j = leaf->keynum; j > i; --j) {
        leaf->keys[j] = leaf->keys[j - 1];
        leaf->values[j] = leaf->values[j - 1];
    }
    leaf->keys[i] = k;
    leaf->values[i] = v;
    ++leaf->keynum;

    if (leaf->keynum > Max) {
        SplitLeaf(T, leaf);
    } else {
        // 如果插到了叶子的最前面，可能影响父分隔键（因为父存的是右孩子最小key）
        // 这里采用“父keys代表右孩子最小key”的规则：只有当 leaf 不是父的 children[0]，
        // 且 leaf 的最小key改变时，需要更新父中对应分隔键。
        if (leaf->parent) {
            BPTNode* p = leaf->parent;
            // 找 leaf 在父 children[] 的位置
            int idx = 0;
            while (idx <= p->keynum && p->children[idx] != leaf) ++idx;
            if (idx >= 1) {
                p->keys[idx - 1] = leaf->keys[0];
            }
        }
    }
}

// ====== 删除辅助：在叶子中删除一个 key ======
bool RemoveFromLeaf(BPTNode* leaf, KeyType k) {
    int i = LowerBound(leaf->keys, leaf->keynum, k);
    if (i >= leaf->keynum || leaf->keys[i] != k) return false;

    for (int j = i + 1; j < leaf->keynum; ++j) {
        leaf->keys[j - 1] = leaf->keys[j];
        leaf->values[j - 1] = leaf->values[j];
    }
    --leaf->keynum;
    return true;
}

// ====== 取得结点在父中的孩子下标 ======
int ChildIndex(BPTNode* parent, BPTNode* child) {
    for (int i = 0; i <= parent->keynum; ++i) {
        if (parent->children[i] == child) return i;
    }
    return -1;
}

// ====== 叶子结点借/并 ======
void RebalanceLeaf(BPlusTree& T, BPTNode* leaf) {
    if (!leaf->parent) {
        // 根是叶子：允许 keynum=0 => 空树
        if (leaf->keynum == 0) {
            delete leaf;
            T.root = nullptr;
        }
        return;
    }

    if (leaf->keynum >= Min) {
        // 够用：可能需要修父分隔键（leaf 最小key可能变）
        BPTNode* p = leaf->parent;
        int idx = ChildIndex(p, leaf);
        if (idx >= 1) p->keys[idx - 1] = leaf->keys[0];
        return;
    }

    BPTNode* p = leaf->parent;
    int idx = ChildIndex(p, leaf);

    BPTNode* left  = (idx > 0) ? p->children[idx - 1] : nullptr;
    BPTNode* right = (idx < p->keynum) ? p->children[idx + 1] : nullptr;

    // 1) 向左兄弟借（左兄弟多于 Min）
    if (left && left->isLeaf && left->keynum > Min) {
        // 把 left 最后一个移到 leaf 最前面
        for (int j = leaf->keynum; j > 0; --j) {
            leaf->keys[j] = leaf->keys[j - 1];
            leaf->values[j] = leaf->values[j - 1];
        }
        leaf->keys[0] = left->keys[left->keynum - 1];
        leaf->values[0] = left->values[left->keynum - 1];
        --left->keynum;
        ++leaf->keynum;

        // 更新父分隔键：leaf 的最小 key 变了 => p->keys[idx-1]
        p->keys[idx - 1] = leaf->keys[0];
        return;
    }

    // 2) 向右兄弟借
    if (right && right->isLeaf && right->keynum > Min) {
        // 把 right 第一个移到 leaf 末尾
        leaf->keys[leaf->keynum] = right->keys[0];
        leaf->values[leaf->keynum] = right->values[0];
        ++leaf->keynum;

        // right 左移
        for (int j = 1; j < right->keynum; ++j) {
            right->keys[j - 1] = right->keys[j];
            right->values[j - 1] = right->values[j];
        }
        --right->keynum;

        // 更新父分隔键：right 的最小 key 变了 => p->keys[idx]
        p->keys[idx] = right->keys[0];
        return;
    }

    // 3) 需要合并：优先与左合并，否则与右合并
    if (left && left->isLeaf) {
        // merge leaf into left
        for (int i = 0; i < leaf->keynum; ++i) {
            left->keys[left->keynum + i] = leaf->keys[i];
            left->values[left->keynum + i] = leaf->values[i];
        }
        left->keynum += leaf->keynum;
        left->next = leaf->next;
        delete leaf;

        // 父删除分隔键 p->keys[idx-1]，并删除 children[idx]
        for (int j = idx; j < p->keynum; ++j) {
            p->keys[j - 1] = p->keys[j];
            p->children[j] = p->children[j + 1];
        }
        --p->keynum;

        // 上溯处理父
    } else if (right && right->isLeaf) {
        // merge right into leaf
        for (int i = 0; i < right->keynum; ++i) {
            leaf->keys[leaf->keynum + i] = right->keys[i];
            leaf->values[leaf->keynum + i] = right->values[i];
        }
        leaf->keynum += right->keynum;
        leaf->next = right->next;
        delete right;

        // 父删除分隔键 p->keys[idx]，并删除 children[idx+1]
        for (int j = idx + 1; j < p->keynum; ++j) {
            p->keys[j - 1] = p->keys[j];
            p->children[j] = p->children[j + 1];
        }
        --p->keynum;

        // 修正 leaf 在父中的分隔键（如果 leaf 不是第0个孩子）
        if (idx >= 1) p->keys[idx - 1] = leaf->keys[0];
    }

    // 父结点可能下溢
    // 在这里统一交给内部结点 rebalance
}

// ====== 内部结点借/并 ======
void RebalanceInternal(BPlusTree& T, BPTNode* node) {
    if (!node->parent) {
        // 根缩减：若根无 key，则把唯一孩子提为根
        if (!node->isLeaf && node->keynum == 0) {
            BPTNode* newRoot = node->children[0];
            if (newRoot) newRoot->parent = nullptr;
            delete node;
            T.root = newRoot;
        }
        return;
    }

    if (node->keynum >= Min) {
        // 内部结点关键字足够：无需处理
        return;
    }

    BPTNode* p = node->parent;
    int idx = ChildIndex(p, node);

    BPTNode* left  = (idx > 0) ? p->children[idx - 1] : nullptr;
    BPTNode* right = (idx < p->keynum) ? p->children[idx + 1] : nullptr;

    // 1) 向左借：left.keynum > Min
    if (left && !left->isLeaf && left->keynum > Min) {
        // node 整体右移，为插入腾位：keys[0..] 右移一格，children[0..] 右移一格
        for (int j = node->keynum; j > 0; --j) node->keys[j] = node->keys[j - 1];
        for (int j = node->keynum + 1; j > 0; --j) node->children[j] = node->children[j - 1];

        // 把父分隔键下沉到 node->keys[0]
        node->keys[0] = p->keys[idx - 1];

        // left 的最后一个孩子挪给 node 的 children[0]
        node->children[0] = left->children[left->keynum];
        if (node->children[0]) node->children[0]->parent = node;

        ++node->keynum;

        // left 的最后一个 key 上升到父
        p->keys[idx - 1] = left->keys[left->keynum - 1];
        --left->keynum;
        return;
    }

    // 2) 向右借
    if (right && !right->isLeaf && right->keynum > Min) {
        // 把父分隔键下沉到 node 尾部
        node->keys[node->keynum] = p->keys[idx];

        // 把 right 的最左孩子给 node 的最右孩子位
        node->children[node->keynum + 1] = right->children[0];
        if (node->children[node->keynum + 1]) node->children[node->keynum + 1]->parent = node;

        ++node->keynum;

        // right 的第一个 key 上升到父
        p->keys[idx] = right->keys[0];

        // right 左移 keys/children（删掉 keys[0] 和 children[0]）
        for (int j = 1; j < right->keynum; ++j) right->keys[j - 1] = right->keys[j];
        for (int j = 1; j <= right->keynum; ++j) right->children[j - 1] = right->children[j];
        --right->keynum;
        return;
    }

    // 3) 合并：优先与左合并，否则与右合并
    if (left && !left->isLeaf) {
        // left + (父分隔键) + node 合并进 left
        left->keys[left->keynum] = p->keys[idx - 1];
        ++left->keynum;

        // node 的 keys/children 追加到 left
        for (int j = 0; j < node->keynum; ++j) {
            left->keys[left->keynum + j] = node->keys[j];
        }
        for (int j = 0; j <= node->keynum; ++j) {
            left->children[left->keynum + j] = node->children[j];
            if (left->children[left->keynum + j]) left->children[left->keynum + j]->parent = left;
        }
        left->keynum += node->keynum;

        delete node;

        // 父删除 keys[idx-1] 与 children[idx]
        for (int j = idx; j < p->keynum; ++j) {
            p->keys[j - 1] = p->keys[j];
            p->children[j] = p->children[j + 1];
        }
        --p->keynum;

    } else if (right && !right->isLeaf) {
        // node + (父分隔键) + right 合并进 node
        node->keys[node->keynum] = p->keys[idx];
        ++node->keynum;

        for (int j = 0; j < right->keynum; ++j) {
            node->keys[node->keynum + j] = right->keys[j];
        }
        for (int j = 0; j <= right->keynum; ++j) {
            node->children[node->keynum + j] = right->children[j];
            if (node->children[node->keynum + j]) node->children[node->keynum + j]->parent = node;
        }
        node->keynum += right->keynum;

        delete right;

        // 父删除 keys[idx] 与 children[idx+1]
        for (int j = idx + 1; j < p->keynum; ++j) {
            p->keys[j - 1] = p->keys[j];
            p->children[j] = p->children[j + 1];
        }
        --p->keynum;
    }

    // 父可能下溢，继续上溯
    RebalanceInternal(T, p);
}

// ====== 删除：先删叶子，再自底向上调整 ======
bool Remove(BPlusTree& T, KeyType k) {
    BPTNode* leaf = FindLeaf(T, k);
    if (!leaf) return false;

    bool ok = RemoveFromLeaf(leaf, k);
    if (!ok) return false;

    // 若 leaf 是根，直接处理
    if (!leaf->parent) {
        if (leaf->keynum == 0) {
            delete leaf;
            T.root = nullptr;
        }
        return true;
    }

    // 叶子下溢/或需要修父分隔键
    RebalanceLeaf(T, leaf);

    // 叶子合并会导致父 keynum 变化：需要对父做内部 rebalance
    if (T.root && !T.root->isLeaf) {
        // 从 leaf 的父开始向上修（如果 leaf 已被删，则 leaf 不可用；但 RebalanceLeaf 中已处理 delete 的情况）
        // 为稳妥：重新定位一下父（从根找 k 所在叶可能成本较高，这里简化：只从 root 开始不做定位）
        // 实用写法：在 RebalanceLeaf 里合并后你本就有 p 指针，并可直接 RebalanceInternal(T, p)。
        // 这里做一次保底：若根关键字为0也能缩减
        RebalanceInternal(T, T.root);
    }
    return true;
}

// ====== 范围查询：从 >=L 的叶子开始沿 next 扫到 R ======
void RangeScan(BPlusTree& T, KeyType L, KeyType R) {
    BPTNode* leaf = FindLeaf(T, L);
    if (!leaf) return;

    while (leaf) {
        for (int i = 0; i < leaf->keynum; ++i) {
            KeyType k = leaf->keys[i];
            if (k < L) continue;
            if (k > R) return;
            std::cout << "(" << k << "," << leaf->values[i] << ") ";
        }
        leaf = leaf->next;
    }
}

// ====== 层序打印：手写链式队列（不使用 STL queue）=====
struct QNode {
    BPTNode* p{nullptr};
    QNode* next{nullptr};
};
struct Queue {
    QNode* front{nullptr};
    QNode* rear{nullptr};
};
void QueueInit(Queue& q) { q.front = q.rear = new QNode{}; }
bool QueueEmpty(const Queue& q) { return q.front == q.rear; }
void Enqueue(Queue& q, BPTNode* p) {
    auto* n = new QNode{};
    n->p = p;
    q.rear->next = n;
    q.rear = n;
}
bool Dequeue(Queue& q, BPTNode*& out) {
    if (QueueEmpty(q)) return false;
    QNode* f = q.front->next;
    out = f->p;
    q.front->next = f->next;
    if (q.rear == f) q.rear = q.front;
    delete f;
    return true;
}
void QueueDestroy(Queue& q) {
    while (q.front) {
        QNode* nx = q.front->next;
        delete q.front;
        q.front = nx;
    }
    q.rear = nullptr;
}

void PrintTree(BPlusTree& T) {
    if (!T.root) {
        std::cout << "Empty B+Tree\n";
        return;
    }
    Queue q;
    QueueInit(q);
    Enqueue(q, T.root);

    int cur = 1, nxt = 0;
    while (!QueueEmpty(q)) {
        BPTNode* p = nullptr;
        Dequeue(q, p);
        --cur;

        if (p->isLeaf) {
            std::cout << "L[ ";
            for (int i = 0; i < p->keynum; ++i) std::cout << p->keys[i] << ' ';
            std::cout << "] ";
        } else {
            std::cout << "I{ ";
            for (int i = 0; i < p->keynum; ++i) std::cout << p->keys[i] << ' ';
            std::cout << "} ";
            for (int i = 0; i <= p->keynum; ++i) {
                if (p->children[i]) {
                    Enqueue(q, p->children[i]);
                    ++nxt;
                }
            }
        }

        if (cur == 0) {
            std::cout << '\n';
            cur = nxt;
            nxt = 0;
        }
    }

    QueueDestroy(q);

    // 打印叶子链表
    BPTNode* p = T.root;
    while (p && !p->isLeaf) p = p->children[0];
    std::cout << "Leaf chain: ";
    while (p) {
        std::cout << "[";
        for (int i = 0; i < p->keynum; ++i) std::cout << p->keys[i] << (i + 1 == p->keynum ? "" : " ");
        std::cout << "] -> ";
        p = p->next;
    }
    std::cout << "null\n";
}

// ====== 递归释放整棵树 ======
void DestroyNode(BPTNode* p) {
    if (!p) return;
    if (!p->isLeaf) {
        for (int i = 0; i <= p->keynum; ++i) DestroyNode(p->children[i]);
    }
    delete p;
}
void DestroyTree(BPlusTree& T) {
    DestroyNode(T.root);
    T.root = nullptr;
}

// ====== Demo ======
int main() {
    BPlusTree T;

    // 插入一些 key
    int keys[] = {25, 38, 49, 60, 80, 90, 99, 88, 83, 87, 70, 92, 93, 94, 73, 74, 75};
    int n = static_cast<int>(sizeof(keys) / sizeof(keys[0]));

    std::cout << "Insert:\n";
    for (int i = 0; i < n; ++i) {
        Insert(T, keys[i], keys[i] * 10);
        // 你想看每一步结构可打开：
        // std::cout << "After insert " << keys[i] << ":\n"; PrintTree(T);
    }
    PrintTree(T);

    std::cout << "\nSearch 87:\n";
    ValueType v{};
    if (Search(T, 87, v)) std::cout << "found value=" << v << "\n";
    else std::cout << "not found\n";

    std::cout << "\nRangeScan [70, 95]:\n";
    RangeScan(T, 70, 95);
    std::cout << "\n";

    std::cout << "\nRemove 25, 90, 87:\n";
    Remove(T, 25);
    Remove(T, 90);
    Remove(T, 87);
    PrintTree(T);

    DestroyTree(T);
    return 0;
}