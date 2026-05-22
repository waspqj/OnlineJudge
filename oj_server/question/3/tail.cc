#ifndef COMPILER_ONLINE
#include "header.cpp"
#endif

void Test1() {
    vector<int> nums = {2, 7, 11, 15};
    vector<int> ret = Solution().twoSum(nums, 9);
    if (ret.size() == 2 && 
        ((ret[0] == 0 && ret[1] == 1) || (ret[0] == 1 && ret[1] == 0))) {
        cout << "通过用例1, 测试 [2,7,11,15] target=9 通过 ... OK!" << endl;
    } else {
        cout << "没有通过用例1, 测试的值是: [2,7,11,15] target=9" << endl;
    }
}

void Test2() {
    vector<int> nums = {3, 2, 4};
    vector<int> ret = Solution().twoSum(nums, 6);
    if (ret.size() == 2 && 
        ((ret[0] == 1 && ret[1] == 2) || (ret[0] == 2 && ret[1] == 1))) {
        cout << "通过用例2, 测试 [3,2,4] target=6 通过 ... OK!" << endl;
    } else {
        cout << "没有通过用例2, 测试的值是: [3,2,4] target=6" << endl;
    }
}

void Test3() {
    vector<int> nums = {3, 3};
    vector<int> ret = Solution().twoSum(nums, 6);
    if (ret.size() == 2 && 
        ((ret[0] == 0 && ret[1] == 1) || (ret[0] == 1 && ret[1] == 0))) {
        cout << "通过用例3, 测试 [3,3] target=6 通过 ... OK!" << endl;
    } else {
        cout << "没有通过用例3, 测试的值是: [3,3] target=6" << endl;
    }
}

int main() {
    Test1();
    Test2();
    Test3();
    return 0;
}