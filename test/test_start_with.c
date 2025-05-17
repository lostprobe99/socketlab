/**
 * test_start_with.c
 *
 * @author : 通义千问
 * @date   : 2025/05/14 22:11
 * @brief  : test_start_with.c
 */
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../include/util.h"

void test_start_with() {
    CU_ASSERT_TRUE(start_with("hello world", "hello"));
    CU_ASSERT_FALSE(start_with("hello world", "world"));
    CU_ASSERT_TRUE(start_with("hello", "hello"));
    CU_ASSERT_FALSE(start_with("hi", "hello"));
}

// 测试空字符串情况
void test_start_with_empty_strings() {
    CU_ASSERT_TRUE(start_with("", ""));  // 空字符串以空字符串开头
    CU_ASSERT_TRUE(start_with("test", ""));  // 所有字符串都以空字符串开头
    CU_ASSERT_FALSE(start_with("", "test"));  // 空字符串不能以非空字符串开头
}

// 测试前缀长度等于主字符串的情况
void test_start_with_equal_length() {
    CU_ASSERT_TRUE(start_with("same", "same"));  // 相同字符串
    CU_ASSERT_FALSE(start_with("different", "diff"));  // 部分匹配但不完全
}

// 测试特殊字符
void test_start_with_special_characters() {
    CU_ASSERT_TRUE(start_with("/path/to/file", "/path"));  // 路径字符串
    CU_ASSERT_TRUE(start_with("http://example.com", "http://"));  // URL 前缀
    CU_ASSERT_TRUE(start_with("1234567890", "1234"));  // 数字字符串
}

// 测试长前缀
void test_start_with_long_prefix() {
    CU_ASSERT_FALSE(start_with("short", "longer prefix"));  // 主字符串比前缀短
    CU_ASSERT_FALSE(start_with("exactsize", "exactsize plus more"));  // 前缀仅比主字符串长1个字符
}

int main() {
    CU_initialize_registry();
    
    CU_pSuite suite = CU_add_suite("Test Suite", 0, 0);
    
    CU_add_test(suite, "test_start_with_basic", test_start_with);
    CU_add_test(suite, "test_start_with_empty_strings", test_start_with_empty_strings);
    CU_add_test(suite, "test_start_with_equal_length", test_start_with_equal_length);
    CU_add_test(suite, "test_start_with_special_characters", test_start_with_special_characters);
    CU_add_test(suite, "test_start_with_long_prefix", test_start_with_long_prefix);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    CU_cleanup_registry();
    return 0;
}