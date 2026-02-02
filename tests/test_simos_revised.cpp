#include "../include/simos_revised.hpp"
#include "../include/utils.hpp"
#include <gtest/gtest.h>

// Test fixture with helper to access protected methods
class SimosRevisedTestable : public SimosRevised {
public:
  SimosRevisedTestable(const RanksMapType &ranks,
                       const WhiteCardsMapType &whiteCards,
                       DataPrecisionType zRatio, unsigned int decimals,
                       bool debug = false)
      : SimosRevised(ranks, whiteCards, zRatio, decimals, debug) {}

  // Expose protected methods for testing
  using SimosRevised::convertToRanksWeight;
  using SimosRevised::getCriteriaCount;
  using SimosRevised::getF1Ranks;
  using SimosRevised::getF2Ranks;
  using SimosRevised::getGreaterElements;
  using SimosRevised::getNearestDownWeights;
  using SimosRevised::getNearestUpWeights;
  using SimosRevised::getNonNormalizedWeights;
  using SimosRevised::getNormalizedWeights;
  using SimosRevised::getNormalizedWeightsTruncated;
  using SimosRevised::getRatio;
  using SimosRevised::getSumGreaterElements;
  using SimosRevised::getWeightsSum;
  using SimosRevised::getWhiteCardsCount;
  using SimosRevised::getWhiteCardsWithNonZeroValues;
  using SimosRevised::sortWeights;
};

class SimosRevisedTest : public ::testing::Test {
protected:
  const double EPSILON = 1e-6;

  // Test data
  RanksMapType testRanks = {{1, {"c", "g", "l"}},      {2, {"d"}},
                            {3, {"b", "f", "i", "j"}}, {4, {"e"}},
                            {5, {"a", "h"}},           {6, {"k"}}};

  WhiteCardsMapType testWhiteCards = {
      {1, 0}, // between rank 1 and 2
      {2, 1}, // between rank 2 and 3 (1 white card)
      {3, 0},
      {4, 0},
      {5, 0}};

  DataPrecisionType testZRatio = 6.5;
  unsigned int testDecimals = 1;
};

// ============================================================================
// Tests for getWhiteCardsWithNonZeroValues
// Convert white cards to intervals: e_r = e'_r + 1
// ============================================================================

TEST_F(SimosRevisedTest, GetWhiteCardsWithNonZeroValues_Basic) {
  SimosRevisedTestable simos(testRanks, testWhiteCards, testZRatio,
                             testDecimals);
  auto result = simos.getWhiteCardsWithNonZeroValues();

  EXPECT_EQ(5, result.size());
  EXPECT_EQ(1, result[1]); // 0 + 1
  EXPECT_EQ(2, result[2]); // 1 + 1
  EXPECT_EQ(1, result[3]); // 0 + 1
  EXPECT_EQ(1, result[4]); // 0 + 1
  EXPECT_EQ(1, result[5]); // 0 + 1
}

TEST_F(SimosRevisedTest, GetWhiteCardsWithNonZeroValues_AllZeros) {
  WhiteCardsMapType whiteCards = {{1, 0}, {2, 0}, {3, 0}};
  SimosRevisedTestable simos(testRanks, whiteCards, 4.0, 1);
  auto result = simos.getWhiteCardsWithNonZeroValues();

  EXPECT_EQ(3, result.size());
  for (const auto &wc : result) {
    EXPECT_EQ(1, wc.second);
  }
}

// ============================================================================
// Tests for getWhiteCardsCount
// Calculate total intervals: e = sum of e_r
// ============================================================================

TEST_F(SimosRevisedTest, GetWhiteCardsCount_TestData) {
  SimosRevisedTestable simos(testRanks, testWhiteCards, testZRatio,
                             testDecimals);
  auto whiteCardsNonZero = simos.getWhiteCardsWithNonZeroValues();
  auto count = simos.getWhiteCardsCount(whiteCardsNonZero);

  // 1 + 2 + 1 + 1 + 1 = 6
  EXPECT_EQ(6, count);
}

TEST_F(SimosRevisedTest, GetWhiteCardsCount_Empty) {
  WhiteCardsMapType empty;
  SimosRevisedTestable simos(testRanks, empty, 4.0, 1);
  auto count = simos.getWhiteCardsCount(empty);

  EXPECT_EQ(0, count);
}

// ============================================================================
// Tests for getRatio
// Calculate unit ratio: u = (z-1)/e
// ============================================================================

