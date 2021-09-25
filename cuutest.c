#include "cuutest.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CUU_ANSI_RESET "\x1b[m"
#define CUU_ANSI_RED "\x1b[31m"
#define CUU_ANSI_GREEN "\x1b[32m"
#define CUU_ANSI_MAGENTA "\x1b[35m"

#define CUU_COLOR_INFO CUU_ANSI_GREEN
#define CUU_COLOR_SUCCESS CUU_ANSI_GREEN
#define CUU_COLOR_ERROR CUU_ANSI_RED
#define CUU_COLOR_NOTE CUU_ANSI_MAGENTA

#define CUU_TEXT_INFO(s) CUU_COLOR_INFO s CUU_ANSI_RESET
#define CUU_TEXT_SUCCESS(s) CUU_COLOR_SUCCESS s CUU_ANSI_RESET
#define CUU_TEXT_ERROR(s) CUU_COLOR_ERROR s CUU_ANSI_RESET
#define CUU_TEXT_NOTE(s) CUU_COLOR_NOTE s CUU_ANSI_RESET

#define CUU_OUTPUT(...) (fprintf(stderr, __VA_ARGS__))

#define CUU_VOUTPUT(fmt, last)                                                 \
    do {                                                                       \
        va_list args;                                                          \
        va_start(args, last);                                                  \
        vfprintf(stderr, fmt, args);                                           \
        va_end(args);                                                          \
    } while (0)

static int cuu_group_depth = 0;

static struct {
    clock_t start_time;
    int checks;
    int errors;
} cuu_groups[CUUTEST_MAX_GROUP_DEPTH + 1] = {0};

struct cuu_group_result {
    double duration_ms;
    int checks;
    int errors;
};

static void cuu_indent(void) {
    for (int i = 0; i < cuu_group_depth; i++) {
        CUU_OUTPUT("  ");
    }
}

static void cuu_check_success(void) {
    assert(cuu_group_depth >= 0);
    assert(cuu_group_depth <= CUUTEST_MAX_GROUP_DEPTH);

    cuu_groups[cuu_group_depth - 1].checks++;
}

static void cuu_check_error(void) {
    assert(cuu_group_depth >= 0);
    assert(cuu_group_depth <= CUUTEST_MAX_GROUP_DEPTH);

    cuu_groups[cuu_group_depth - 1].checks++;
    cuu_groups[cuu_group_depth - 1].errors++;
}

static void cuu_push_group(void) {
    assert(cuu_group_depth < CUUTEST_MAX_GROUP_DEPTH);

    cuu_groups[cuu_group_depth].start_time = clock();
    cuu_groups[cuu_group_depth].checks = 0;
    cuu_groups[cuu_group_depth].errors = 0;

    cuu_group_depth++;
}

static void cuu_pop_group(struct cuu_group_result *result) {
    assert(cuu_group_depth > 0);

    cuu_group_depth--;

    clock_t end_time = clock();
    clock_t start_time = cuu_groups[cuu_group_depth].start_time;

    result->duration_ms = 1e3 * (end_time - start_time) / CLOCKS_PER_SEC;
    result->checks = cuu_groups[cuu_group_depth].checks;
    result->errors = cuu_groups[cuu_group_depth].errors;
}

void cuu_impl_run_test(void (*test_func)(void), const char *test_name) {
    cuu_indent();
    CUU_OUTPUT(CUU_TEXT_INFO("================================") "\n");
    cuu_indent();
    CUU_OUTPUT("  " CUU_TEXT_INFO("%s") "\n", test_name);
    cuu_indent();
    CUU_OUTPUT(CUU_TEXT_INFO("================================") "\n");
    CUU_OUTPUT("\n");

    cuu_push_group();

    test_func();

    struct cuu_group_result result;
    cuu_pop_group(&result);

    cuu_indent();
    if (result.errors > 0) {
        CUU_OUTPUT("[" CUU_TEXT_ERROR("failed %d/%d") "]", result.errors,
                   result.checks);
        cuu_check_error();
    } else {
        CUU_OUTPUT("[" CUU_TEXT_SUCCESS("passed %d/%d") "]", result.checks,
                   result.checks);
        cuu_check_success();
    }

    CUU_OUTPUT(" - " CUU_TEXT_NOTE("%.1fms") "\n", result.duration_ms);
    CUU_OUTPUT("\n");
}

