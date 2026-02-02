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

  SimosRevised simosRevised(ranks, whiteCards, 0.06, 0);

  const auto weights = simosRevised.generateWeights();

  const auto total = SimosUtils::getWeightsSum(weights);
  for (const auto &w : weights) {
    std::cout << w.rank << ": " << w.criterion << " : " << w.weight
              << std::endl;
  }

  std::cout << "Total: " << total << std::endl;

  return 0;
}
