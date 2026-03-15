#include <iostream>
#include <array>

int main()
{
    std::array<std::array<int, 3>, 3> A =
    {
        {
            {0, 1, 2, },
            {10, 11, 12, },
            {20, 21, 22,}
        }
    };


for (size_t row = 0; row < A.size(); ++row)
{
    for(size_t col = 0; col < A[row].size(); ++col)
    {
        std::cout << "A[" << row << "][" << col << "] = " << A[row][col] << "\t";
    }
    std::cout << '\n';
}

//单数组模拟二维数组
const int rows = 3;
const int cols = 3;
int B[rows * cols] = {0, 1, 2, 10, 11, 12, 20, 21, 22, };

for(int row = 0; row < rows; ++row)
{
    for(int col = 0; col < cols; ++col)
    {
        int index = row * cols + col;
        std::cout << "A[" << row <<'][' << col << "] = " << B[index] << '\t';
    }
    std::cout << '\n';
}
return 0;
}