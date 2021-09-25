#include "cuutest.h"

#include <stdio.h>

int factorial(int a) {
    int r = 1;
    for (int i = 1; i <= a; i++) {
        r *= i;
    }
    return r;
}

cuu_test(factorial) {
    it("should return a valid result") {
        should(to_be_a(factorial(0), 1));
        should(to_be_a(factorial(1), 1));
        should(to_be_a(factorial(5), 120));
    }
}

cuu_test(other) {
    describe("string predicates") {
        const char *s = "Hello, world!";

        must(not(to_be_null(s)));

        should(to_be_str(s, "Hello, world!"));
        should(to_be_str_n(s, 6, "Hello,"));
        should(contains(s, "lo"));
        should(starts_with(s, "Hell"));
        should(ends_with(s, "!"));
    }

    describe("boolean predicates") {
        bool c = true;

        should(to_be_true(c));
        should(to_be_false(!c));
    }

    describe("numeric predicates") {
        it("int") {
            int i = 42;

            should(to_be_a(i, 42));

            should(to_be_gt(i, 41));
            should(to_be_ge(i, 42));
            should(to_be_lt(i, 43));
            should(to_be_le(i, 42));
            // same as
            should(to_be(i, >, 41));
            should(to_be(i, >=, 42));
            should(to_be(i, <, 43));
            should(to_be(i, <=, 42));
        }
        it("double") {
            double f = 3.14159265;

            should(to_be_near(f, 3.1416, 0.0001));

            should(to_be_gt(f, 3.1));
            should(to_be_ge(f, 3.1));
            should(to_be_lt(f, 3.2));
            should(to_be_le(f, 3.2));
            // same as
            should(to_be(f, >, 3.1));
            should(to_be(f, >=, 3.1));
            should(to_be(f, <, 3.2));
            should(to_be(f, <=, 3.2));
        }
    }

#if 0
    describe("fail") {
        cuu_fail("fail test");
        // never reached
        fprintf(stderr, "never reached here\n");
    }
#endif
}

int main(void) {
    cuu_run_test(factorial);
    cuu_run_test(other);
    return cuu_test_result();
}
