# SIMOS Revised Algorithm

A C++ implementation of the **SIMOS Revised** (Sequential Interactive Model for Objective Selection) method for calculating criterion weights in Multi-Criteria Decision Analysis (MCDA).

## What Problem Does This Solve?

In decision-making processes involving multiple criteria (e.g., choosing between products, evaluating alternatives), it's crucial to determine the relative importance (weights) of each criterion. The SIMOS Revised algorithm helps decision-makers:

- **Convert subjective preferences into objective weights**: Transform rank-based importance into numerical weights that sum to 100%
- **Handle equal importance**: Use "white cards" to indicate equal importance between criteria at different rank levels
- **Minimize distortion**: Apply sophisticated rounding techniques to maintain precision while ensuring weights sum exactly to 100%
- **Provide transparency**: Generate weights through a clear, reproducible mathematical process

## Algorithm Overview

The SIMOS Revised method works by:

1. **Ranking criteria** by importance (1 = least important)
2. **Using white cards** to define the gap/interval between rank groups (0 = adjacent ranks are equally spaced)
3. **Calculating normalized weights** that sum to exactly 100% with specified decimal precision
4. **Applying lexicographic optimization** to minimize rounding distortion

The algorithm ensures that:

- The ratio between the most and least important criteria equals a predefined value `z`
- All weights sum to exactly 100 (or 1.0 if normalized to 1)
- Rounding distortion is minimized using dysfunction analysis

## Features

- ✅ Full implementation of SIMOS Revised algorithm
- ✅ Configurable decimal precision (0..9 decimal places)
- ✅ Support for white cards to model intervals between ranks
- ✅ Lexicographic rounding optimization
- ✅ Comprehensive unit tests using Google Test
- ✅ Debug mode for detailed calculation steps

## Requirements

- **C++ Compiler**: GCC or Clang with C++23 support
- **Build System**: Make
- **Testing** (optional): Google Test library

### Installing Dependencies

#### macOS

```bash
brew install googletest
```

#### Ubuntu/Debian

```bash
sudo apt-get install libgtest-dev
```

## Building the Project

### Build the main application

```bash
make
```

### Run the application

```bash
make run
```

### Build and run tests

```bash
make test
```

### Clean build artifacts

```bash
make clean
```

### Show all available commands

```bash
make help
```

## Usage

### Basic Example

```cpp
#include "include/simos_revised.hpp"
#include "include/utils.hpp"
#include <iostream>

int main() {
  // Configuration
  double z = 5.0;        // Ratio between max and min weights
  int decimals = 2;      // Decimal precision (0, 1, or 2)

  // Define rank groups (1 = least important)
  RanksMapType ranks = {
    {1, RankGroupType{"RGB_Lights"}},    // Least important
    {2, RankGroupType{"RAM"}},
    {3, RankGroupType{"GPU_Power"}},
    {4, RankGroupType{"Price"}}          // Most important
  };

  // Define white cards (intervals between ranks)
  // 0 = adjacent ranks are equally spaced
  // n > 0 = add n extra intervals between rank i and i+1
  WhiteCardsMapType whiteCards = {
    {1, 0},  // Between rank 1 and 2
    {2, 0},  // Between rank 2 and 3
    {3, 0}   // Between rank 3 and 4
  };

  // Create SIMOS instance and generate weights
  SimosRevised simos(ranks, whiteCards, z, decimals);
  const auto weights = simos.generateWeights();

  // Display results
  for (const auto &w : weights) {
    std::cout << "Rank " << w.rank << ": "
              << w.criterion << " : "
              << w.weight << std::endl;
  }

  // Verify sum equals 100
  const auto total = SimosUtils::getWeightsSum(weights);
  std::cout << "Total: " << total << std::endl;

  return 0;
}
```

### Understanding Parameters

#### `z` (Ratio)

The ratio between the maximum and minimum criterion weights. For example:

- `z = 5.0`: Most important criterion has 5× the weight of the least important
- `z = 3.0`: Maximum is 3× the minimum
- Higher values = greater differentiation between criteria

