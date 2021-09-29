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

// must(actual, ...)
#define cuu_must(actual, pred) CUUTEST_MUST(actual, CUUTEST_PRED_##pred)

#define CUUTEST_MUST(actual, pred) CUUTEST_MUST_I(actual, pred)
#define CUUTEST_MUST_I(actual, cond, desc, ...)                                \
    {                                                                          \
        bool _cuu_ok = (cond(actual, __VA_ARGS__));                            \
        const char *_cuu_desc = (desc(actual, __VA_ARGS__));                   \
        cuu_impl_must(_cuu_ok, _cuu_desc, __FILE__, __LINE__);                 \
        if (!_cuu_ok) {                                                        \
            continue;                                                          \
        }                                                                      \
    }

void cuu_impl_must(bool ok, const char *desc, const char *file,
                   unsigned long long line);

// should(actual, ...)
#define cuu_should(actual, pred) CUUTEST_SHOULD(actual, CUUTEST_PRED_##pred)

#define CUUTEST_SHOULD(actual, pred) CUUTEST_SHOULD_I(actual, pred)
#define CUUTEST_SHOULD_I(actual, cond, desc, ...)                              \
    do {                                                                       \
        bool _cuu_ok = (cond(actual, __VA_ARGS__));                            \
        const char *_cuu_desc = (desc(actual, __VA_ARGS__));                   \
        cuu_impl_should(_cuu_ok, _cuu_desc, __FILE__, __LINE__);               \
    } while (0)

void cuu_impl_should(bool ok, const char *desc, const char *file,
                     unsigned long long line);

// fail(message)
#define cuu_fail(...)                                                          \
    {                                                                          \
        cuu_impl_fail(__FILE__, __LINE__, __VA_ARGS__);                        \
        continue;                                                              \
    }

void cuu_impl_fail(const char *file, unsigned long long line, const char *fmt,
                   ...) CUUTEST_ATTRIBUTE(format(printf, 3, 4));

// be_true
#define CUUTEST_PRED_be_true                                                   \
    CUUTEST_PRED_COND_be_true, CUUTEST_PRED_DESC_be_true, _
#define CUUTEST_PRED_COND_be_true(actual, _)                                   \
    (cuu_impl_pred_cond_be_true((actual)))
#define CUUTEST_PRED_DESC_be_true(actual, _) #actual " == true"

bool cuu_impl_pred_cond_be_true(bool actual);

// be_false
#define CUUTEST_PRED_be_false                                                  \
    CUUTEST_PRED_COND_be_false, CUUTEST_PRED_DESC_be_false, _
#define CUUTEST_PRED_COND_be_false(actual, _)                                  \
    (cuu_impl_pred_cond_be_false((actual)))
#define CUUTEST_PRED_DESC_be_false(actual, _) #actual " == false"

bool cuu_impl_pred_cond_be_false(bool actual);

// be_null
#define CUUTEST_PRED_be_null                                                   \
    CUUTEST_PRED_COND_be_null, CUUTEST_PRED_DESC_be_null, _
#define CUUTEST_PRED_COND_be_null(actual, _)                                   \
    (cuu_impl_pred_cond_be_null((actual)))
#define CUUTEST_PRED_DESC_be_null(actual, _) #actual " == null"

bool cuu_impl_pred_cond_be_null(const void *actual);

// not(pred)
#define CUUTEST_PRED_not(pred)                                                 \
    CUUTEST_PRED_COND_not, CUUTEST_PRED_DESC_not, CUUTEST_PRED_##pred
#define CUUTEST_PRED_COND_not(actual, cond, desc, ...)                         \
    !(cond(actual, __VA_ARGS__))
#define CUUTEST_PRED_DESC_not(actual, cond, desc, ...)                         \
    "not " desc(actual, __VA_ARGS__)

// be(op, x)
#define CUUTEST_PRED_be(op, x) CUUTEST_PRED_COND_be, CUUTEST_PRED_DESC_be, op, x
#define CUUTEST_PRED_COND_be(actual, op, x) ((actual)op(x))
#define CUUTEST_PRED_DESC_be(actual, op, x) #actual " " #op " " #x

// eq(x)
#define CUUTEST_PRED_eq(x) CUUTEST_PRED_be(==, x)

// ne(x)
#define CUUTEST_PRED_ne(x) CUUTEST_PRED_be(!=, x)

// gt(x)
#define CUUTEST_PRED_gt(x) CUUTEST_PRED_be(>, x)

// ge(x)
#define CUUTEST_PRED_ge(x) CUUTEST_PRED_be(>=, x)

// lt(x)
#define CUUTEST_PRED_lt(x) CUUTEST_PRED_be(<, x)

// le(x)
#define CUUTEST_PRED_le(x) CUUTEST_PRED_be(<=, x)

// be_near(x, err)
#define CUUTEST_PRED_be_near(x, err)                                           \
    CUUTEST_PRED_COND_be_near, CUUTEST_PRED_DESC_be_near, x, err
#define CUUTEST_PRED_COND_be_near(actual, x, err)                              \
    (((x) - (err)) <= (actual) && (actual) <= ((x) + (err)))
#define CUUTEST_PRED_DESC_be_near(actual, x, err) #actual " ≃ " #x "±" #err

// eq_str(expected)
#define CUUTEST_PRED_eq_str(expected)                                          \
    CUUTEST_PRED_COND_eq_str, CUUTEST_PRED_DESC_eq_str, expected
#define CUUTEST_PRED_COND_eq_str(actual, expected)                             \
    _Generic((actual), cuu_string_span                                         \
             : cuu_impl_pred_cond_eq_str_n, default                            \
             : cuu_impl_pred_cond_eq_str)((actual), (expected))
#define CUUTEST_PRED_DESC_eq_str(actual, expected) #actual " == " #expected

typedef struct cuu_string_span {
    const char *ptr;
    size_t len;
} cuu_string_span;

cuu_string_span cuu_str_n(const char *ptr, size_t len);

bool cuu_impl_pred_cond_eq_str(const char *actual, const char *expected);
bool cuu_impl_pred_cond_eq_str_n(cuu_string_span actual, const char *expected);

// contain(s)
#define CUUTEST_PRED_contain(s)                                                \
    CUUTEST_PRED_COND_contain, CUUTEST_PRED_DESC_contain, s
#define CUUTEST_PRED_COND_contain(actual, s)                                   \
    (cuu_impl_pred_cond_contain((actual), (s)))
#define CUUTEST_PRED_DESC_contain(actual, s) #actual " contain " #s

bool cuu_impl_pred_cond_contain(const char *actual, const char *s);

// start_with(prefix)
#define CUUTEST_PRED_start_with(prefix)                                        \
    CUUTEST_PRED_COND_start_with, CUUTEST_PRED_DESC_start_with, prefix
#define CUUTEST_PRED_COND_start_with(actual, prefix)                           \
    (cuu_impl_pred_cond_start_with((actual), (prefix)))
#define CUUTEST_PRED_DESC_start_with(actual, prefix)                           \
#actual " start with " #prefix

bool cuu_impl_pred_cond_start_with(const char *actual, const char *prefix);

// end_with(suffix)
#define CUUTEST_PRED_end_with(suffix)                                          \
    CUUTEST_PRED_COND_end_with, CUUTEST_PRED_DESC_end_with, suffix
#define CUUTEST_PRED_COND_end_with(actual, suffix)                             \
    (cuu_impl_pred_cond_end_with((actual), (suffix)))
#define CUUTEST_PRED_DESC_end_with(actual, suffix) #actual " end with " #suffix

bool cuu_impl_pred_cond_end_with(const char *actual, const char *suffix);

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
