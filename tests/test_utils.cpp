#include "../include/utils.hpp"
#include <gtest/gtest.h>
#include <cmath>

// Test fixture for SimosUtils tests
class SimosUtilsTest : public ::testing::Test {
protected:
    const double EPSILON = 1e-9;
};

// ============================================================================
// Tests for roundToXDecimals
// ============================================================================

TEST_F(SimosUtilsTest, RoundToXDecimals_ZeroDecimals) {
    EXPECT_DOUBLE_EQ(3.0, SimosUtils::roundToXDecimals(2.5, 0));
    EXPECT_DOUBLE_EQ(2.0, SimosUtils::roundToXDecimals(2.4, 0));
    EXPECT_DOUBLE_EQ(3.0, SimosUtils::roundToXDecimals(2.6, 0));
    EXPECT_DOUBLE_EQ(-3.0, SimosUtils::roundToXDecimals(-2.5, 0));
}

TEST_F(SimosUtilsTest, RoundToXDecimals_OneDecimal) {
    EXPECT_NEAR(2.4, SimosUtils::roundToXDecimals(2.366863905, 1), EPSILON);
    EXPECT_NEAR(2.4, SimosUtils::roundToXDecimals(2.35, 1), EPSILON);
    EXPECT_NEAR(2.3, SimosUtils::roundToXDecimals(2.34, 1), EPSILON);
    EXPECT_NEAR(8.9, SimosUtils::roundToXDecimals(8.875739645, 1), EPSILON);
}

TEST_F(SimosUtilsTest, RoundToXDecimals_TwoDecimals) {
    EXPECT_NEAR(2.37, SimosUtils::roundToXDecimals(2.366863905, 2), EPSILON);
    EXPECT_NEAR(8.88, SimosUtils::roundToXDecimals(8.875739645, 2), EPSILON);
    EXPECT_NEAR(11.05, SimosUtils::roundToXDecimals(11.053254438, 2), EPSILON);
}

TEST_F(SimosUtilsTest, RoundToXDecimals_NegativeNumbers) {
    EXPECT_NEAR(-2.4, SimosUtils::roundToXDecimals(-2.366863905, 1), EPSILON);
    EXPECT_NEAR(-8.9, SimosUtils::roundToXDecimals(-8.875739645, 1), EPSILON);
}

// ============================================================================
// Tests for truncateToXDecimals
// ============================================================================

TEST_F(SimosUtilsTest, TruncateToXDecimals_ZeroDecimals) {
    EXPECT_DOUBLE_EQ(2.0, SimosUtils::truncateToXDecimals(2.9, 0));
    EXPECT_DOUBLE_EQ(2.0, SimosUtils::truncateToXDecimals(2.1, 0));
    EXPECT_DOUBLE_EQ(15.0, SimosUtils::truncateToXDecimals(15.384615385, 0));
    EXPECT_DOUBLE_EQ(-2.0, SimosUtils::truncateToXDecimals(-2.9, 0));
}

TEST_F(SimosUtilsTest, TruncateToXDecimals_OneDecimal) {
    EXPECT_NEAR(2.3, SimosUtils::truncateToXDecimals(2.366863905, 1), EPSILON);
    EXPECT_NEAR(8.8, SimosUtils::truncateToXDecimals(8.875739645, 1), EPSILON);
    EXPECT_NEAR(11.0, SimosUtils::truncateToXDecimals(11.053254438, 1), EPSILON);
    EXPECT_NEAR(15.3, SimosUtils::truncateToXDecimals(15.384615385, 1), EPSILON);
}

TEST_F(SimosUtilsTest, TruncateToXDecimals_TwoDecimals) {
    EXPECT_NEAR(2.36, SimosUtils::truncateToXDecimals(2.366863905, 2), EPSILON);
    EXPECT_NEAR(8.87, SimosUtils::truncateToXDecimals(8.875739645, 2), EPSILON);
    EXPECT_NEAR(11.05, SimosUtils::truncateToXDecimals(11.053254438, 2), EPSILON);
}

