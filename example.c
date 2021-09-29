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
        should(factorial(0), be_a(1));
        should(factorial(1), be_a(1));
        should(factorial(5), be_a(120));
    }
}

cuu_test(other) {
    describe("string predicates") {
        const char *s = "Hello, world!";

        must(s, not(be_null));

        should(s, be_str("Hello, world!"));
        should(cuu_str_n(s, 6), be_str("Hello,"));
        should(s, contain("lo"));
        should(s, start_with("Hell"));
        should(s, end_with("!"));
    }

    describe("boolean predicates") {
        bool c = true;

        should(c, be_true);
        should(!c, be_false);
    }

    describe("numeric predicates") {
        it("int") {
            int i = 42;

            should(i, be_a(42));
            should(i, be_gt(41));
            should(i, be_ge(42));
            should(i, be_lt(43));
            should(i, be_le(42));
            // same as
            should(i, be(==, 42));
            should(i, be(>, 41));
            should(i, be(>=, 42));
            should(i, be(<, 43));
            should(i, be(<=, 42));
        }
        it("double") {
            double f = 3.14159265;

            should(f, be_near(3.1416, 0.0001));

            should(f, be_gt(3.1));
            should(f, be_ge(3.1));
            should(f, be_lt(3.2));
            should(f, be_le(3.2));
            // same as
            should(f, be(>, 3.1));
            should(f, be(>=, 3.1));
            should(f, be(<, 3.2));
            should(f, be(<=, 3.2));
        }
    }

#if 0
    describe("cuu_fail()") {
        cuu_fail("always fail");
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