int cuu_test_result(void) {
    assert(cuu_group_depth == 0 &&
           "cannot call `cuu_test_result()` inside of tests");

    return cuu_groups[cuu_group_depth].errors > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}

void cuu_impl_group_start(const char *format, ...) {
    cuu_indent();
    CUU_VOUTPUT(format, format);
    CUU_OUTPUT("\n");

    cuu_push_group();
}

void cuu_impl_group_end(void) {
    struct cuu_group_result result;
    cuu_pop_group(&result);

    cuu_indent();
    if (result.errors > 0) {
        CUU_OUTPUT("[" CUU_TEXT_ERROR("failed %d/%d") "]", result.errors,
                   result.checks);
        cuu_check_error();
    } else {
        CUU_OUTPUT("[" CUU_TEXT_SUCCESS("passed %d/%d") "]", result.checks,
                   result.checks);
        cuu_check_success();
    }

    CUU_OUTPUT(" - " CUU_TEXT_NOTE("%.1fms") "\n", result.duration_ms);
    CUU_OUTPUT("\n");
}

void cuu_impl_must(bool ok, const char *desc, const char *file,
                   unsigned long long line) {
    cuu_indent();
    if (ok) {
        CUU_OUTPUT(CUU_TEXT_SUCCESS("OK! - must %s") "\n", desc);
        cuu_check_success();
    } else {
        CUU_OUTPUT(CUU_TEXT_ERROR("NG! - must %s"), desc);
        CUU_OUTPUT(" at %s:%llu\n", file, line);
        cuu_check_error();
    }
}

void cuu_impl_should(bool ok, const char *desc, const char *file,
                     unsigned long long line) {
    cuu_indent();
    if (ok) {
        CUU_OUTPUT(CUU_TEXT_SUCCESS("OK! - should %s") "\n", desc);
        cuu_check_success();
    } else {
        CUU_OUTPUT(CUU_TEXT_ERROR("NG! - should %s"), desc);
        CUU_OUTPUT(" at %s:%llu\n", file, line);
        cuu_check_error();
    }
}

void cuu_impl_fail(const char *file, unsigned long long line, const char *fmt,
                   ...) {
    cuu_indent();
    CUU_OUTPUT(CUU_COLOR_ERROR "fail! - ");
    CUU_VOUTPUT(fmt, fmt);
    CUU_OUTPUT(CUU_ANSI_RESET " at %s:%llu\n", file, line);
    cuu_check_error();
}

bool cuu_impl_pred_cond_to_be_true(bool actual) { return actual; }
bool cuu_impl_pred_cond_to_be_false(bool actual) { return !actual; }

bool cuu_impl_pred_cond_to_be_null(const void *actual) {
    return actual == NULL;
}

bool cuu_impl_pred_cond_to_be_str(const char *actual, const char *expected) {
    assert(expected != NULL);
    if (actual == NULL)
        return false;
    return strcmp(actual, expected) == 0;
}

bool cuu_impl_pred_cond_to_be_str_n(const char *actual, size_t actual_len,
                                    const char *expected) {
    assert(expected != NULL);
    if (actual == NULL && actual_len > 0)
        return false;
    return actual_len == strlen(expected) &&
           strncmp(actual, expected, actual_len) == 0;
}

bool cuu_impl_pred_cond_contains(const char *actual, const char *expected) {
    assert(expected != NULL);
    if (actual == NULL)
        return false;
    return strstr(actual, expected) != NULL;
}

bool cuu_impl_pred_cond_starts_with(const char *actual,
                                    const char *expected_prefix) {
    assert(expected_prefix);
    if (actual == NULL)
        return false;
    return strncmp(actual, expected_prefix, strlen(expected_prefix)) == 0;
}

bool cuu_impl_pred_cond_ends_with(const char *actual,
                                  const char *expected_suffix) {
    assert(expected_suffix);
    if (actual == NULL)
        return false;
    size_t actual_len = strlen(actual);
    size_t expected_len = strlen(expected_suffix);
    return actual_len >= expected_len &&
           strncmp(actual + actual_len - expected_len, expected_suffix,
                   expected_len) == 0;
}
