/**
 * simple_log_sink_test.c
 *
 * @author : Tongyi Lingma
 * @date   : 2025/09/03 20:40
 * @brief  : Unit tests for simple_log_sink.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "internal/simple_log_sink.h"

// 用于测试的辅助函数
static void reset_sink() {
    slog_set_sink(NULL, NULL, 0);
}

// 测试 slog_get_sink_p 函数
void test_slog_get_sink_p() {
    log_sink_t* sink1 = slog_get_sink_p();
    log_sink_t* sink2 = slog_get_sink_p();
    
    // 验证返回的是同一个实例
    CU_ASSERT_PTR_EQUAL(sink1, sink2);
    CU_ASSERT_PTR_NOT_NULL(sink1);
}

// 测试 slog_get_sink_fp 函数
void test_slog_get_sink_fp() {
    reset_sink();
    
    FILE* fp = slog_get_sink_fp();
    CU_ASSERT_PTR_NULL(fp);
    
    // 测试设置 fp 后的获取
    slog_set_sink_fp(stderr);
    fp = slog_get_sink_fp();
    CU_ASSERT_PTR_EQUAL(fp, stderr);
}

// 测试 slog_get_sink_file 函数
void test_slog_get_sink_file() {
    reset_sink();
    
    char* file = slog_get_sink_file();
    CU_ASSERT_PTR_NULL(file);
    
    // 测试设置文件名后的获取
    const char* test_file = "/tmp/test.log";
    slog_set_sink_file(test_file);
    file = slog_get_sink_file();
    CU_ASSERT_PTR_NOT_NULL(file);
    CU_ASSERT_STRING_EQUAL(file, test_file);
}

// 测试 slog_get_sink_owned 函数
void test_slog_get_sink_owned() {
    reset_sink();
    
    int owned = slog_get_sink_owned();
    CU_ASSERT_EQUAL(owned, 0);
    
    // 测试设置 owned 后的获取
    slog_set_sink_owned(1);
    owned = slog_get_sink_owned();
    CU_ASSERT_EQUAL(owned, 1);
}

// 测试 slog_set_sink_fp 函数
void test_slog_set_sink_fp() {
    reset_sink();
    
    // 测试设置 stdout
    slog_set_sink_fp(stdout);
    FILE* fp = slog_get_sink_fp();
    CU_ASSERT_PTR_EQUAL(fp, stdout);
    
    // 测试设置 stderr
    slog_set_sink_fp(stderr);
    fp = slog_get_sink_fp();
    CU_ASSERT_PTR_EQUAL(fp, stderr);
    
    // 测试设置 NULL
    slog_set_sink_fp(NULL);
    fp = slog_get_sink_fp();
    CU_ASSERT_PTR_NULL(fp);
    
    // 测试 owned 情况下的文件关闭
    reset_sink();
    slog_set_sink_owned(1);
    FILE* test_fp = fopen("/tmp/slog_test.txt", "w");
    CU_ASSERT_PTR_NOT_NULL(test_fp);
    slog_set_sink_fp(test_fp);
    // 设置新的 fp 应该会关闭之前的文件
    slog_set_sink_fp(NULL);
    fp = slog_get_sink_fp();
    CU_ASSERT_PTR_NULL(fp);
}

// 测试 slog_set_sink_file 函数
void test_slog_set_sink_file() {
    reset_sink();
    
    // 测试设置文件名
    const char* test_file = "test.log";
    slog_set_sink_file(test_file);
    char* file = slog_get_sink_file();
    CU_ASSERT_PTR_NOT_NULL(file);
    CU_ASSERT_STRING_EQUAL(file, test_file);
    
    // 测试更改文件名
    const char* new_file = "/tmp/new_test.log";
    slog_set_sink_file(new_file);
    file = slog_get_sink_file();
    CU_ASSERT_PTR_NOT_NULL(file);
    CU_ASSERT_STRING_EQUAL(file, new_file);
    
    // 测试设置 NULL
    slog_set_sink_file(NULL);
    file = slog_get_sink_file();
    CU_ASSERT_PTR_NULL(file);
}

// 测试 slog_set_sink_owned 函数
void test_slog_set_sink_owned() {
    reset_sink();
    
    // 测试设置 owned 为 1
    slog_set_sink_owned(1);
    int owned = slog_get_sink_owned();
    CU_ASSERT_EQUAL(owned, 1);
    
    // 测试设置 owned 为 0
    slog_set_sink_owned(0);
    owned = slog_get_sink_owned();
    CU_ASSERT_EQUAL(owned, 0);
    
    // 测试切换 owned 状态
    slog_set_sink_owned(1);
    owned = slog_get_sink_owned();
    CU_ASSERT_EQUAL(owned, 1);
}

// 测试 slog_set_sink 函数
void test_slog_set_sink() {
    reset_sink();
    
    const char* test_file = "combined_test.log";
    slog_set_sink(stdout, test_file, 1);
    
    FILE* fp = slog_get_sink_fp();
    char* file = slog_get_sink_file();
    int owned = slog_get_sink_owned();
    
    CU_ASSERT_PTR_EQUAL(fp, stdout);
    CU_ASSERT_PTR_NOT_NULL(file);
    CU_ASSERT_STRING_EQUAL(file, test_file);
    CU_ASSERT_EQUAL(owned, 1);
    
    // 测试设置为 NULL
    slog_set_sink(NULL, NULL, 0);
    fp = slog_get_sink_fp();
    file = slog_get_sink_file();
    owned = slog_get_sink_owned();
    
    CU_ASSERT_PTR_NULL(fp);
    CU_ASSERT_PTR_NULL(file);
    CU_ASSERT_EQUAL(owned, 0);
}

int main(int argc, char** argv) {
    // 初始化CUnit测试注册表
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // 添加测试套件
    CU_pSuite pSuite = CU_add_suite("simple_log_sink_Test_Suite", NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // 添加测试用例到测试套件
    CU_add_test(pSuite, "test_slog_get_sink_p", test_slog_get_sink_p);
    CU_add_test(pSuite, "test_slog_get_sink_fp", test_slog_get_sink_fp);
    CU_add_test(pSuite, "test_slog_get_sink_file", test_slog_get_sink_file);
    CU_add_test(pSuite, "test_slog_get_sink_owned", test_slog_get_sink_owned);
    CU_add_test(pSuite, "test_slog_set_sink_fp", test_slog_set_sink_fp);
    CU_add_test(pSuite, "test_slog_set_sink_file", test_slog_set_sink_file);
    CU_add_test(pSuite, "test_slog_set_sink_owned", test_slog_set_sink_owned);
    CU_add_test(pSuite, "test_slog_set_sink", test_slog_set_sink);

    // 运行所有测试
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    // 清理注册表并返回结果
    CU_cleanup_registry();
    return CU_get_error();
}