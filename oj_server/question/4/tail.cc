#ifndef COMPILER_ONLINE
#include "header.cpp"
#endif

void Test1() {
    vector<char> s = {'h', 'e', 'l', 'l', 'o'};
    vector<char> expected = {'o', 'l', 'l', 'e', 'h'};
    Solution().reverseString(s);
    if (s == expected) {
        cout << "通过用例1, 测试 ['h','e','l','l','o'] 通过 ... OK!" << endl;
    } else {
        cout << "没有通过用例1, 测试的值是: ['h','e','l','l','o']" << endl;
    }
}

void Test2() {
    vector<char> s = {'H', 'a', 'n', 'n', 'a', 'h'};
    vector<char> expected = {'h', 'a', 'n', 'n', 'a', 'H'};
    Solution().reverseString(s);
    if (s == expected) {
        cout << "通过用例2, 测试 ['H','a','n','n','a','h'] 通过 ... OK!" << endl;
    } else {
        cout << "没有通过用例2, 测试的值是: ['H','a','n','n','a','h']" << endl;
    }
}

void Test3() {
    vector<char> s = {'A'};
    vector<char> expected = {'A'};
    Solution().reverseString(s);
    if (s == expected) {
        cout << "通过用例3, 测试 ['A'] 通过 ... OK!" << endl;
    } else {
        cout << "没有通过用例3, 测试的值是: ['A']" << endl;
    }
}

int main() {
    Test1();
    Test2();
    Test3();
    return 0;
}