TEST_F(SimosRevisedTest, GetRatio_TestData) {
  SimosRevisedTestable simos(testRanks, testWhiteCards, testZRatio,
                             testDecimals);
  auto ratio = simos.getRatio(6);

  // u = (6.5 - 1) / 6 = 5.5 / 6 = 0.916666...
  EXPECT_NEAR(0.916666, ratio, EPSILON);
}

TEST_F(SimosRevisedTest, GetRatio_SimpleCase) {
  SimosRevisedTestable simos(testRanks, testWhiteCards, 4.0, 1);
  auto ratio = simos.getRatio(3);

  // u = (4 - 1) / 3 = 1.0
  EXPECT_NEAR(1.0, ratio, EPSILON);
}

// ============================================================================
// Tests for getCriteriaCount
// ============================================================================

TEST_F(SimosRevisedTest, GetCriteriaCount_TestData) {
  SimosRevisedTestable simos(testRanks, testWhiteCards, testZRatio,
                             testDecimals);
  auto count = simos.getCriteriaCount();

  // 3 + 1 + 4 + 1 + 2 + 1 = 12 criteria
  EXPECT_EQ(12, count);
}

TEST_F(SimosRevisedTest, GetCriteriaCount_SingleRank) {
  RanksMapType ranks = {{1, {"a", "b", "c"}}};
  WhiteCardsMapType whiteCards;
  SimosRevisedTestable simos(ranks, whiteCards, 2.0, 1);
  auto count = simos.getCriteriaCount();

  EXPECT_EQ(3, count);
}

// ============================================================================
// Tests for getNonNormalizedWeights
// Formula: k(r) = 1 + u(e_0 + ... + e_{r-1})
// ============================================================================

TEST_F(SimosRevisedTest, GetNonNormalizedWeights_TestData) {
  SimosRevisedTestable simos(testRanks, testWhiteCards, testZRatio,
                             testDecimals);
  auto whiteCardsNonZero = simos.getWhiteCardsWithNonZeroValues();
  auto ratio = simos.getRatio(6);
  auto weights = simos.getNonNormalizedWeights(whiteCardsNonZero, ratio);

  EXPECT_NEAR(1.00, weights[1], 0.01); // k(1) = 1.00
  EXPECT_NEAR(1.92, weights[2], 0.01); // k(2) = 1 + 0.916666*1 = 1.92
  EXPECT_NEAR(3.75, weights[3], 0.01); // k(3) = 1 + 0.916666*3 = 3.75
  EXPECT_NEAR(4.67, weights[4], 0.01); // k(4) = 1 + 0.916666*4 = 4.67
  EXPECT_NEAR(5.58, weights[5], 0.01); // k(5) = 1 + 0.916666*5 = 5.58
  EXPECT_NEAR(6.50, weights[6], 0.01); // k(6) = 1 + 0.916666*6 = 6.50
}

TEST_F(SimosRevisedTest, GetNonNormalizedWeights_UniformIntervals) {
  RanksMapType ranks = {{1, {"a"}}, {2, {"b"}}, {3, {"c"}}, {4, {"d"}}};
  WhiteCardsMapType whiteCards = {{1, 0}, {2, 0}, {3, 0}};
  SimosRevisedTestable simos(ranks, whiteCards, 4.0, 1);

  auto whiteCardsNonZero = simos.getWhiteCardsWithNonZeroValues();
  auto ratio = simos.getRatio(3); // u = 1.0
  auto weights = simos.getNonNormalizedWeights(whiteCardsNonZero, ratio);

  EXPECT_NEAR(1.0, weights[1], EPSILON);
  EXPECT_NEAR(2.0, weights[2], EPSILON);
  EXPECT_NEAR(3.0, weights[3], EPSILON);
  EXPECT_NEAR(4.0, weights[4], EPSILON);
}

// ============================================================================
// Tests for getWeightsSum
// ============================================================================

TEST_F(SimosRevisedTest, GetWeightsSum_TestData) {
  SimosRevisedTestable simos(testRanks, testWhiteCards, testZRatio,
                             testDecimals);
  auto whiteCardsNonZero = simos.getWhiteCardsWithNonZeroValues();
  auto ratio = simos.getRatio(6);
  auto weights = simos.getNonNormalizedWeights(whiteCardsNonZero, ratio);
  auto sum = simos.getWeightsSum(weights);

  // 3*1.00 + 1*1.92 + 4*3.75 + 1*4.67 + 2*5.58 + 1*6.50 = 42.25
  EXPECT_NEAR(42.25, sum, 0.01);
}

// ============================================================================
// Tests for getNormalizedWeights
// Formula: k_i = 100 * k'_i / K'
// ============================================================================

