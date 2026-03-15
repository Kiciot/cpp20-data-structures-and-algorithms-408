#include <iostream>
#include <memory>

using ElemType = int;

// ===== 顺序表 =====
struct SSTable {
    std::unique_ptr<ElemType[]> elem;
    int tableLen{};
};

// ===== 索引项 =====
struct IndexItem {
    ElemType maxKey{}; // 该块最大关键字
    int start{};       // 该块起始下标
    int len{};         // 该块长度
};

struct IndexTable {
    std::unique_ptr<IndexItem[]> idx;
    int blockCount{};
    int blockSize{};
};

// 建立索引：按 blockSize 分块，记录每块 maxKey / start / len
IndexTable BuildIndex(const SSTable& st, int blockSize)
{
    IndexTable it;
    it.blockSize = blockSize;

    const int n = st.tableLen;
    it.blockCount = (n + blockSize - 1) / blockSize;
    it.idx = std::make_unique<IndexItem[]>(it.blockCount);

    for (int b = 0; b < it.blockCount; ++b) {
        const int start = b * blockSize;
        const int end   = (start + blockSize < n) ? (start + blockSize) : n;
        const int len   = end - start;

        ElemType mx = st.elem[start];
        for (int i = start + 1; i < end; ++i) {
            if (st.elem[i] > mx) mx = st.elem[i];
        }

        it.idx[b].maxKey = mx;
        it.idx[b].start  = start;
        it.idx[b].len    = len;
    }

    return it; // 返回值优化/移动：不会复制大数组
}

// 分块查找：先在索引表定位块，再块内顺序查找
// 返回：找到返回下标，否则 -1
int BlockSearch(const SSTable& st, const IndexTable& it, ElemType key) noexcept
{
    // 1) 在索引表中找第一个 maxKey >= key 的块
    int block = -1;
    for (int b = 0; b < it.blockCount; ++b) {
        if (key <= it.idx[b].maxKey) {
            block = b;
            break;
        }
    }
    if (block == -1) return -1; // key 比所有块最大值都大，必不在表中

    // 2) 在该块内顺序查找
    const int start = it.idx[block].start;
    const int end   = start + it.idx[block].len;

    for (int i = start; i < end; ++i) {
        if (st.elem[i] == key) return i;
    }
    return -1;
}

void PrintIndex(const IndexTable& it)
{
    std::cout << "IndexTable (blockCount=" << it.blockCount
              << ", blockSize=" << it.blockSize << ")\n";
    std::cout << "block  start  len  maxKey\n";
    for (int b = 0; b < it.blockCount; ++b) {
        std::cout << b << "      "
                  << it.idx[b].start << "      "
                  << it.idx[b].len << "    "
                  << it.idx[b].maxKey << "\n";
    }
}

int main()
{
    // ===== 示例数据：按“块有序”组织 =====
    // 块0: (<= 18)   块1: (<= 35)   块2: (<= 60)
    // 注意：块内可以无序
    ElemType a[] = { 12, 18,  5,   30, 22, 35,   60, 41, 50 };

    SSTable st;
    st.tableLen = static_cast<int>(sizeof(a) / sizeof(a[0]));
    st.elem = std::make_unique<ElemType[]>(st.tableLen);
    for (int i = 0; i < st.tableLen; ++i) st.elem[i] = a[i];

    const int blockSize = 3;
    IndexTable it = BuildIndex(st, blockSize);
    PrintIndex(it);

    for (ElemType key : {35, 41, 7, 60, 100}) {
        int pos = BlockSearch(st, it, key);
        if (pos == -1) {
            std::cout << "key=" << key << " not found\n";
        } else {
            std::cout << "key=" << key << " found at index " << pos << "\n";
        }
    }

    return 0;
}