#include "../include/simos_revised.hpp"
#include "../include/utils.hpp"
#include <gtest/gtest.h>

// Wrapper struct for SIMOS test scenarios
struct SimosTestScenario {
  std::string name;
  double zRatio;
  int decimals;
  RanksMapType ranks;
  WhiteCardsMapType whiteCards;
};

class SimosScenariosTest : public ::testing::Test {
protected:
  const double EPSILON = 1e-6;
  std::vector<SimosTestScenario> scenarios;

  void SetUp() override {
    // Example 1: Budget Laptop
    scenarios.push_back({
        "Budget Laptop",
        5.0,
        2,
        {{1, {"RGB_Lights"}}, {2, {"RAM"}}, {3, {"GPU_Power"}}, {4, {"Price"}}},
        {{1, 0}, {2, 0}, {3, 0}}
    });

    // Example 2: Apartment
    scenarios.push_back({
        "Apartment",
        20.0,
        1,
        {{1, {"Pool"}}, {2, {"View"}}, {3, {"Rent"}}, {4, {"Safety"}}},
        {{1, 0}, {2, 0}, {3, 4}} // Gap of 4 cards between Rent and Safety
    });

    // Example 3: Hiring Junior
    scenarios.push_back({
        "Hiring Junior",
        2.5,
        3,
        {{1, {"Location"}}, {2, {"Degree"}}, {3, {"Coding_Basics"}}, {4, {"Potential"}}},
        {{1, 0}, {2, 0}, {3, 0}}
    });

    // Example 4: Database
    scenarios.push_back({
        "Database",
        8.0,
        0,
        {{1, {"Ease_Use"}}, {2, {"Cost"}}, {3, {"Speed"}}, {4, {"Integrity"}}},
        {{1, 0}, {2, 0}, {3, 0}}
    });

    // Example 5: ER Triage
    scenarios.push_back({
        "ER Triage",
        100.0,
        2,
        {{1, {"Wait_Time"}}, {2, {"Insurance"}}, {3, {"Pain_Level"}}, {4, {"Vitals"}}},
        {{1, 0}, {2, 0}, {3, 0}}
    });

    // Example 6: Family Car
    scenarios.push_back({
        "Family Car",
        6.0,
        4,
        {{1, {"Color"}}, {2, {"Brand"}}, {3, {"Space"}}, {4, {"Safety"}}},
        {{1, 0}, {2, 1}, {3, 0}} // Gap separating Vanity (1-2) from Utility (3-4)
    });

    // Example 7: Restaurant
    scenarios.push_back({
        "Restaurant",
        4.0,
        1,
        {{1, {"Decor"}}, {2, {"Distance"}}, {3, {"Hygiene"}}, {4, {"Taste"}}},
        {{1, 0}, {2, 0}, {3, 0}}
    });

    // Example 8: Software Vendor
    scenarios.push_back({
        "Software Vendor",
        3.5,
        2,
        {{1, {"UI"}}, {2, {"Features"}}, {3, {"Support"}}, {4, {"Compliance"}}},
        {{1, 0}, {2, 0}, {3, 0}}
    });

    // Example 9: Smartphone
    scenarios.push_back({
        "Smartphone",
        12.0,
        3,
        {{1, {"Speaker"}}, {2, {"Brand"}}, {3, {"Camera"}}, {4, {"Battery"}}},
        {{1, 0}, {2, 3}, {3, 0}} // Big jump of 3 cards
    });

    // Example 10: University
    scenarios.push_back({
        "University",
        1.5,
        2,
        {{1, {"Weather"}}, {2, {"Campus"}}, {3, {"Cost"}}, {4, {"Reputation"}}},
        {{1, 0}, {2, 0}, {3, 0}}
    });
  }
};

// Test weight sum equals 100 for all scenarios
TEST_F(SimosScenariosTest, AllScenariosWeightSumEquals100) {
  for (const auto &scenario : scenarios) {
    SimosRevised simos(scenario.ranks, scenario.whiteCards, scenario.zRatio, scenario.decimals);
    auto weights = simos.generateWeights();
    auto sum = SimosUtils::getWeightsSum(weights);

    EXPECT_NEAR(100.0, sum, 0.1) << "Failed for scenario: " << scenario.name;
  }
}

// Individual tests for each scenario
TEST_F(SimosScenariosTest, BudgetLaptop_WeightSum) {
  const auto &scenario = scenarios[0];
  SimosRevised simos(scenario.ranks, scenario.whiteCards, scenario.zRatio, scenario.decimals);
  auto weights = simos.generateWeights();
  
  EXPECT_EQ(4, weights.size());
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);
}

