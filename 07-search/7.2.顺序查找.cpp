#include <iostream>
#include <memory>

using ElemType = int;

struct SSTable {
    std::unique_ptr<ElemType[]> elem;
    int tableLen{};
};

int Search_Seq_Ordered(const SSTable& st, ElemType key) noexcept
{
    for (int i = 0; i < st.tableLen; ++i)
    {
        if (st.elem[i] == key) {
            return i;
        }
        if (st.elem[i] > key) {
            return -1;
        }
    }
    return -1;
}

int main()
{
    ElemType searchNum = 60;

    SSTable table;
    table.tableLen = 7;
    table.elem = std::make_unique<ElemType[]>(table.tableLen);

    // 有序表（递增）
    table.elem[0] = 20;
    table.elem[1] = 40;
    table.elem[2] = 50;
    table.elem[3] = 60;
    table.elem[4] = 70;
    table.elem[5] = 80;
    table.elem[6] = 90;

    int idx = Search_Seq_Ordered(table, searchNum);

    if (idx == -1) {
        std::cout << "未查找到值：" << searchNum << '\n';
    } else {
        std::cout << "查找到值：" << searchNum
                  << "，下标为：" << idx << '\n';
    }

    return 0;
}