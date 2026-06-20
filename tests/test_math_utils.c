#include <stdio.h>
#include "minunit.h"
#include "../include/pc/ultra64.h"
#include "../include/functions.h"

int tests_run = 0;

s32 Math_RoundF(f32 value);

static char * test_math_roundf() {
    mu_assert("Round 0.0 failed", Math_RoundF(0.0f) == 0);
    mu_assert("Round 0.5 failed", Math_RoundF(0.5f) == 1);
    mu_assert("Round 0.4 failed", Math_RoundF(0.4f) == 0);
    mu_assert("Round -0.5 failed", Math_RoundF(-0.5f) == -1);
    mu_assert("Round -0.4 failed", Math_RoundF(-0.4f) == 0);
    mu_assert("Round 1.9 failed", Math_RoundF(1.9f) == 2);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_math_roundf);
    return 0;
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    char *result = all_tests();
    if (result != 0) {
        printf("TEST FAILED: %s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
