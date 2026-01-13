
#ifndef RANDOM_H
#define RANDOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// DLL export/import macros
#if defined(_WIN32) || defined(_WIN64)
    #ifdef RANDOM_BUILD_SHARED
        #define RANDOM_API __declspec(dllexport)
    #elif defined(RANDOM_USE_SHARED)
        #define RANDOM_API __declspec(dllimport)
    #else
        #define RANDOM_API
    #endif
#else
    #if defined(RANDOM_BUILD_SHARED) && defined(__GNUC__)
        #define RANDOM_API __attribute__((visibility("default")))
    #else
        #define RANDOM_API
    #endif
#endif

/**
 * @brief Initialize the random number generator with a seed.
 * 
 * This function must be called before using uniform() or randRange().
 * 
 * @param seed Valid seed values [0x00000000 - 0xFFFFFFFF].
 */
RANDOM_API void random_init(int32_t seed);

/**
 * @brief Clean up resources used by the random number generator.
 * 
 * Call this function when done using the generator. After calling,
 * random_init() must be called again before using the generator.
 */
RANDOM_API void random_destroy(void);

/**
 * @brief Set the seed for the random number generators.
 * 
 * @param _seed Valid seed values [0x00000000 - 0xFFFFFFFF].
 */
RANDOM_API void setSeed(int32_t _seed);

/**
 * @brief Return random double value.
 * 
 * @return double - Random value, [0, 1).
 */
RANDOM_API double uniform(void);

/**
 * @brief Return random 32 bit signed integer.
 * 
 * @param lower Lower bound, inclusive.
 * @param upper Upper bound, exclusive.
 * @return int32_t - Random value, [lower, upper).
 */
RANDOM_API int32_t randRange(int32_t lower, int32_t upper);




#ifdef __cplusplus
}
#endif

#endif