TEST_F(SimosRevisedTest, GetNormalizedWeights_TestData) {
  SimosRevisedTestable simos(testRanks, testWhiteCards, testZRatio,
                             testDecimals);
  auto whiteCardsNonZero = simos.getWhiteCardsWithNonZeroValues();
  auto ratio = simos.getRatio(6);
  auto nonNormalizedWeights =
      simos.getNonNormalizedWeights(whiteCardsNonZero, ratio);
  auto sum = simos.getWeightsSum(nonNormalizedWeights);
  auto normalizedWeights =
      simos.getNormalizedWeights(nonNormalizedWeights, sum);

  // k_i = 100 * k'_i / 42.25
  EXPECT_NEAR(2.366863905, normalizedWeights[1], 0.01);
  EXPECT_NEAR(4.544378698, normalizedWeights[2], 0.01);
  EXPECT_NEAR(8.875739645, normalizedWeights[3], 0.01);
  EXPECT_NEAR(11.053254438, normalizedWeights[4], 0.01);
  EXPECT_NEAR(13.207100592, normalizedWeights[5], 0.01);
  EXPECT_NEAR(15.384615385, normalizedWeights[6], 0.01);
}

// ============================================================================
// Tests for getNormalizedWeightsTruncated
// Truncate to w decimal places: k''_i
// ============================================================================

TEST_F(SimosRevisedTest, GetNormalizedWeightsTruncated_OneDecimal) {
  WeightsMapType weights = {
      {1, 2.366863905}, {2, 4.544378698}, {3, 8.875739645}};

  RanksMapType dummyRanks = {{1, {"a"}}, {2, {"b"}}, {3, {"c"}}};
  WhiteCardsMapType dummyWhiteCards = {{1, 0}, {2, 0}};
  SimosRevisedTestable simos(dummyRanks, dummyWhiteCards, 3.0, 1);

  auto truncated = simos.getNormalizedWeightsTruncated(weights);

  EXPECT_NEAR(2.3, truncated[1], EPSILON);
  EXPECT_NEAR(4.5, truncated[2], EPSILON);
  EXPECT_NEAR(8.8, truncated[3], EPSILON);
}

TEST_F(SimosRevisedTest, GetNormalizedWeightsTruncated_ZeroDecimals) {
  WeightsMapType weights = {
      {1, 2.366863905}, {2, 4.544378698}, {3, 8.875739645}};

  RanksMapType dummyRanks = {{1, {"a"}}, {2, {"b"}}, {3, {"c"}}};
  WhiteCardsMapType dummyWhiteCards = {{1, 0}, {2, 0}};
  SimosRevisedTestable simos(dummyRanks, dummyWhiteCards, 3.0, 0);

  auto truncated = simos.getNormalizedWeightsTruncated(weights);

  EXPECT_NEAR(2.0, truncated[1], EPSILON);
  EXPECT_NEAR(4.0, truncated[2], EPSILON);
  EXPECT_NEAR(8.0, truncated[3], EPSILON);
}

// ============================================================================
// Tests for getNearestUpWeights and getNearestDownWeights
// Dysfunction ratios: δ'_i and δ_i for rounding decisions
// ============================================================================

TEST_F(SimosRevisedTest, GetNearestUpWeights_Basic) {
  WeightsMapType normalized = {{1, 2.366863905}};
  WeightsMapType truncated = {{1, 2.3}};

  RanksMapType dummyRanks = {{1, {"a"}}};
  WhiteCardsMapType dummyWhiteCards;
  SimosRevisedTestable simos(dummyRanks, dummyWhiteCards, 2.0, 1);

  auto upWeights = simos.getNearestUpWeights(normalized, truncated);

  // δ'_i = [10^-1 - (2.366863905 - 2.3)] / 2.366863905
  //      = [0.1 - 0.066863905] / 2.366863905
  //      = 0.033136095 / 2.366863905
  //      ≈ 0.014
  EXPECT_NEAR(0.014, upWeights[1], 0.001);
}

TEST_F(SimosRevisedTest, GetNearestDownWeights_Basic) {
  WeightsMapType normalized = {{1, 2.366863905}};
  WeightsMapType truncated = {{1, 2.3}};

  RanksMapType dummyRanks = {{1, {"a"}}};
  WhiteCardsMapType dummyWhiteCards;
  SimosRevisedTestable simos(dummyRanks, dummyWhiteCards, 2.0, 1);

  auto downWeights = simos.getNearestDownWeights(normalized, truncated);

  // δ_i = (2.366863905 - 2.3) / 2.366863905
  //     = 0.066863905 / 2.366863905
  //     ≈ 0.02825
  EXPECT_NEAR(0.02825, downWeights[1], 0.001);
}

