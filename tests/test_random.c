#include "random.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define TEST_ITERATIONS 100000
#define TOLERANCE 0.01

// Color codes for output
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\033[0;31m"
#define COLOR_RESET "\033[0m"

int tests_passed = 0;
int tests_failed = 0;

void test_passed(const char* test_name) {
    printf("[%sPASS%s] %s\n", COLOR_GREEN, COLOR_RESET, test_name);
    tests_passed++;
}

void test_failed(const char* test_name, const char* reason) {
    printf("[%sFAIL%s] %s: %s\n", COLOR_RED, COLOR_RESET, test_name, reason);
    tests_failed++;
}

// Test 1: Verify uniform() returns values in [0, 1)
void test_uniform_range() {
    setSeed(12345);
    int valid = 1;
    
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        double val = uniform();
        if (val < 0.0 || val >= 1.0) {
            char msg[100];
            snprintf(msg, sizeof(msg), "value %.10f out of range [0, 1)", val);
            test_failed("test_uniform_range", msg);
            valid = 0;
            break;
        }
    }
    
    if (valid) {
        test_passed("test_uniform_range");
    }
}

// Test 2: Verify uniform() distribution approximates uniform distribution
void test_uniform_distribution() {
    setSeed(42);
    const int bins = 10;
    int counts[10] = {0};
    
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        double val = uniform();
        int bin = (int)(val * bins);
        if (bin >= bins) bin = bins - 1;  // Handle edge case
        counts[bin]++;
    }
    
    // Each bin should have approximately TEST_ITERATIONS / bins values
    double expected = TEST_ITERATIONS / (double)bins;
    int valid = 1;
    
    for (int i = 0; i < bins; i++) {
        double deviation = fabs(counts[i] - expected) / expected;
        if (deviation > 0.1) {  // Allow 10% deviation
            char msg[100];
            snprintf(msg, sizeof(msg), "bin %d has %d (expected ~%.0f, deviation %.2f%%)", 
                     i, counts[i], expected, deviation * 100);
            test_failed("test_uniform_distribution", msg);
            valid = 0;
            break;
        }
    }
    
    if (valid) {
        test_passed("test_uniform_distribution");
    }
}

// Test 3: Verify setSeed produces consistent results
void test_seed_consistency() {
    setSeed(999);
    double val1 = uniform();
    double val2 = uniform();
    
    setSeed(999);
    double val3 = uniform();
    double val4 = uniform();
    
    if (val1 == val3 && val2 == val4) {
        test_passed("test_seed_consistency");
    } else {
        char msg[100];
        snprintf(msg, sizeof(msg), "inconsistent: (%.10f, %.10f) != (%.10f, %.10f)", 
                 val1, val2, val3, val4);
        test_failed("test_seed_consistency", msg);
    }
}

// Test 4: Verify different seeds produce different sequences
void test_seed_diversity() {
    setSeed(100);
    double val1 = uniform();
    
    setSeed(200);
    double val2 = uniform();
    
    if (val1 != val2) {
        test_passed("test_seed_diversity");
    } else {
        test_failed("test_seed_diversity", "different seeds produced same value");
    }
}

// Test 5: Verify randRange returns values in correct range
void test_randRange_bounds() {
    setSeed(777);
    int lower = 10;
    int upper = 50;
    int valid = 1;
    
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        int val = randRange(lower, upper);
        if (val < lower || val >= upper) {
            char msg[100];
            snprintf(msg, sizeof(msg), "value %d out of range [%d, %d)", val, lower, upper);
            test_failed("test_randRange_bounds", msg);
            valid = 0;
            break;
        }
    }
    
    if (valid) {
        test_passed("test_randRange_bounds");
    }
}

// Test 6: Verify randRange handles swapped bounds
void test_randRange_swapped() {
    setSeed(555);
    int lower = 100;
    int upper = 10;
    int valid = 1;
    
    for (int i = 0; i < 1000; i++) {
        int val = randRange(lower, upper);
        if (val < upper || val >= lower) {
            char msg[100];
            snprintf(msg, sizeof(msg), "value %d out of range [%d, %d)", val, upper, lower);
            test_failed("test_randRange_swapped", msg);
            valid = 0;
            break;
        }
    }
    
    if (valid) {
        test_passed("test_randRange_swapped");
    }
}