#### `decimals` (Precision)

Number of decimal places for weights:

- `0`: Integer weights (e.g., 25, 33, 42)
- `1`: One decimal (e.g., 25.5, 33.3, 41.2)
- `2`: Two decimals (e.g., 25.42, 33.33, 41.25)

#### `ranks` (Rank Groups)

Ordered map where:

- Key = rank number (1 = least important)
- Value = vector of criterion names at that rank level
- Multiple criteria can share the same rank

#### `whiteCards` (Intervals)

Map defining extra spacing between ranks:

- Key = rank number
- Value = number of additional intervals
- `0` = equal spacing
- `n > 0` = adds n extra intervals between rank i and i+1

### Advanced Example with White Cards

```cpp
// Different spacing between ranks
RanksMapType ranks = {
  {1, RankGroupType{"Aesthetics", "Brand"}},  // Two criteria at rank 1
  {2, RankGroupType{"Battery"}},
  {3, RankGroupType{"Performance"}},
  {4, RankGroupType{"Price", "Warranty"}}     // Two criteria at rank 4
};

// Add extra intervals to emphasize importance differences
WhiteCardsMapType whiteCards = {
  {1, 1},  // 1 extra interval between rank 1 and 2
  {2, 0},  // Normal spacing
  {3, 2}   // 2 extra intervals between rank 3 and 4
};

SimosRevised simos(ranks, whiteCards, 6.0, 2, true);  // Enable debug mode
auto weights = simos.generateWeights();
```

## Project Structure

```
.
├── include/
│   ├── simos_revised.hpp    # Main SIMOS algorithm class
│   └── utils.hpp             # Utility functions (rounding, printing, etc.)
├── src/
│   ├── simos_revised.cpp    # SIMOS algorithm implementation
│   └── utils.cpp             # Utility functions implementation
├── tests/
│   ├── test_simos_revised.cpp  # SIMOS algorithm tests
│   └── test_utils.cpp          # Utility functions tests
├── main.cpp                  # Example usage
├── Makefile                  # Build configuration
└── README.md                 # This file
```

## Diagrams

For visual representations of the algorithm and class structure, see the [diagrams documentation](diagrams/readme.md).

## Algorithm Details

The SIMOS Revised algorithm follows these steps:

### Step 1: Convert White Cards to Intervals

- Transform white card counts into interval measures: `e_r = e'_r + 1`

### Step 2: Calculate Unit Ratio

- Compute `u = (z-1)/e` where `e` is the sum of all intervals
- This ratio determines the weight increment per interval

### Step 3: Generate Non-Normalized Weights

- Calculate `k(r) = 1 + u × (e_0 + ... + e_{r-1})`
- By convention, `k(1) = 1` for the least important criterion

### Step 4: Normalize Weights

- Scale weights so they sum to 100: `k_i = 100 × k'_i / K'`

### Step 5: Truncate to Desired Precision

- Remove decimal figures beyond the specified precision

### Step 6: Lexicographic Rounding Optimization

- Calculate dysfunction ratios for rounding decisions:
  - `δ'_i`: dysfunction for rounding up
  - `δ_i`: dysfunction for rounding down
- Partition criteria into sets F+ (round up) and F- (round down)
- Minimize total distortion in lexicographic order

## Testing

The project includes comprehensive unit tests. For detailed information about the testing strategy and test cases, see the [testing documentation](tests/readme.md).

```bash
# Run all tests
make test

# Run only utility tests
make test-utils

# Run only SIMOS algorithm tests
make test-simos
```

## References

The SIMOS Revised algorithm is based on:

- Figueira, J., & Roy, B. (2002). "Determining the weights of criteria in the ELECTRE type methods with a revised Simos' procedure"
- SIMOS method for determining criterion weights in MCDA

## License

This project is provided as-is for educational and research purposes.

## Contributing

Contributions are welcome! Please ensure:

- Code follows C++23 standards
- All tests pass (`make test`)
- New features include corresponding unit tests

## Support

For issues or questions, please open an issue in the project repository.
