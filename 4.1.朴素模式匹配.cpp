#include <iostream>
#include <string>
#include <string_view>
constexpr int Index1(const std::string_view& S, const std::string_view& T) noexcept
{
    if(T.empty()) return 1;
    std::size_t i = 0, j = 0;
    while(i < S.size() && j < T.size())
    {
        if(S[i] == T[j])
        {
            ++i;
            ++j;
        }
        else
        {
            i = i - j + 1;
            j = 0;
        }
    }
    return (j == T.size()) ? static_cast<int>(i - j + 1) : 0;
}

constexpr int Index0(const std::string_view& S, const std::string_view& T) noexcept
{
    if(T.empty()) return 0;
    std::size_t i = 0, j = 0;
    while(i < S.size() && j < T.size())
    {
        if(S[i] == T[j])
        {
            ++i;
            ++j;
        }
        else
        {
            i = i - j + 1;
            j = 0;
        }
    }
    return (j == T.size()) ? static_cast<int>(i - j) : -1;
}

int main() {
    // 与你原先的示例等价的字符串
    std::string S = "ababcabcacbab";
    std::string T = "abcac";

    int pos1 = Index1(S, T);
    if (pos1 != 0) std::cout << "下标从1开始的匹配位置: " << pos1 << '\n';
    else           std::cout << "下标从1开始未找到\n";

    std::string S1 = "aaabc";
    std::string T1 = "ab";

    int pos0 = Index0(S1, T1);
    if (pos0 != -1) std::cout << "下标从0开始的匹配位置: " << pos0 << '\n';
    else            std::cout << "下标从0开始未找到\n";

    return 0;
}