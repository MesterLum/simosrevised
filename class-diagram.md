# UML Class Diagram

This diagram shows the class structure of the SIMOS Revised weight calculation system.

```mermaid
classDiagram
    class SimosRevised {
        -RanksMapType ranks
        -WhiteCardsMapType whiteCards
        -DataPrecisionType zRatio
        -int decimals
        -bool debug
        +SimosRevised(RanksMapType, WhiteCardsMapType, DataPrecisionType, unsigned int, bool)
        +generateWeights() vector~RankWeight~
        #getNonNormalizedWeights(WhiteCardsMapType, DataPrecisionType) WeightsMapType
        #getNormalizedWeights(WeightsMapType, DataPrecisionType) WeightsMapType
        #getCriteriaCount() unsigned int
        #getWhiteCardsWithNonZeroValues() WhiteCardsMapType
        #getWhiteCardsCount(WhiteCardsMapType) unsigned int
        #getRatio(unsigned int) DataPrecisionType
        #getWeightsSum(WeightsMapType) DataPrecisionType
        #getNormalizedWeightsTruncated(WeightsMapType) WeightsMapType
        #getNearestUpWeights(WeightsMapType, WeightsMapType) WeightsMapType
        #getNearestDownWeights(WeightsMapType, WeightsMapType) WeightsMapType
        #getGreaterElements(WeightsMapType, WeightsMapType) RanksMapType
        #getSumGreaterElements(RanksMapType) unsigned int
        #sortWeights(WeightsMapType, bool) ListWeightType
        #getF1Ranks(ListWeightType, RanksMapType, unsigned int, unsigned int) RanksMapType
        #getF2Ranks(ListWeightType, RanksMapType, unsigned int) RanksMapType
        #roundUp(WeightsMapType, RanksMapType) void
        #roundDown(WeightsMapType, RanksMapType) void
        #convertToRanksWeight(WeightsMapType) vector~RankWeight~
        #forceRoundUp(vector~RankWeight~, ListWeightType, RanksMapType, int) void
        #forceRoundDown(vector~RankWeight~, WeightsMapType, RanksMapType, int) void
    }

    class SimosUtils {
        <<utility>>
        +roundToXDecimals(DataPrecisionType, unsigned int)$ DataPrecisionType
        +roundUpToXDecimals(DataPrecisionType, unsigned int)$ DataPrecisionType
        +truncateToXDecimals(DataPrecisionType, unsigned int)$ DataPrecisionType
        +printUnorderedRanksData(ListWeightType)$ void
        +printWeights(WeightsMapType, int)$ void
        +printRankGroups(RanksMapType)$ void
        +getWeightsSum(vector~RankWeight~)$ DataPrecisionType
    }

    class RankWeight {
        <<struct>>
        +unsigned int rank
        +CriterionType criterion
        +DataPrecisionType weight
    }

    SimosRevised ..> RankWeight : uses
    SimosUtils ..> RankWeight : uses
    SimosUtils ..> SimosRevised : depends on types

    note for SimosRevised "Main class implementing the\nSIMOS Revised method for\nmulti-criteria weight calculation"
    note for SimosUtils "Utility class providing\nstatic helper methods for\nrounding and printing"
    note for RankWeight "Data structure representing\na criterion with its rank\nand calculated weight"
```

## Type Definitions

The following type aliases are used throughout the system:

- **DataPrecisionType**: `double` - Precision type for weight calculations
- **CriterionType**: `std::string` - Type for criterion identifiers
- **RankGroupType**: `std::vector<CriterionType>` - Group of criteria at the same rank
- **RanksMapType**: `std::map<unsigned int, RankGroupType>` - Map of rank to criteria groups
- **WhiteCardsMapType**: `std::map<unsigned int, unsigned int>` - Map of white cards per rank
- **WeightsMapType**: `std::map<unsigned int, DataPrecisionType>` - Map of criterion ID to weight
- **ListWeightType**: `std::vector<std::pair<unsigned int, DataPrecisionType>>` - List of ID-weight pairs

## Relationships

- **SimosRevised** uses **RankWeight** as return type for `generateWeights()`
- **SimosUtils** depends on types defined in SimosRevised header
- **SimosUtils** provides static utility methods for both **SimosRevised** and **RankWeight**
