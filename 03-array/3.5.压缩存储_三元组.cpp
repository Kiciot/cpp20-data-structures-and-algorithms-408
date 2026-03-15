#include <iostream>

struct Triplet
{
    int i;
    int j;
    int v;
};

int main () 
{
    constexpr int n = 5;
    int A[n][n] = {
        {  1,  5,  0, 22, 0},
        {  7,  0, 33,  0, 0},
        {  0, 55,  0,  9, 0},
        {  0,  0,  0, 18, 0},
        { 16,  0, 17,  0, 0}
    };

    int nz = 0;
    for (int r = 0; r < n; ++r)
        for(int c = 0; c < n; ++c)
            if (A[r][c] != 0) ++nz;
    
    Triplet* B = new Triplet[nz];

    int k = 0;
    for(int r = 0; r < n; ++r)
    {
        for(int c = 0 ; c < n; ++c)
        {
            if(A[r][c] != 0)
            {
                B[k++] = Triplet{r, c, A[r][c]};
            }
        }
    }

        std::cout << "Ñ¹ËõºóµÄ¾ØÕóB£º\n"
                << "i\tj\tv\n";
    for (int t = 0; t < nz; ++t) {
        std::cout << B[t].i << '\t'
                << B[t].j << '\t'
                << B[t].v << '\n';
    }

    delete[] B;
    return 0;
}