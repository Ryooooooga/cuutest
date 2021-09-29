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

#define cuu_must(pred)                                                         \
    {                                                                          \
        bool _cuu_ok = cuu_impl_pred_cond_##pred;                              \
        const char *_cuu_desc = cuu_impl_pred_desc_##pred;                     \
        cuu_impl_must(_cuu_ok, _cuu_desc, __FILE__, __LINE__);                 \
        if (!_cuu_ok) {                                                        \
            continue;                                                          \
        }                                                                      \
    }

#define cuu_should(pred)                                                       \
    do {                                                                       \
        bool _cuu_ok = cuu_impl_pred_cond_##pred;                              \
        const char *_cuu_desc = cuu_impl_pred_desc_##pred;                     \
        cuu_impl_should(_cuu_ok, _cuu_desc, __FILE__, __LINE__);               \
    } while (0)

#define cuu_fail(...)                                                          \
    {                                                                          \
        cuu_impl_fail(__FILE__, __LINE__, __VA_ARGS__);                        \
        continue;                                                              \
    }

void cuu_impl_must(bool ok, const char *desc, const char *file,
                   unsigned long long line);

void cuu_impl_should(bool ok, const char *desc, const char *file,
                     unsigned long long line);

void cuu_impl_fail(const char *file, unsigned long long line, const char *fmt,
                   ...) CUUTEST_ATTRIBUTE(format(printf, 3, 4));

// be_true(x)
bool cuu_impl_pred_cond_be_true(bool actual);
#define cuu_impl_pred_desc_be_true(actual) #actual " == true"

// be_false(x)
bool cuu_impl_pred_cond_be_false(bool actual);
#define cuu_impl_pred_desc_be_false(actual) #actual " == false"

// be_null(x)
bool cuu_impl_pred_cond_be_null(const void *actual);
#define cuu_impl_pred_desc_be_null(actual) #actual " == null"

// not(pred)
#define cuu_impl_pred_cond_not(pred) (!(cuu_impl_pred_cond_##pred))
#define cuu_impl_pred_desc_not(pred) "not " cuu_impl_pred_desc_##pred

// be(x, op, y)
#define cuu_impl_pred_cond_be(actual, op, expected) ((actual)op(expected))
#define cuu_impl_pred_desc_be(actual, op, expected)                            \
#actual " " #op " " #expected

// be_near(x, y, err)
#define cuu_impl_pred_cond_be_near(actual, expected, error)                    \
    (((expected) - (error)) <= (actual) && (actual) <= ((expected) + (error)))
#define cuu_impl_pred_desc_be_near(actual, expected, error)                    \
#actual " ≃ " #expected "±" #error

// be_a(x, y)
#define cuu_impl_pred_cond_be_a(actual, expected)                              \
    cuu_impl_pred_cond_be((actual), ==, (expected))
#define cuu_impl_pred_desc_be_a(actual, expected)                              \
    cuu_impl_pred_desc_be(actual, ==, expected)

// be_gt(x, y)
#define cuu_impl_pred_cond_be_gt(actual, expected)                             \
    cuu_impl_pred_cond_be((actual), >, (expected))
#define cuu_impl_pred_desc_be_gt(actual, expected)                             \
    cuu_impl_pred_desc_be(actual, >, expected)

// be_ge(x, y)
#define cuu_impl_pred_cond_be_ge(actual, expected)                             \
    cuu_impl_pred_cond_be((actual), >=, (expected))
#define cuu_impl_pred_desc_be_ge(actual, expected)                             \
    cuu_impl_pred_desc_be(actual, >=, expected)

// be_lt(x, y)
#define cuu_impl_pred_cond_be_lt(actual, expected)                             \
    cuu_impl_pred_cond_be((actual), <, (expected))
#define cuu_impl_pred_desc_be_lt(actual, expected)                             \
    cuu_impl_pred_desc_be(actual, <, expected)

// be_le(x, y)
#define cuu_impl_pred_cond_be_le(actual, expected)                             \
    cuu_impl_pred_cond_be((actual), <=, (expected))
#define cuu_impl_pred_desc_be_le(actual, expected)                             \
    cuu_impl_pred_desc_be(actual, <=, expected)

// be_str(x, y)
bool cuu_impl_pred_cond_be_str(const char *actual, const char *expected);
#define cuu_impl_pred_desc_be_str(actual, expected) #actual " == " #expected

// be_str_n(x, n, y)
bool cuu_impl_pred_cond_be_str_n(const char *actual, size_t actual_len,
                                 const char *expected);
#define cuu_impl_pred_desc_be_str_n(actual, actual_len, expected)              \
#actual " == " #expected

// contain(x, y)
bool cuu_impl_pred_cond_contain(const char *actual, const char *expected);
#define cuu_impl_pred_desc_contain(actual, expected)                           \
#actual " contain " #expected

// start_with(x, prefix)
bool cuu_impl_pred_cond_start_with(const char *actual,
                                   const char *expected_prefix);
#define cuu_impl_pred_desc_start_with(actual, expected_prefix)                 \
#actual " start with " #expected_prefix

// end_with(x, suffix)
bool cuu_impl_pred_cond_end_with(const char *actual,
                                 const char *expected_suffix);
#define cuu_impl_pred_desc_end_with(actual, expected_suffix)                   \
#actual " end with " #expected_suffix

#define cuu_describe cuu_group
#define cuu_context cuu_group
#define cuu_it cuu_group

#if !CUUTEST_NO_ALIASES
#define must cuu_must
#define should cuu_should
#define describe cuu_describe
#define context cuu_context
#define it cuu_it
#endif

#endif // INCLUDE_CUUTEST_H
