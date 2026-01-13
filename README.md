# random

Random number generator library in C using L'Ecuyer's combined linear congruential generator.

## Project Structure

```
random/
├── include/          # Header files
│   └── random.h
├── src/              # Source files
│   └── random.c
├── tests/            # Test suite
│   └── test_random.c
├── examples/         # Example applications
│   └── main.c
├── build/            # Build directory (generated)
├── CMakeLists.txt    # Build configuration
├── LICENSE
└── README.md
```

## Building

```bash
cmake -B build
cmake --build build
```

## Running Tests

```bash
cd build
ctest --verbose
```

Or run the test executable directly:
```bash
./build/test_random      # Linux/Mac
.\build\Release\test_random.exe  # Windows
```

## Usage Example

```c
#include "random.h"

int main(void) {
    // Set seed
    setSeed(42);
    
    // Generate uniform random number [0, 1)
    double rand_val = uniform();
    
    // Generate random integer in range [10, 50)
    int rand_int = randRange(10, 50);
    
    return 0;
}
```

## API

### `void setSeed(int32_t _seed)`
Set the seed for the random number generator. Valid seed values: 0x00000000 - 0xFFFFFFFF.

### `double uniform()`
Returns a random double precision value in the range [0, 1).

### `int32_t randRange(int32_t lower, int32_t upper)`
Returns a random 32-bit signed integer in the range [lower, upper).
If lower > upper, the bounds are automatically swapped.
If lower == upper, returns that value.

## Algorithm

This library implements L'Ecuyer's combined linear congruential generator using Schrage's method for modular arithmetic, which provides a period of approximately 2.3 × 10^18 before repetition.
