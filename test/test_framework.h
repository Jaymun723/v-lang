#pragma once
#include <stdio.h>
#include <stdlib.h>

/**
 * Print a test success message
 */
#define TEST_PASS(msg) printf("PASS: %s\n", msg)

/**
 * Print a test failure message and exit
 */
#define TEST_FAIL(msg)                                                         \
  do {                                                                         \
    fprintf(stderr, "FAIL: %s\n", msg);                                        \
    exit(1);                                                                   \
  } while (0)

/**
 * Assert that a condition is true
 */
#define TEST_ASSERT(cond)                                                      \
  do {                                                                         \
    if (!(cond)) {                                                             \
      TEST_FAIL("Assertion failed: " #cond);                                   \
    }                                                                          \
  } while (0)
