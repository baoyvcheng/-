#include "gtest/gtest.h"
#include "pch.h"
#include <iostream>
#include <vector>
using namespace std;

// ����������
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

// ��Ԫ����
// ���� max_subarray_sum �����п�����Ĳ���������
TEST(MaxSubArraySumTest, HandlesEmptyArray) {
    
    std::vector<int> nums = {};
    EXPECT_EQ(max_subarray_sum(nums), 0);
}
// �������и����Ĳ���������
TEST(MaxSubArraySumTest, HandlesAllNegativeNumbers) {
    
    std::vector<int> nums = { -1, -2, -3, -4 };
    EXPECT_EQ(max_subarray_sum(nums), 0);
}
// �������������Ĳ���������
TEST(MaxSubArraySumTest, HandlesMixedPositiveAndNegativeNumbers) {
    
    std::vector<int> nums = { -2, 11, -4, 13, -5, -2 };
    EXPECT_EQ(max_subarray_sum(nums), 20);
}
// �������������Ĳ���������
TEST(MaxSubArraySumTest, HandlesAllPositiveNumbers) {
   
    std::vector<int> nums = { 1, 2, 3, 4 };
    EXPECT_EQ(max_subarray_sum(nums), 10);
}
// ������Ԫ�صĲ���������
TEST(MaxSubArraySumTest, HandlesSingleElementArray) {
    
    std::vector<int> nums = { -5 };
    EXPECT_EQ(max_subarray_sum(nums), 0);

    
    nums = { 5 };
    EXPECT_EQ(max_subarray_sum(nums), 5);
}
// ��������Ԫ�صĲ���������
TEST(MaxSubArraySumTest, HandlesSingleZeroElementArray) {
    
    std::vector<int> nums = { 0 };
    EXPECT_EQ(max_subarray_sum(nums), 0);
}
// ����������Ԫ�صĲ���������
TEST(MaxSubArraySumTest, HandlesAllZerosArray) {
    
    std::vector<int> nums = { 0, 0, 0, 0 };
    EXPECT_EQ(max_subarray_sum(nums), 0);
}


//Google Test������
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}