// Test 7: Verify randRange handles equal bounds
void test_randRange_equal() {
    setSeed(333);
    int bound = 42;
    int valid = 1;
    
    for (int i = 0; i < 100; i++) {
        int val = randRange(bound, bound);
        if (val != bound) {
            char msg[100];
            snprintf(msg, sizeof(msg), "expected %d, got %d", bound, val);
            test_failed("test_randRange_equal", msg);
            valid = 0;
            break;
        }
    }
    
    if (valid) {
        test_passed("test_randRange_equal");
    }
}

// Test 8: Verify randRange covers full range
void test_randRange_coverage() {
    setSeed(888);
    int lower = 0;
    int upper = 10;
    int seen[10] = {0};
    
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        int val = randRange(lower, upper);
        seen[val] = 1;
    }
    
    int all_seen = 1;
    for (int i = 0; i < upper - lower; i++) {
        if (!seen[i]) {
            char msg[100];
            snprintf(msg, sizeof(msg), "value %d never generated in %d iterations", i, TEST_ITERATIONS);
            test_failed("test_randRange_coverage", msg);
            all_seen = 0;
            break;
        }
    }
    
    if (all_seen) {
        test_passed("test_randRange_coverage");
    }
}

// Test 9: Verify negative seed handling
void test_negative_seed() {
    setSeed(-12345);
    int valid = 1;
    
    for (int i = 0; i < 1000; i++) {
        double val = uniform();
        if (val < 0.0 || val >= 1.0) {
            test_failed("test_negative_seed", "negative seed produced invalid values");
            valid = 0;
            break;
        }
    }
    
    if (valid) {
        test_passed("test_negative_seed");
    }
}

// Test 10: Verify zero seed handling
void test_zero_seed() {
    setSeed(0);
    double val = uniform();
    
    if (val >= 0.0 && val < 1.0) {
        test_passed("test_zero_seed");
    } else {
        test_failed("test_zero_seed", "zero seed produced invalid value");
    }
}

// Test 11: Statistical test - mean should be close to 0.5
void test_uniform_mean() {
    setSeed(111);
    double sum = 0.0;
    
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        sum += uniform();
    }
    
    double mean = sum / TEST_ITERATIONS;
    double expected_mean = 0.5;
    double deviation = fabs(mean - expected_mean);
    
    if (deviation < 0.01) {  // Within 1% of expected
        test_passed("test_uniform_mean");
    } else {
        char msg[100];
        snprintf(msg, sizeof(msg), "mean %.6f deviates from expected 0.5 by %.6f", mean, deviation);
        test_failed("test_uniform_mean", msg);
    }
}

// Test 12: Verify no immediate repetition for reasonable sample
void test_no_immediate_repetition() {
    setSeed(654);
    double prev = uniform();
    int valid = 1;
    
    for (int i = 0; i < 1000; i++) {
        double curr = uniform();
        if (curr == prev) {
            test_failed("test_no_immediate_repetition", "immediate repetition detected");
            valid = 0;
            break;
        }
        prev = curr;
    }
    
    if (valid) {
        test_passed("test_no_immediate_repetition");
    }
}

int main(int argc, char** argv) {
    printf("===========================================\n");
    printf("Random Number Generator Test Suite\n");
    printf("===========================================\n\n");
    
    test_uniform_range();
    test_uniform_distribution();
    test_seed_consistency();
    test_seed_diversity();
    test_randRange_bounds();
    test_randRange_swapped();
    test_randRange_equal();
    test_randRange_coverage();
    test_negative_seed();
    test_zero_seed();
    test_uniform_mean();
    test_no_immediate_repetition();
    
    printf("\n===========================================\n");
    printf("Results: %d passed, %d failed\n", tests_passed, tests_failed);
    printf("===========================================\n");
    
    return tests_failed > 0 ? 1 : 0;
}
