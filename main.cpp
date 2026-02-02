#include <random>

#include "./include/simos_revised.hpp"
#include "include/utils.hpp"

#include <iostream>
int main() {
  RanksMapType ranks = {
      {1, RankGroupType{"c", "g", "l"}},      {2, RankGroupType{"d"}},
      {3, RankGroupType{"b", "f", "i", "j"}}, {4, RankGroupType{"e"}},
      {5, RankGroupType{"a", "h"}},           {6, RankGroupType{"k"}},
  };

  WhiteCardsMapType whiteCards = {
      {1, 0}, {2, 1}, {3, 0}, {4, 0}, {5, 0},
  };

  for (int i = 0; i <= 10000; i++) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist1(0.0, 100.0);
    std::uniform_int_distribution<int> dist2(0, 8);

    float randomParam1 = dist1(gen);
    int randomParam2 = dist2(gen);

    SimosRevised simosRevised(ranks, whiteCards, randomParam1, randomParam2);

    const auto weights = simosRevised.generateWeights();

    const auto total = SimosUtils::getWeightsSum(weights);
    // for (const auto &w : weights) {
    //   std::cout << w.rank << ": " << w.criterion << " : " << w.weight
    //             << std::endl;
    // }
    std::cout << "Total: " << total << " Z Ratio: " << randomParam1
              << " Decimals: " << randomParam2 << std::endl;
  }

  return 0;
}
