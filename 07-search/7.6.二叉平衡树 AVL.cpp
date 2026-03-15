#include <iostream>

struct AVLNode {
    int key;            // 关键字
    int height;         // 结点高度
    AVLNode* left;      // 左孩子
    AVLNode* right;     // 右孩子

    AVLNode(int k)
        : key(k), height(1), left(nullptr), right(nullptr) {}
};

int height(AVLNode* node) {
    return node ? node->height : 0;
}

int max(int a, int b) {
    return a > b ? a : b;
}

// 平衡因子 = 左子树高度 - 右子树高度
int balanceFactor(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

AVLNode* rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // 旋转
    x->right = y;
    y->left = T2;

    // 更新高度（先 y 后 x）
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x; // 新根
}

AVLNode* rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // 旋转
    y->left = x;
    x->right = T2;

    // 更新高度
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y; // 新根
}

AVLNode* AVL_Insert(AVLNode* root, int key) {
    // 1. 按 BST 规则插入
    if (root == nullptr)
        return new AVLNode(key);

    if (key < root->key)
        root->left = AVL_Insert(root->left, key);
    else if (key > root->key)
        root->right = AVL_Insert(root->right, key);
    else
        return root; // 不允许重复关键字

    // 2. 更新高度
    root->height = max(height(root->left), height(root->right)) + 1;

    // 3. 计算平衡因子
    int bf = balanceFactor(root);

    // 4. 四种失衡情况

    // LL
    if (bf > 1 && key < root->left->key)
        return rotateRight(root);

    // RR
    if (bf < -1 && key > root->right->key)
        return rotateLeft(root);

    // LR
    if (bf > 1 && key > root->left->key) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // RL
    if (bf < -1 && key < root->right->key) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

void InOrder(AVLNode* root) {
    if (!root) return;
    InOrder(root->left);
    std::cout << root->key << " ";
    InOrder(root->right);
}

void DestroyAVL(AVLNode* root) {
    if (!root) return;
    DestroyAVL(root->left);
    DestroyAVL(root->right);
    delete root;
}

int main() {
    AVLNode* root = nullptr;

    int data[] = {10, 20, 30, 40, 50, 25};
    int n = sizeof(data) / sizeof(int);

    for (int i = 0; i < n; ++i) {
        root = AVL_Insert(root, data[i]);
    }

    std::cout << "AVL 树中序遍历结果：";
    InOrder(root);
    std::cout << "\n";

    DestroyAVL(root);
    return 0;
}