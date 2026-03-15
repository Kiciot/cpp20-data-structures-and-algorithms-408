#include <iostream>
#include <string>
#include <string_view>
#include <vector>

std::vector<int> build_next(std::string_view pat)
{
    const int m = static_cast<int>(pat.size());
    std::vector<int> next(m, 0);
    int j = 0;

    for (int i = 1; i < m ; ++i)
    {
        while(j > 0 && pat[i] != pat[j])
        {
            j = next[j - 1];
        }
        if(pat[i] == pat[j])
        {
            ++j;
        }
        next[i] = j;
    }
    return next;
}

std::vector<int> build_nextval(std::string_view pat)
{
    const int m = static_cast<int>(pat.size());
    if (m == 0) return {};
    
    auto next = build_next(pat);
    std::vector<int> nextval(m, 0);

    nextval[0] = 0;
    for(int i = 1; i < m; ++i)
    {
        if(pat[i] != pat[next[i] - 1] || next[i] == 0)
        {
            nextval[i] = next[i];
        }
        else
        {
            nextval[i] = nextval[next[i] - 1];
        }
    }
    return nextval;
}

int kmp_index_0(std::string_view text, std::string_view pat)
{
    const int n = static_cast<int>(text.size());
    const int m = static_cast<int>(pat.size());

    if(m == 0) return 0; //空模式

    auto next = build_next(pat);
    int j = 0;

    for (int i = 0; i < n; ++i)
    {
        while(j > 0 && text[i] != pat[j])
        {
            j = next[j - 1];
        }
        if(text[i] == pat[j])
        {
            ++j;
        }
        if(j == m)
        {
            return i - m + 1;
        }
    }
    return -1;
}

int kmp_index_1(std::string_view text, std::string_view pat) 
{
    int pos0 = kmp_index_0(text, pat);
    return (pos0 == -1) ? 0 : pos0 + 1;  // 找不到返回 0，找到返回 1 基
}

int main() {
    // 对应你原来的例子
    std::string S  = "ababcabcacbab";
    std::string T  = "abcac";

    int pos1 = kmp_index_1(S, T);
    if (pos1 != 0)
        std::cout << "下标从1开始的匹配位置: " << pos1 << '\n';
    else
        std::cout << "下标从1开始未找到\n";

    std::string S1 = "aaabc";
    std::string T1 = "ab";

    int pos0 = kmp_index_0(S1, T1);
    if (pos0 != -1)
        std::cout << "下标从0开始的匹配位置: " << pos0 << '\n';
    else
        std::cout << "下标从0开始未找到\n";

    return 0;
}