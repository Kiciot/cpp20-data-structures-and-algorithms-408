#include <iostream>
#include <memory>

using ElemType = int;

struct SSTable
{
    std::unique_ptr<ElemType[]> elem;
    int tableLen{};
};

int Binary_Search(const SSTable& st, ElemType key) noexcept
{
    int low = 0;
    int high = st.tableLen - 1;
    while(low <= high)
    {
        int mid = low + (high - low) / 2;
        if(st.elem[mid] == key) 
        {
            return mid;
        }
        else if (st.elem[mid] > key)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return -1;
}

int main()
{
    ElemType searchNum = 100;

    SSTable table;
    table.tableLen = 7;
    table.elem = std::make_unique<ElemType[]>(table.tableLen);

    // 递增有序表
    table.elem[0] = 20;
    table.elem[1] = 40;
    table.elem[2] = 50;
    table.elem[3] = 60;
    table.elem[4] = 70;
    table.elem[5] = 80;
    table.elem[6] = 90;

    int index = Binary_Search(table, searchNum);

    if (index == -1) {
        std::cout << "未查找到值：" << searchNum << '\n';
    } else {
        std::cout << "查找到值：" << searchNum
                  << "，下标为：" << index << '\n';
    }

    return 0;
}

int main()
{
    ElemType searchNum = 100;

    SSTable table;
    table.tableLen = 7;
    table.elem = std::make_unique<ElemType[]>(table.tableLen);

    // 递增有序表
    table.elem[0] = 20;
    table.elem[1] = 40;
    table.elem[2] = 50;
    table.elem[3] = 60;
    table.elem[4] = 70;
    table.elem[5] = 80;
    table.elem[6] = 90;

    int index = Binary_Search(table, searchNum);

    if (index == -1) {
        std::cout << "未查找到值：" << searchNum << '\n';
    } else {
        std::cout << "查找到值：" << searchNum
                  << "，下标为：" << index << '\n';
    }

    return 0;
}