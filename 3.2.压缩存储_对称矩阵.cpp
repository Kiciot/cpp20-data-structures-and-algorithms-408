#include <iostream>
#include <vector>

#define DIRECTION 1

int main()
{
    std::vector<std::vector<char>> A  = {
        {'a', 'b', 'b', },
        {'b', 'a', 'b', },
        {'b', 'b', 'a', }
    };

    std::vector<char> B;
    for(size_t i = 0; i < A.size(); ++i)
    {
        for(size_t j = 0; i < A[i].size(); ++j)
        {
            int k;

            if (DIRECTION == 1 && i < j)
            {
                continue;
            }

            if (DIRECTION == 2 && i > j)
            {
                continue;
            }

            if(i >= j)
            {
                k = i * (i + 1) / 2 + j;
            }else
            {
                k = j * (j + 1) / 2 + i;
            }
            
            if (k >= B.size())
            {
                B.reserve(k + 1);
            }
            B[k] = A[i][j];

        }
    }

    std::cout << "Ñ¹ËõºóµÄ¾ØÕóB£º" << std::endl;
    for (const auto& val : B) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;

}