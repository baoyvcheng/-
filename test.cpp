#include "gtest/gtest.h"
#include "pch.h"
#include <iostream>
#include <vector>
using namespace std;

// 最大子数组和
int max_subarray_sum(vector<int>& arr) {
    int n = arr.size();
    int max_sum = 0;
    int current_sum = 0;

    for (int i = 0; i < n; ++i) {
        current_sum = max(0, current_sum + arr[i]);
        max_sum = max(max_sum, current_sum);
    }

    return max_sum;
}

// 单元测试
// 处理 max_subarray_sum 函数中空数组的测试用例。
TEST(MaxSubArraySumTest, HandlesEmptyArray) {
    
    std::vector<int> nums = {};
    EXPECT_EQ(max_subarray_sum(nums), 0);
}
// 处理所有负数的测试用例。
TEST(MaxSubArraySumTest, HandlesAllNegativeNumbers) {
    
    std::vector<int> nums = { -1, -2, -3, -4 };
    EXPECT_EQ(max_subarray_sum(nums), 0);
}
// 处理混合正负数的测试用例。
TEST(MaxSubArraySumTest, HandlesMixedPositiveAndNegativeNumbers) {
    
    std::vector<int> nums = { -2, 11, -4, 13, -5, -2 };
    EXPECT_EQ(max_subarray_sum(nums), 20);
}
// 处理所有正数的测试用例。
TEST(MaxSubArraySumTest, HandlesAllPositiveNumbers) {
   
    std::vector<int> nums = { 1, 2, 3, 4 };
    EXPECT_EQ(max_subarray_sum(nums), 10);
}
// 处理单个元素的测试用例。
TEST(MaxSubArraySumTest, HandlesSingleElementArray) {
    
    std::vector<int> nums = { -5 };
    EXPECT_EQ(max_subarray_sum(nums), 0);

    
    nums = { 5 };
    EXPECT_EQ(max_subarray_sum(nums), 5);
}
// 处理单个零元素的测试用例。
TEST(MaxSubArraySumTest, HandlesSingleZeroElementArray) {
    
    std::vector<int> nums = { 0 };
    EXPECT_EQ(max_subarray_sum(nums), 0);
}
// 处理所有零元素的测试用例。
TEST(MaxSubArraySumTest, HandlesAllZerosArray) {
    
    std::vector<int> nums = { 0, 0, 0, 0 };
    EXPECT_EQ(max_subarray_sum(nums), 0);
}


//Google Test主程序
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}