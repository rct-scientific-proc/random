#include "random.h"
#include <stdio.h>

int main(void) {
    printf("Random Number Generator Example\n");
    printf("================================\n\n");
    
    // Set seed
    setSeed(42);
    
    // Generate uniform random numbers
    printf("10 uniform random numbers [0, 1):\n");
    for (int i = 0; i < 10; i++) {
        printf("  %.6f\n", uniform());
    }
    
    printf("\n10 random integers in range [1, 100):\n");
    setSeed(12345);
    for (int i = 0; i < 10; i++) {
        printf("  %d\n", randRange(1, 100));
    }
    
    printf("\nSimulating 6-sided die rolls:\n");
    setSeed(99);
    for (int i = 0; i < 10; i++) {
        printf("  Roll %d: %d\n", i + 1, randRange(1, 7));
    }
    
    return 0;
}
