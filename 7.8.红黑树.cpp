#include <iostream>

class RBTree {
public:
    using Key = int;

private:
    enum class Color : unsigned char { Red, Black };

    struct Node {
        Key key{};
        Color color{Color::Red};
        Node* parent{nullptr};
        Node* left{nullptr};
        Node* right{nullptr};
    };

    Node* root_{nullptr};

private:
    static bool is_red(const Node* x) noexcept {
        return x != nullptr && x->color == Color::Red;
    }
    static bool is_black(const Node* x) noexcept {
        return x == nullptr || x->color == Color::Black;
    }
    static void set_red(Node* x) noexcept {
        if (x) x->color = Color::Red;
    }
    static void set_black(Node* x) noexcept {
        if (x) x->color = Color::Black;
    }

    // 左旋：以 x 为支点，把 x 的右孩子 y 提上来
    void rotate_left(Node* x) noexcept {
        Node* y = x->right;
        // y 的左子树接到 x 的右边
        x->right = y->left;
        if (y->left) y->left->parent = x;

        // y 接到 x 的父结点下面
        y->parent = x->parent;
        if (!x->parent) {
            root_ = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        // x 挂到 y 的左边
        y->left = x;
        x->parent = y;
    }

    // 右旋：以 x 为支点，把 x 的左孩子 y 提上来
    void rotate_right(Node* x) noexcept {
        Node* y = x->left;
        // y 的右子树接到 x 的左边
        x->left = y->right;
        if (y->right) y->right->parent = x;

        // y 接到 x 的父结点下面
        y->parent = x->parent;
        if (!x->parent) {
            root_ = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        // x 挂到 y 的右边
        y->right = x;
        x->parent = y;
    }

    // 插入后的修复（考研核心）
    void insert_fixup(Node* z) noexcept {
        // 只要父亲是红，就违反“红结点不能有红孩子”
        while (z->parent && is_red(z->parent)) {
            Node* p = z->parent;
            Node* g = p->parent;          // 祖父（一定存在，因为父为红，根必黑）
            if (!g) break;

            if (p == g->left) {
                Node* u = g->right;       // 叔叔

                // Case 1：叔叔是红 -> 变色上溯
                if (is_red(u)) {
                    set_black(p);
                    set_black(u);
                    set_red(g);
                    z = g;
                }
                // Case 2/3：叔叔是黑（或空）
                else {
                    // Case 2：z 是右孩子 -> 先对父亲左旋转，转成 Case 3
                    if (z == p->right) {
                        z = p;
                        rotate_left(z);
                        p = z->parent;
                        g = p ? p->parent : nullptr;
                    }
                    // Case 3：z 是左孩子 -> 对祖父右旋 + 变色
                    set_black(p);
                    set_red(g);
                    rotate_right(g);
                }
            } else {
                // 对称情况：父亲在祖父右边
                Node* u = g->left;

                // Case 1（对称）：叔叔红
                if (is_red(u)) {
                    set_black(p);
                    set_black(u);
                    set_red(g);
                    z = g;
                } else {
                    // Case 2（对称）：z 是左孩子 -> 先右旋父亲
                    if (z == p->left) {
                        z = p;
                        rotate_right(z);
                        p = z->parent;
                        g = p ? p->parent : nullptr;
                    }
                    // Case 3（对称）：z 是右孩子 -> 左旋祖父 + 变色
                    set_black(p);
                    set_red(g);
                    rotate_left(g);
                }
            }
        }
        // 根必须黑
        set_black(root_);
    }

    static void inorder_print(const Node* x) {
        if (!x) return;
        inorder_print(x->left);
        std::cout << x->key << (x->color == Color::Red ? "(R) " : "(B) ");
        inorder_print(x->right);
    }

    static void destroy(Node* x) {
        if (!x) return;
        destroy(x->left);
        destroy(x->right);
        delete x;
    }

public:
    RBTree() = default;
    ~RBTree() { destroy(root_); }

    RBTree(const RBTree&) = delete;
    RBTree& operator=(const RBTree&) = delete;

    Node* find(Key k) const noexcept {
        Node* cur = root_;
        while (cur) {
            if (k == cur->key) return cur;
            cur = (k < cur->key) ? cur->left : cur->right;
        }
        return nullptr;
    }

    // 插入：BST 插入 + 修复
    bool insert(Key k) {
        // 1) 标准 BST 插入
        Node* y = nullptr;
        Node* x = root_;
        while (x) {
            y = x;
            if (k == x->key) return false; // 不插重复
            x = (k < x->key) ? x->left : x->right;
        }

        Node* z = new Node{};
        z->key = k;
        z->color = Color::Red;
        z->parent = y;

        if (!y) {
            root_ = z;                 // 空树，z 为根
        } else if (k < y->key) {
            y->left = z;
        } else {
            y->right = z;
        }

        // 2) 插入修复
        insert_fixup(z);
        return true;
    }

    void print_inorder() const {
        inorder_print(root_);
        std::cout << "\n";
    }
};

int main() {
    RBTree t;

    int a[] = {10, 20, 30, 15, 25, 5, 1, 8, 12};
    constexpr int n = sizeof(a) / sizeof(a[0]);

    for (int i = 0; i < n; ++i) {
        t.insert(a[i]);
        std::cout << "After insert " << a[i] << ": ";
        t.print_inorder();
    }

    return 0;
}