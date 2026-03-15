#include <iostream>

class OpenAddressHash {
public:
    enum class ProbeType { Linear, Quadratic };

private:
    enum class State : unsigned char { Empty, Occupied, Deleted };

    int  size_{0};      // 表长 m
    int  p_{0};         // 不大于 m 的最大质数（用于 key % p）
    int* table_{nullptr};
    State* state_{nullptr};
    ProbeType probe_{ProbeType::Linear};

private:
    static bool is_prime(int n) noexcept {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

    static int prev_prime_leq(int m) noexcept {
        for (int p = m; p >= 2; --p) {
            if (is_prime(p)) return p;
        }
        return 2;
    }

    static int mod_norm(int a, int m) noexcept {
        int r = a % m;
        if (r < 0) r += m;
        return r;
    }

    int h(int key) const noexcept {
        // 除留余数法：key mod p（p 为质数）
        return mod_norm(key, p_);
    }

    // 计算第 i 次探测（i 从 0 开始）的位置
    int probe_index(int base, int i) const noexcept {
        if (probe_ == ProbeType::Linear) {
            // base, base+1, base+2, ...
            return (base + i) % size_;
        }

        // Quadratic（常见教材写法：±1^2, ±2^2...）
        // 为了不额外存储序列，这里用 i 映射到序号 k 与符号：
        // i=0 -> 0
        // i=1 -> +1^2
        // i=2 -> -1^2
        // i=3 -> +2^2
        // i=4 -> -2^2 ...
        if (i == 0) return base;

        int k = (i + 1) / 2;          // 1,1,2,2,3,3...
        int sign = (i % 2 == 1) ? +1 : -1;
        long long offset = 1LL * sign * k * k;

        int idx = base + static_cast<int>(offset % size_);
        idx %= size_;
        if (idx < 0) idx += size_;
        return idx;
    }

public:
    OpenAddressHash(int size, ProbeType probe)
        : size_(size), p_(prev_prime_leq(size)), probe_(probe)
    {
        table_ = new int[size_]{};
        state_ = new State[size_]{};
        for (int i = 0; i < size_; ++i) state_[i] = State::Empty;
    }

    ~OpenAddressHash() {
        delete[] table_;
        delete[] state_;
        table_ = nullptr;
        state_ = nullptr;
    }

    OpenAddressHash(const OpenAddressHash&) = delete;
    OpenAddressHash& operator=(const OpenAddressHash&) = delete;

    int size() const noexcept { return size_; }
    int p() const noexcept { return p_; }

    // 插入：成功 true；如果表满或已存在（可按需改）返回 false
    bool insert(int value) noexcept {
        const int base = h(value);
        int firstDeleted = -1;

        // 最多探测 size_ 次
        for (int i = 0; i < size_; ++i) {
            const int idx = probe_index(base, i);

            if (state_[idx] == State::Occupied) {
                if (table_[idx] == value) return false; // 去重（可选）
                continue;
            }

            if (state_[idx] == State::Deleted) {
                if (firstDeleted == -1) firstDeleted = idx;
                continue;
            }

            // Empty：可以插入
            if (firstDeleted != -1) {
                table_[firstDeleted] = value;
                state_[firstDeleted] = State::Occupied;
            } else {
                table_[idx] = value;
                state_[idx] = State::Occupied;
            }
            return true;
        }

        // 没找到 Empty，但可能有 Deleted
        if (firstDeleted != -1) {
            table_[firstDeleted] = value;
            state_[firstDeleted] = State::Occupied;
            return true;
        }
        return false; // 表满
    }

    // 查找：返回下标，找不到返回 -1
    int find(int value) const noexcept {
        const int base = h(value);

        for (int i = 0; i < size_; ++i) {
            const int idx = probe_index(base, i);

            if (state_[idx] == State::Empty) {
                // 遇到 Empty 可以直接判定“后面也不可能有”
                return -1;
            }

            if (state_[idx] == State::Occupied && table_[idx] == value) {
                return idx;
            }

            // Deleted 或 Occupied(但不同值)：继续探测
        }
        return -1;
    }

    // 删除：成功 true，失败 false
    bool erase(int value) noexcept {
        int idx = find(value);
        if (idx == -1) return false;
        state_[idx] = State::Deleted;
        return true;
    }

    void print() const {
        std::cout << "HashTable(m=" << size_ << ", p=" << p_ << ")\n";
        for (int i = 0; i < size_; ++i) {
            std::cout << "[" << i << "] ";
            if (state_[i] == State::Empty) std::cout << "EMPTY";
            else if (state_[i] == State::Deleted) std::cout << "DELETED";
            else std::cout << table_[i];
            std::cout << "\n";
        }
    }
};

int main() {
    // 线性探测
    OpenAddressHash htLinear(11, OpenAddressHash::ProbeType::Linear);
    int a1[] = {87, 40, 30, 6, 11, 22, 98, 20};
    for (int x : a1) htLinear.insert(x);

    std::cout << "Linear probing:\n";
    htLinear.print();

    // 二次探测
    OpenAddressHash htQuad(27, OpenAddressHash::ProbeType::Quadratic);
    int a2[] = {6, 19, 32, 45, 58, 71, 84};
    for (int x : a2) htQuad.insert(x);

    std::cout << "\nQuadratic probing:\n";
    htQuad.print();

    std::cout << "\nfind(84) = " << htQuad.find(84) << "\n";

    // 演示删除对查找的影响（有 DELETED 才不会断）
    htQuad.erase(45);
    std::cout << "\nAfter erase(45):\n";
    htQuad.print();
    std::cout << "find(84) = " << htQuad.find(84) << "\n";

    return 0;
}