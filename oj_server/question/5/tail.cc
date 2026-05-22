#ifndef COMPILER_ONLINE
#include "header.cpp"
#endif

void Test1() {
    int ret = Solution().fib(2);
    if (ret == 1) {
        cout << "通过用例1, 测试 fib(2)=1 通过 ... OK!" << endl;
    } else {
        cout << "没有通过用例1, 测试的值是: 2, 预期 1, 实际 " << ret << endl;
    }
}

void Test2() {
    int ret = Solution().fib(3);
    if (ret == 2) {
        cout << "通过用例2, 测试 fib(3)=2 通过 ... OK!" << endl;
    } else {
        cout << "没有通过用例2, 测试的值是: 3, 预期 2, 实际 " << ret << endl;
    }
}

void Test3() {
    int ret = Solution().fib(4);
    if (ret == 3) {
        cout << "通过用例3, 测试 fib(4)=3 通过 ... OK!" << endl;
    } else {
        cout << "没有通过用例3, 测试的值是: 4, 预期 3, 实际 " << ret << endl;
    }
}

int main() {
    Test1();
    Test2();
    Test3();
    return 0;
}