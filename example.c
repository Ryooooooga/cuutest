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
        should(be_a(factorial(0), 1));
        should(be_a(factorial(1), 1));
        should(be_a(factorial(5), 120));
    }
}

cuu_test(other) {
    describe("string predicates") {
        const char *s = "Hello, world!";

        must(not(be_null(s)));

        should(be_str(s, "Hello, world!"));
        should(be_str_n(s, 6, "Hello,"));
        should(contain(s, "lo"));
        should(start_with(s, "Hell"));
        should(end_with(s, "!"));
    }

    describe("boolean predicates") {
        bool c = true;

        should(be_true(c));
        should(be_false(!c));
    }

    describe("numeric predicates") {
        it("int") {
            int i = 42;

            should(be_a(i, 42));

            should(be_gt(i, 41));
            should(be_ge(i, 42));
            should(be_lt(i, 43));
            should(be_le(i, 42));
            // same as
            should(be(i, >, 41));
            should(be(i, >=, 42));
            should(be(i, <, 43));
            should(be(i, <=, 42));
        }
        it("double") {
            double f = 3.14159265;

            should(be_near(f, 3.1416, 0.0001));

            should(be_gt(f, 3.1));
            should(be_ge(f, 3.1));
            should(be_lt(f, 3.2));
            should(be_le(f, 3.2));
            // same as
            should(be(f, >, 3.1));
            should(be(f, >=, 3.1));
            should(be(f, <, 3.2));
            should(be(f, <=, 3.2));
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
