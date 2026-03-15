#include <iostream>
#include <memory>

using ElemType = int;

struct SSTable {
    std::unique_ptr<ElemType[]> elem;
    int tableLen{};
};

int Sentinel_Search_Seq(SSTable& st, ElemType key) noexcept
{
    st.elem[0] = key;
    int i = st.tableLen;
    while(st.elem[i] != key) -- i;
    return i;
}

int Search_Seq(const SSTable& st, ElemType key) noexcept
{
    for (int i = 0; i < st.tableLen; ++i)
    {
        if(st.elem[i] = key) return i;
    }
    return -1;
}

int main()
{
    const ElemType searchNum = 60;
    int idx = -1;

    // ===== 带哨兵表：下标 0 是哨兵，有效数据从 1..tableLen =====
    SSTable sentinel{};
    sentinel.tableLen = 7;
    sentinel.elem = std::make_unique<ElemType[]>(sentinel.tableLen + 1);

    sentinel.elem[0] = -1; // 哨兵位（会被查找时覆盖成 key）
    sentinel.elem[1] = 40;
    sentinel.elem[2] = 20;
    sentinel.elem[3] = 50;
    sentinel.elem[4] = 10;
    sentinel.elem[5] = 60;
    sentinel.elem[6] = 80;
    sentinel.elem[7] = 70;

    idx = Sentinel_Search_Seq(sentinel, searchNum);
    if (idx == 0) {
        std::cout << "哨兵版未查找到值：" << searchNum << "，结果=" << idx << '\n';
    } else {
        std::cout << "哨兵版查找值：" << searchNum << "，下标=" << idx << '\n';
    }

    // ===== 普通表：下标 0..tableLen-1 =====
    SSTable normal{};
    normal.tableLen = 7;
    normal.elem = std::make_unique<ElemType[]>(normal.tableLen);

    normal.elem[0] = 40;
    normal.elem[1] = 20;
    normal.elem[2] = 50;
    normal.elem[3] = 10;
    normal.elem[4] = 60;
    normal.elem[5] = 80;
    normal.elem[6] = 70;

    idx = Search_Seq(normal, searchNum);
    if (idx == -1) {
        std::cout << "普通版未查找到值：" << searchNum << "，结果=" << idx << '\n';
    } else {
        std::cout << "普通版查找值：" << searchNum << "，下标=" << idx << '\n';
    }

    return 0;
}