#include <iostream>
#include <string>
#include <cctype>

constexpr int MAX_EXPR_SIZE = 100;

inline int precedence(char op)
{
    switch(op)
    {
        case '^':return 3;
        case '*': case '/': return 2;
        case '+': case '-': return 1;
        default: return -1;
    }
}

inline bool isOperator(char c)
{
    return c=='+' || c=='-' || c=='*' || c=='/' || c=='^';
}

inline bool isRightAssoc(char op)
{
    return op == '^';
}

bool infixToPostfix (const std::string& infix, std::string& postfix)
{
    char st[MAX_EXPR_SIZE];
    int top = -1;
    postfix.clear();

    const int n = static_cast<int>(infix.size());

    for(int i = 0; i < n;)
    {
        char c = infix[i];
        if(std::isspace(static_cast<unsigned char>(c)))
        {
            ++i;
            continue;
        }
        if(std::isalnum(static_cast<unsigned char>(c)) || c == '_')
        {
            int j = i;
            while(j < n)
            {
                char t = infix[j];
                if (std::isalnum(static_cast<unsigned char>(t)) || t == '_') ++j;
                else break;
            }

            postfix.append(infix.substr(i, j - i));
            postfix.push_back(' ');
            i = j;
            continue;
        }

        if(c == '(')
        {
            if (top + 1 >= MAX_EXPR_SIZE) return false;
            st[++top] = c;
            ++i;
            continue;
        }

        if (c == ')')
        {
            while(top >= 0 && st[top] != '(')
            {
                postfix.push_back(st[top--]);
                postfix.push_back(' ');
            }
            if (top < 0 || st[top] != '(') return false;
            --top;
            ++i;
            continue;
        }

        if(isOperator(c))
        {
            int pc = precedence(c);
            while(top >= 0 && isOperator(st[top]))
            {
                int pt = precedence(st[top]);

                if(pt > pc || (pt == pc && !isRightAssoc(c)))
                {
                    postfix.push_back(st[top--]);
                    postfix.push_back(' ');
                }else break;
            }

            if(top +1 >= MAX_EXPR_SIZE) return false;
            st[++top] = c;
            ++i;
            continue;
        }
        return false;
    }

    while (top >= 0)
    {
        if (st[top] == '(') return false;
        postfix.push_back(st[top--]);
        postfix.push_back(' ');
    }

    if(!postfix.empty() && postfix.back() == ' ') postfix.pop_back();
    return true;
}

static inline void swapParens(std::string& s)
{
    for(char& c : s)
    {
        if(c == '(') c = ')';
        else if(c == ')') c = '(';
    }
}

static inline std::string reverseTokens(const std::string& s)
{
    std::string out;
    int i = static_cast<int>(s.size()) - 1;
    while (i >= 0)
    {
        while(i >= 0 && s[i] == ' ') --i;
        if (i < 0) break;
        int end = i;
        while(i >= 0 && s[i] != ' ') --i;
        int start = i + 1;
        if(!out.empty()) out.push_back(' ');
        out.append(s.substr(start, end - start + 1));
    }
    return out;
}

bool infixToPrefix(std::string infix, std::string& prefix)
{
    std::reverse(infix.begin(), infix.end());

    swapParens(infix);

    std::string tempPost;
    if(!infixToPostfix(infix, tempPost)) return false;

    prefix = reverseTokens(tempPost);
    return true;
}

int main() {
    // 你可以试试这两组：带字母，带多位数字
    std::string infix = "A + B * (C - D) - E / F";
    // std::string infix = "((15/(7-(1+1)))*3)-(2+(1+1))";

    std::cout << "中缀表达式: " << infix << "\n";

    std::string postfix, prefix;
    if (infixToPostfix(infix, postfix)) {
        std::cout << "后缀表达式(左优先): " << postfix << "\n";
    } else {
        std::cout << "后缀转换失败：括号或表达式非法\n";
    }

    if (infixToPrefix(infix, prefix)) {
        std::cout << "前缀表达式(右优先): " << prefix << "\n";
    } else {
        std::cout << "前缀转换失败：括号或表达式非法\n";
    }
    return 0;
}