// ============================================================================
// Tests for getGreaterElements
// Build set M = {i | δ'_i > δ_i} for partition decisions
// ============================================================================

TEST_F(SimosRevisedTest, GetGreaterElements_Basic) {
  RanksMapType ranks = {{1, {"a"}}, {2, {"b"}}, {3, {"c"}}, {4, {"d"}}};

  WeightsMapType d1 = {{1, 0.5}, {2, 0.3}, {3, 0.7}, {4, 0.2}};
  WeightsMapType d2 = {{1, 0.4}, {2, 0.6}, {3, 0.1}, {4, 0.8}};

  WhiteCardsMapType dummyWhiteCards;
  SimosRevisedTestable simos(ranks, dummyWhiteCards, 2.0, 1);

  auto mRanks = simos.getGreaterElements(d1, d2);

  // M should contain: {1} (0.5 > 0.4) and {3} (0.7 > 0.1)
  EXPECT_EQ(2, mRanks.size());
  EXPECT_TRUE(mRanks.find(1) != mRanks.end());
  EXPECT_TRUE(mRanks.find(3) != mRanks.end());
  EXPECT_TRUE(mRanks.find(2) == mRanks.end());
  EXPECT_TRUE(mRanks.find(4) == mRanks.end());
}

// ============================================================================
// Tests for getSumGreaterElements
// ============================================================================

TEST_F(SimosRevisedTest, GetSumGreaterElements_Basic) {
  RanksMapType mRanks = {{1, {"a", "b"}}, {3, {"c", "d", "e"}}};

  WhiteCardsMapType dummyWhiteCards;
  SimosRevisedTestable simos(testRanks, dummyWhiteCards, 2.0, 1);

  auto sum = simos.getSumGreaterElements(mRanks);

  EXPECT_EQ(5, sum); // 2 + 3
}

// ============================================================================
// Tests for sortWeights
// Create sorted lists L (ascending δ_i) and L̄ (descending δ'_i)
// ============================================================================

TEST_F(SimosRevisedTest, SortWeights_Ascending) {
  WeightsMapType weights = {{1, 0.5}, {2, 0.1}, {3, 0.9}, {4, 0.3}};

  WhiteCardsMapType dummyWhiteCards;
  SimosRevisedTestable simos(testRanks, dummyWhiteCards, 2.0, 1);

  auto sorted = simos.sortWeights(weights, true);

  EXPECT_EQ(4, sorted.size());
  EXPECT_EQ(2, sorted[0].first); // 0.1
  EXPECT_EQ(4, sorted[1].first); // 0.3
  EXPECT_EQ(1, sorted[2].first); // 0.5
  EXPECT_EQ(3, sorted[3].first); // 0.9
}

TEST_F(SimosRevisedTest, SortWeights_Descending) {
  WeightsMapType weights = {{1, 0.5}, {2, 0.1}, {3, 0.9}, {4, 0.3}};

  WhiteCardsMapType dummyWhiteCards;
  SimosRevisedTestable simos(testRanks, dummyWhiteCards, 2.0, 1);

  auto sorted = simos.sortWeights(weights, false);

  EXPECT_EQ(4, sorted.size());
  EXPECT_EQ(3, sorted[0].first); // 0.9
  EXPECT_EQ(1, sorted[1].first); // 0.5
  EXPECT_EQ(4, sorted[2].first); // 0.3
  EXPECT_EQ(2, sorted[3].first); // 0.1
}

// ============================================================================
// Tests for convertToRanksWeight
// ============================================================================

TEST_F(SimosRevisedTest, ConvertToRanksWeight_Basic) {
  RanksMapType ranks = {{1, {"a", "b"}}, {2, {"c"}}, {3, {"d", "e", "f"}}};

  WeightsMapType weights = {{1, 2.5}, {2, 5.0}, {3, 7.5}};

  WhiteCardsMapType dummyWhiteCards;
  SimosRevisedTestable simos(ranks, dummyWhiteCards, 2.0, 1);

  auto result = simos.convertToRanksWeight(weights);

  EXPECT_EQ(6, result.size()); // 2 + 1 + 3

  // Check first rank
  EXPECT_EQ(1, result[0].rank);
  EXPECT_EQ("a", result[0].criterion);
  EXPECT_NEAR(2.5, result[0].weight, EPSILON);

  EXPECT_EQ(1, result[1].rank);
  EXPECT_EQ("b", result[1].criterion);
  EXPECT_NEAR(2.5, result[1].weight, EPSILON);

  // Check second rank
  EXPECT_EQ(2, result[2].rank);
  EXPECT_EQ("c", result[2].criterion);
  EXPECT_NEAR(5.0, result[2].weight, EPSILON);
}

