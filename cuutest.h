#ifndef INCLUDE_CUUTEST_H
#define INCLUDE_CUUTEST_H

#include <stdbool.h>
#include <stddef.h>

#ifndef CUUTEST_MAX_GROUP_DEPTH
#define CUUTEST_MAX_GROUP_DEPTH (8)
#endif

#ifndef CUUTEST_NO_ALIASES
#define CUUTEST_NO_ALIASES 0
#endif

#ifdef __GNUC__
#define CUUTEST_ATTRIBUTE(attribute) __attribute__((attribute))
#else
#define CUUTEST_ATTRIBUTE(attribute)
#endif

#define cuu_test(test_name) void cuu_test_function_##test_name(void)

#define cuu_run_test(test_name)                                                \
    do {                                                                       \
        void cuu_test_function_##test_name(void);                              \
        cuu_impl_run_test(&cuu_test_function_##test_name, #test_name);         \
    } while (0)

void cuu_impl_run_test(void (*test_func)(void), const char *test_name);

int cuu_test_result(void);

#define cuu_group(...)                                                         \
    for (bool _cuu_group_first = true;                                         \
         _cuu_group_first ? cuu_impl_group_start(__VA_ARGS__)                  \
                          : cuu_impl_group_end(),                              \
              _cuu_group_first;                                                \
         _cuu_group_first = false)

void cuu_impl_group_start(const char *format, ...)
    CUUTEST_ATTRIBUTE(format(printf, 1, 2));

void cuu_impl_group_end(void);

#define must(pred)                                                             \
    do {                                                                       \
        bool _cuu_ok = cuu_impl_pred_cond_##pred;                              \
        const char *_cuu_desc = cuu_impl_pred_desc_##pred;                     \
        cuu_impl_must(_cuu_ok, _cuu_desc, __FILE__, __LINE__);                 \
        if (!_cuu_ok) {                                                        \
            continue;                                                          \
        }                                                                      \
    } while (0)

#define should(pred)                                                           \
    do {                                                                       \
        bool _cuu_ok = cuu_impl_pred_cond_##pred;                              \
        const char *_cuu_desc = cuu_impl_pred_desc_##pred;                     \
        cuu_impl_should(_cuu_ok, _cuu_desc, __FILE__, __LINE__);               \
    } while (0)

void cuu_impl_must(bool ok, const char *desc, const char *file,
                   unsigned long long line);

void cuu_impl_should(bool ok, const char *desc, const char *file,
                     unsigned long long line);

// to_be_true(x)
bool cuu_impl_pred_cond_to_be_true(bool actual);
#define cuu_impl_pred_desc_to_be_true(actual) #actual " == true"

// to_be_false(x)
bool cuu_impl_pred_cond_to_be_false(bool actual);
#define cuu_impl_pred_desc_to_be_false(actual) #actual " == false"

// to_be_null(x)
bool cuu_impl_pred_cond_to_be_null(const void *actual);
#define cuu_impl_pred_desc_to_be_null(actual) #actual " == null"

// not(pred)
#define cuu_impl_pred_cond_not(pred) (!(cuu_impl_pred_cond_##pred))
#define cuu_impl_pred_desc_not(pred) "not " cuu_impl_pred_desc_##pred

// to_be(x, op, y)
#define cuu_impl_pred_cond_to_be(actual, op, expected) ((actual)op(expected))
#define cuu_impl_pred_desc_to_be(actual, op, expected)                         \
#actual " " #op " " #expected

// to_be_near(x, y, err)
#define cuu_impl_pred_cond_to_be_near(actual, expected, error)                 \
    (((expected) - (error)) <= (actual) && (actual) <= ((expected) + (error)))
#define cuu_impl_pred_desc_to_be_near(actual, expected, error)                 \
#actual " ≃ " #expected "±" #error

// to_be_a(x, y)
#define cuu_impl_pred_cond_to_be_a(actual, expected)                           \
    cuu_impl_pred_cond_to_be((actual), ==, (expected))
#define cuu_impl_pred_desc_to_be_a(actual, expected)                           \
    cuu_impl_pred_desc_to_be(actual, ==, expected)

// to_be_gt(x, y)
#define cuu_impl_pred_cond_to_be_gt(actual, expected)                          \
    cuu_impl_pred_cond_to_be((actual), >, (expected))
#define cuu_impl_pred_desc_to_be_gt(actual, expected)                          \
    cuu_impl_pred_desc_to_be(actual, >, expected)

// to_be_ge(x, y)
#define cuu_impl_pred_cond_to_be_ge(actual, expected)                          \
    cuu_impl_pred_cond_to_be((actual), >=, (expected))
#define cuu_impl_pred_desc_to_be_ge(actual, expected)                          \
    cuu_impl_pred_desc_to_be(actual, >=, expected)

// to_be_lt(x, y)
#define cuu_impl_pred_cond_to_be_lt(actual, expected)                          \
    cuu_impl_pred_cond_to_be((actual), <, (expected))
#define cuu_impl_pred_desc_to_be_lt(actual, expected)                          \
    cuu_impl_pred_desc_to_be(actual, <, expected)

// to_be_le(x, y)
#define cuu_impl_pred_cond_to_be_le(actual, expected)                          \
    cuu_impl_pred_cond_to_be((actual), <=, (expected))
#define cuu_impl_pred_desc_to_be_le(actual, expected)                          \
    cuu_impl_pred_desc_to_be(actual, <=, expected)

// to_be_str(x, y)
bool cuu_impl_pred_cond_to_be_str(const char *actual, const char *expected);
#define cuu_impl_pred_desc_to_be_str(actual, expected) #actual " == " #expected

// to_be_str_n(x, n, y)
bool cuu_impl_pred_cond_to_be_str_n(const char *actual, size_t actual_len,
                                    const char *expected);
#define cuu_impl_pred_desc_to_be_str_n(actual, actual_len, expected)           \
#actual " == " #expected

// contains(x, y)
bool cuu_impl_pred_cond_contains(const char *actual, const char *expected);
#define cuu_impl_pred_desc_contains(actual, expected)                          \
#actual " contains " #expected

// starts_with(x, prefix)
bool cuu_impl_pred_cond_starts_with(const char *actual,
                                    const char *expected_prefix);
#define cuu_impl_pred_desc_starts_with(actual, expected_prefix)                \
#actual " starts with " #expected_prefix

// ends_with(x, suffix)
bool cuu_impl_pred_cond_ends_with(const char *actual,
                                  const char *expected_suffix);
#define cuu_impl_pred_desc_ends_with(actual, expected_suffix)                  \
#actual " ends with " #expected_suffix

#define cuu_describe cuu_group
#define cuu_context cuu_group
#define cuu_it cuu_group

#if !CUUTEST_NO_ALIASES
#define describe cuu_describe
#define context cuu_context
#define it cuu_it
#endif

#endif // INCLUDE_CUUTEST_H
