/**
 * test_end_with.c
 *
 * @author : 通义千问
 * @date   : 2025/05/14 22:10
 * @brief  : test_end_with.c
 */
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../include/util.h"

// 测试用例：基本功能测试
void test_end_with_basic() {
    CU_ASSERT_TRUE(end_with("hello world", "world"));
    CU_ASSERT_FALSE(end_with("hello world", "hello"));
    CU_ASSERT_TRUE(end_with("test.txt", ".txt"));
    CU_ASSERT_FALSE(end_with("test.txt", ".png"));
}

// 测试用例：空字符串测试
void test_end_with_empty() {
    CU_ASSERT_TRUE(end_with("", ""));  // 空字符串以空字符串结尾
    CU_ASSERT_TRUE(end_with("test", ""));  // 任何字符串都以空字符串结尾
    CU_ASSERT_FALSE(end_with("", "test"));  // 空字符串不以非空字符串结尾
}

// 测试用例：长度相等的字符串测试
void test_end_with_equal_length() {
    CU_ASSERT_TRUE(end_with("same", "same"));  // 字符串与后缀相同
    CU_ASSERT_FALSE(end_with("different", "diff"));  // 部分匹配但不完全
}

// 测试用例：长后缀测试
void test_end_with_long_suffix() {
    CU_ASSERT_FALSE(end_with("short", "longer"));  // 后缀比字符串长
    CU_ASSERT_FALSE(end_with("exactsize", "exactsize1"));  // 后缀仅比字符串长1个字符
}

int main() {
    // 初始化CUnit测试套件
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // 添加测试用例到测试套件
    CU_pSuite pSuite = CU_add_suite("end_with_Test_Suite", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuite, "test_end_with_basic", test_end_with_basic)) ||
        (NULL == CU_add_test(pSuite, "test_end_with_empty", test_end_with_empty)) ||
        (NULL == CU_add_test(pSuite, "test_end_with_equal_length", test_end_with_equal_length)) ||
        (NULL == CU_add_test(pSuite, "test_end_with_long_suffix", test_end_with_long_suffix))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // 运行所有测试
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    // 清理注册表并返回结果
    CU_cleanup_registry();
    return CU_get_error();
}