TEST_F(SimosUtilsTest, TruncateToXDecimals_NegativeNumbers) {
    EXPECT_NEAR(-2.3, SimosUtils::truncateToXDecimals(-2.366863905, 1), EPSILON);
    EXPECT_NEAR(-8.8, SimosUtils::truncateToXDecimals(-8.875739645, 1), EPSILON);
}

// ============================================================================
// Tests for roundUpToXDecimals
// ============================================================================

TEST_F(SimosUtilsTest, RoundUpToXDecimals_ZeroDecimals) {
    EXPECT_DOUBLE_EQ(3.0, SimosUtils::roundUpToXDecimals(2.1, 0));
    EXPECT_DOUBLE_EQ(3.0, SimosUtils::roundUpToXDecimals(2.9, 0));
    EXPECT_DOUBLE_EQ(16.0, SimosUtils::roundUpToXDecimals(15.384615385, 0));
    EXPECT_DOUBLE_EQ(-2.0, SimosUtils::roundUpToXDecimals(-2.9, 0));
}

TEST_F(SimosUtilsTest, RoundUpToXDecimals_OneDecimal) {
    EXPECT_NEAR(2.4, SimosUtils::roundUpToXDecimals(2.31, 1), EPSILON);
    EXPECT_NEAR(2.4, SimosUtils::roundUpToXDecimals(2.366863905, 1), EPSILON);
    EXPECT_NEAR(8.9, SimosUtils::roundUpToXDecimals(8.81, 1), EPSILON);
    EXPECT_NEAR(8.9, SimosUtils::roundUpToXDecimals(8.875739645, 1), EPSILON);
}

TEST_F(SimosUtilsTest, RoundUpToXDecimals_TwoDecimals) {
    EXPECT_NEAR(2.37, SimosUtils::roundUpToXDecimals(2.361, 2), EPSILON);
    EXPECT_NEAR(2.37, SimosUtils::roundUpToXDecimals(2.366863905, 2), EPSILON);
    EXPECT_NEAR(8.88, SimosUtils::roundUpToXDecimals(8.871, 2), EPSILON);
}

TEST_F(SimosUtilsTest, RoundUpToXDecimals_ExactValues) {
    // When value is already at the decimal precision, should remain same
    EXPECT_NEAR(2.4, SimosUtils::roundUpToXDecimals(2.4, 1), EPSILON);
    EXPECT_NEAR(8.9, SimosUtils::roundUpToXDecimals(8.9, 1), EPSILON);
}

// ============================================================================
// Tests for getWeightsSum
// ============================================================================

TEST_F(SimosUtilsTest, GetWeightsSum_EmptyVector) {
    std::vector<RankWeight> weights;
    EXPECT_DOUBLE_EQ(0.0, SimosUtils::getWeightsSum(weights));
}

TEST_F(SimosUtilsTest, GetWeightsSum_SingleElement) {
    std::vector<RankWeight> weights = {
        {1, "a", 5.5}
    };
    EXPECT_NEAR(5.5, SimosUtils::getWeightsSum(weights), EPSILON);
}

TEST_F(SimosUtilsTest, GetWeightsSum_MultipleElements) {
    std::vector<RankWeight> weights = {
        {1, "a", 2.4},
        {1, "b", 2.4},
        {1, "c", 2.4},
        {2, "d", 4.5},
        {3, "e", 8.9}
    };
    EXPECT_NEAR(20.6, SimosUtils::getWeightsSum(weights), EPSILON);
}

TEST_F(SimosUtilsTest, GetWeightsSum_TestData) {
    // Test data
    std::vector<RankWeight> weights = {
        {1, "c", 2.4},
        {1, "g", 2.4},
        {1, "l", 2.4},
        {2, "d", 4.5},
        {3, "b", 8.9},
        {3, "f", 8.9},
        {3, "i", 8.9},
        {3, "j", 8.9},
        {4, "e", 11.0},
        {5, "a", 13.2},
        {5, "h", 13.2},
        {6, "k", 15.3}
    };
    EXPECT_NEAR(100.0, SimosUtils::getWeightsSum(weights), 0.1);
}

// ============================================================================
// Main function
// ============================================================================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
