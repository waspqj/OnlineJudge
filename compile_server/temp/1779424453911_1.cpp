#include <iostream>
#include <string>
#include <stack>
#include <unistd.h>
using namespace std;

class Solution {
public:
    bool isValid(string s) {
        // 将你的代码写在下面
        // code
        // code
        // code
        // code
        // code
        sleep(1000);
        return false;
    }
};
#ifndef COMPILER_ONLINE
#include "header.cpp"
#endif

void Test1() {
    bool ret = Solution().isValid("()");
    if (ret) {
        cout << "通过用例1, 测试 \"()\" 通过 ... OK!" << endl;
    } else {
        cout << "没有通过用例1, 测试的值是: \"()\"" << endl;
    }
}

void Test2() {
    bool ret = Solution().isValid("()[]{}");
    if (ret) {
        cout << "通过用例2, 测试 \"()[]{}\" 通过 ... OK!" << endl;
    } else {
        cout << "没有通过用例2, 测试的值是: \"()[]{}\"" << endl;
    }
}

void Test3() {
    bool ret = Solution().isValid("(]");
    if (!ret) {
        cout << "通过用例3, 测试 \"(]\" 通过 ... OK!" << endl;
    } else {
        cout << "没有通过用例3, 测试的值是: \"(]\"" << endl;
    }
}

void Test4() {
    bool ret = Solution().isValid("([)]");
    if (!ret) {
        cout << "通过用例4, 测试 \"([)]\" 通过 ... OK!" << endl;
    } else {
        cout << "没有通过用例4, 测试的值是: \"([)]\"" << endl;
    }
}

void Test5() {
    bool ret = Solution().isValid("");
    if (ret) {
        cout << "通过用例5, 测试 \"\" 通过 ... OK!" << endl;
    } else {
        cout << "没有通过用例5, 测试的值是: \"\"" << endl;
    }
}

int main() {
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    return 0;
}