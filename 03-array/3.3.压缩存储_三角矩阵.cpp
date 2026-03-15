#include <iostream>
enum class Direction { Upper = 1, Lower = 2, };

int main()
{
    constexpr int n = 3;
    constexpr Direction DIR = Direction::Lower;

        char A[n][n] = {
        {'a', 'c', 'c'},
        {'b', 'a', 'c'},
        {'b', 'b', 'a'}
    };

    constexpr int TRI_SIZE = n * (n + 1) / 2;
    char B[TRI_SIZE + 1] = {};

    auto idx_lower = [](int i, int j) -> int {
        return i * (i + 1) / 2 + j;
    };

    auto idx_upper = [](int i, int j) -> int {
        return j * (j + 1) / 2 + i;
    };

    for (int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            if(DIR == Direction::Lower)
            {
                if(i < j) continue;
                int k = idx_lower(i, j);
                B[k] = A[i][j];
            }
            else
            {
                if(i > j) continue;
                int k = idx_upper(i, j);
                B[k] = A[i][j];
            }
        }
    }

    if(DIR == Direction::Lower)
    {
        B[TRI_SIZE] = A[0][1];
    }
    else
    {
        B[TRI_SIZE] = A[1][0];
    }

        std::cout << "压缩后的 B（前 " << TRI_SIZE 
                << " 个为选定三角 + 末尾 1 个为另一半常量）:\n";
    for (int i = 0; i < TRI_SIZE + 1; ++i) {
        std::cout << B[i] << (i + 1 == TRI_SIZE + 1 ? '\n' : ' ');
    }
    return 0;
}