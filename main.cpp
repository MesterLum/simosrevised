#include "./include/simos_revised.hpp"
#include "include/utils.hpp"

#include <iostream>
int main() {
  double z = 5.0;
  int decimals = 2;
  RanksMapType ranks = {{1, RankGroupType{"RGB_Lights"}},
                        {2, RankGroupType{"RAM"}},
                        {3, RankGroupType{"GPU_Power"}},
                        {4, RankGroupType{"Price"}}};
  WhiteCardsMapType whiteCards = {{1, 0}, {2, 0}, {3, 0}};

  SimosRevised simosRevised(ranks, whiteCards, z, decimals);

  const auto weights = simosRevised.generateWeights();

  const auto total = SimosUtils::getWeightsSum(weights);
  for (const auto &w : weights) {
    std::cout << w.rank << ": " << w.criterion << " : " << w.weight
              << std::endl;
  }
  std::cout << "Total: " << total << std::endl;

  return 0;
}
