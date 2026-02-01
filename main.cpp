#include "./include/simos_revised.hpp"
int main() {
  RanksMapType ranks = {
      {1, RankGroupType{"c", "g", "l"}},      {2, RankGroupType{"d"}},
      {3, RankGroupType{"b", "f", "i", "j"}}, {4, RankGroupType{"e"}},
      {5, RankGroupType{"a", "h"}},           {6, RankGroupType{"k"}},
  };

  WhiteCardsMapType whiteCards = {
      {1, 0}, {2, 1}, {3, 0}, {4, 0}, {5, 0},
  };

  SimosRevised simosRevised(ranks, whiteCards, 6.5, 1);

  const auto weights = simosRevised.generateWeights();

  return 0;
}
