#include <iostream>

class HashTableChaining {
public:
    using Value = int;

    // 表长：教材常用固定常量
    static constexpr int TABLE_SIZE = 13;

private:
    struct Node {
        Value value{};
        Node* next{nullptr};
    };

    Node* table_[TABLE_SIZE]{};

private:
    inline static constexpr int P = []() consteval {
        auto is_prime = [](int n) consteval noexcept {
            if (n <= 1) return false;
            if (n <= 3) return true;
            if (n % 2 == 0 || n % 3 == 0) return false;
            for (int i = 5; i * i <= n; i += 6) {
                if (n % i == 0 || n % (i + 2) == 0) return false;
            }
        return true;
    };

    for (int p = TABLE_SIZE; p >= 2; --p) {
        if (is_prime(p)) return p;
    }
    return 2;
}();

    static int hash(Value key) noexcept {
        // 处理负数：保证落在 [0, P-1]
        int r = key % P;
        if (r < 0) r += P;
        return r;
    }

public:
    HashTableChaining() = default;

    ~HashTableChaining() {
        clear();
    }

    HashTableChaining(const HashTableChaining&) = delete;
    HashTableChaining& operator=(const HashTableChaining&) = delete;

    void clear() noexcept {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            Node* cur = table_[i];
            while (cur) {
                Node* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
            table_[i] = nullptr;
        }
    }

    // 头插：O(1)
    void insert(Value v) {
        const int idx = hash(v);
        Node* n = new Node{};
        n->value = v;
        n->next = table_[idx];
        table_[idx] = n;
    }

    bool contains(Value v) const noexcept {
        const int idx = hash(v);
        Node* cur = table_[idx];
        while (cur) {
            if (cur->value == v) return true;
            cur = cur->next;
        }
        return false;
    }

    // 删除一个匹配值：成功返回 true
    bool erase(Value v) noexcept {
        const int idx = hash(v);
        Node* cur = table_[idx];
        Node* pre = nullptr;

        while (cur) {
            if (cur->value == v) {
                // 删头 / 删中间统一处理
                if (!pre) table_[idx] = cur->next;
                else pre->next = cur->next;

                delete cur;
                return true;
            }
            pre = cur;
            cur = cur->next;
        }
        return false;
    }

    void print() const {
        std::cout << "TABLE_SIZE=" << TABLE_SIZE << ", p=" << P << "\n";
        for (int i = 0; i < TABLE_SIZE; ++i) {
            std::cout << "Index " << i << ":";
            Node* cur = table_[i];
            while (cur) {
                std::cout << " -> " << cur->value;
                cur = cur->next;
            }
            std::cout << " -> nullptr\n";
        }
    }
};

int main() {
    HashTableChaining ht;

    // 插入
    int a[] = {19, 14, 23, 1, 68, 20, 84, 27, 55, 11, 10, 79};
    constexpr int n = sizeof(a) / sizeof(a[0]);
    for (int i = 0; i < n; ++i) ht.insert(a[i]);

    ht.print();

    // 查找
    std::cout << "Value 20 exists: " << (ht.contains(20) ? "Yes" : "No") << "\n";
    std::cout << "Value 300 exists: " << (ht.contains(300) ? "Yes" : "No") << "\n";
    std::cout << "Value 200 exists: " << (ht.contains(200) ? "Yes" : "No") << "\n";

    // 删除
    int removedValue = 10;
    bool ok = ht.erase(removedValue);
    std::cout << "Value " << removedValue << (ok ? " removed\n" : " not found\n");

    // 再查
    std::cout << "Value " << removedValue << " exists: " << (ht.contains(removedValue) ? "Yes" : "No") << "\n";

    ht.print();
    return 0;
}