TEST_F(SimosScenariosTest, Apartment_WeightSum) {
  const auto &scenario = scenarios[1];
  SimosRevised simos(scenario.ranks, scenario.whiteCards, scenario.zRatio, scenario.decimals);
  auto weights = simos.generateWeights();
  
  EXPECT_EQ(4, weights.size());
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);
}

TEST_F(SimosScenariosTest, HiringJunior_WeightSum) {
  const auto &scenario = scenarios[2];
  SimosRevised simos(scenario.ranks, scenario.whiteCards, scenario.zRatio, scenario.decimals);
  auto weights = simos.generateWeights();
  
  EXPECT_EQ(4, weights.size());
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);
}

TEST_F(SimosScenariosTest, Database_WeightSum) {
  const auto &scenario = scenarios[3];
  SimosRevised simos(scenario.ranks, scenario.whiteCards, scenario.zRatio, scenario.decimals);
  auto weights = simos.generateWeights();
  
  EXPECT_EQ(4, weights.size());
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);
}

TEST_F(SimosScenariosTest, ERTriage_WeightSum) {
  const auto &scenario = scenarios[4];
  SimosRevised simos(scenario.ranks, scenario.whiteCards, scenario.zRatio, scenario.decimals);
  auto weights = simos.generateWeights();
  
  EXPECT_EQ(4, weights.size());
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);
}

TEST_F(SimosScenariosTest, FamilyCar_WeightSum) {
  const auto &scenario = scenarios[5];
  SimosRevised simos(scenario.ranks, scenario.whiteCards, scenario.zRatio, scenario.decimals);
  auto weights = simos.generateWeights();
  
  EXPECT_EQ(4, weights.size());
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);
}

TEST_F(SimosScenariosTest, Restaurant_WeightSum) {
  const auto &scenario = scenarios[6];
  SimosRevised simos(scenario.ranks, scenario.whiteCards, scenario.zRatio, scenario.decimals);
  auto weights = simos.generateWeights();
  
  EXPECT_EQ(4, weights.size());
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);
}

TEST_F(SimosScenariosTest, SoftwareVendor_WeightSum) {
  const auto &scenario = scenarios[7];
  SimosRevised simos(scenario.ranks, scenario.whiteCards, scenario.zRatio, scenario.decimals);
  auto weights = simos.generateWeights();
  
  EXPECT_EQ(4, weights.size());
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);
}

TEST_F(SimosScenariosTest, Smartphone_WeightSum) {
  const auto &scenario = scenarios[8];
  SimosRevised simos(scenario.ranks, scenario.whiteCards, scenario.zRatio, scenario.decimals);
  auto weights = simos.generateWeights();
  
  EXPECT_EQ(4, weights.size());
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);
}

TEST_F(SimosScenariosTest, University_WeightSum) {
  const auto &scenario = scenarios[9];
  SimosRevised simos(scenario.ranks, scenario.whiteCards, scenario.zRatio, scenario.decimals);
  auto weights = simos.generateWeights();
  
  EXPECT_EQ(4, weights.size());
  auto sum = SimosUtils::getWeightsSum(weights);
  EXPECT_NEAR(100.0, sum, 0.1);
}

// Test that all weights are positive
TEST_F(SimosScenariosTest, AllWeightsPositive) {
  for (const auto &scenario : scenarios) {
    SimosRevised simos(scenario.ranks, scenario.whiteCards, scenario.zRatio, scenario.decimals);
    auto weights = simos.generateWeights();
    
    for (const auto &w : weights) {
      EXPECT_GT(w.weight, 0.0) << "Failed for scenario: " << scenario.name 
                               << ", criterion: " << w.criterion;
    }
  }
}

// Test that weights are in ascending order (since higher rank = more important)
TEST_F(SimosScenariosTest, WeightsInAscendingOrder) {
  for (const auto &scenario : scenarios) {
    SimosRevised simos(scenario.ranks, scenario.whiteCards, scenario.zRatio, scenario.decimals);
    auto weights = simos.generateWeights();
    
    for (size_t i = 1; i < weights.size(); ++i) {
      EXPECT_LE(weights[i - 1].weight, weights[i].weight) 
          << "Failed for scenario: " << scenario.name 
          << ", comparing rank " << weights[i - 1].rank 
          << " with rank " << weights[i].rank;
    }
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