// ============================================================================
// Integration test: Full generateWeights()
// ============================================================================

TEST_F(SimosRevisedTest, GenerateWeights_TestData) {
  SimosRevised simos(testRanks, testWhiteCards, testZRatio, testDecimals);
  auto weights = simos.generateWeights();

  // Should have 12 criteria total
  EXPECT_EQ(12, weights.size());

  // Sum should be 100
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);

  // Check some individual weights
  // Note: Exact values depend on rounding algorithm
  for (const auto &w : weights) {
    EXPECT_GT(w.weight, 0.0);
    EXPECT_LE(w.weight, 16.0); // Max weight should be around 15.3
  }
}

TEST_F(SimosRevisedTest, GenerateWeights_UniformWeights) {
  // Simple case: 4 criteria with equal intervals
  RanksMapType ranks = {{1, {"a"}}, {2, {"b"}}, {3, {"c"}}, {4, {"d"}}};
  WhiteCardsMapType whiteCards = {{1, 0}, {2, 0}, {3, 0}};

  SimosRevised simos(ranks, whiteCards, 4.0, 1);
  auto weights = simos.generateWeights();

  EXPECT_EQ(4, weights.size());

  // Sum should be 100
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);

  // Weights should be in ascending order
  EXPECT_LT(weights[0].weight, weights[1].weight);
  EXPECT_LT(weights[1].weight, weights[2].weight);
  EXPECT_LT(weights[2].weight, weights[3].weight);
}

TEST_F(SimosRevisedTest, GenerateWeights_ExAequoCriteria) {
  // Test with multiple criteria at same rank (ex aequo)
  RanksMapType ranks = {
      {1, {"a", "b", "c"}}, // 3 criteria with same importance
      {2, {"d", "e"}}       // 2 criteria with same importance
  };
  WhiteCardsMapType whiteCards = {{1, 2}}; // 2 white cards between ranks

  SimosRevised simos(ranks, whiteCards, 4.0, 1);
  auto weights = simos.generateWeights();

  EXPECT_EQ(5, weights.size());

  // All criteria in rank 1 should have same weight
  EXPECT_NEAR(weights[0].weight, weights[1].weight, 0.2);
  EXPECT_NEAR(weights[1].weight, weights[2].weight, 0.2);

  // All criteria in rank 2 should have same weight
  EXPECT_NEAR(weights[3].weight, weights[4].weight, 0.2);

  // Sum should be 100
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);
}

TEST_F(SimosRevisedTest, GenerateWeights_ZeroDecimals) {
  SimosRevised simos(testRanks, testWhiteCards, testZRatio, 0);
  auto weights = simos.generateWeights();

  EXPECT_EQ(12, weights.size());

  // All weights should be integers
  for (const auto &w : weights) {
    EXPECT_NEAR(round(w.weight), w.weight, EPSILON);
  }

  // Sum should be 100
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);
}

TEST_F(SimosRevisedTest, GenerateWeights_TwoDecimals) {
  SimosRevised simos(testRanks, testWhiteCards, testZRatio, 2);
  auto weights = simos.generateWeights();

  EXPECT_EQ(12, weights.size());

  // Sum should be 100
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.01);
}

// ============================================================================
// Edge cases
// ============================================================================

TEST_F(SimosRevisedTest, GenerateWeights_SingleCriterion) {
  RanksMapType ranks = {{1, {"a"}}};
  WhiteCardsMapType whiteCards;

  // This is an edge case - with single criterion, z ratio doesn't matter much
  SimosRevised simos(ranks, whiteCards, 1.0, 1);
  auto weights = simos.generateWeights();

  EXPECT_EQ(1, weights.size());
  EXPECT_NEAR(100.0, weights[0].weight, 0.1);
}

TEST_F(SimosRevisedTest, GenerateWeights_LargeZRatio) {
  RanksMapType ranks = {{1, {"a"}}, {2, {"b"}}, {3, {"c"}}};
  WhiteCardsMapType whiteCards = {{1, 0}, {2, 0}};

  SimosRevised simos(ranks, whiteCards, 10.0, 1);
  auto weights = simos.generateWeights();

  EXPECT_EQ(3, weights.size());

  // Last criterion should be much heavier than first
  EXPECT_GT(weights[2].weight, weights[0].weight * 5);

  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);
}

// ============================================================================
// Main function
// ============================================================================

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
