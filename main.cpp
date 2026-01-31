#include "./include/simos_revised.hpp"
int main() {
  RanksMapType ranks = {
      {1, RankGroupType{"c", "g", "l"}},      {2, RankGroupType{"d"}},
      {4, RankGroupType{"b", "f", "i", "j"}}, {5, RankGroupType{"e"}},
      {6, RankGroupType{"a", "h"}},           {7, RankGroupType{"k"}},
  };

  WhiteCardsMapType whiteCards = {
      {1, 0}, {2, 1}, {3, 0}, {4, 0}, {5, 0},
  };

  SimosRevised simosRevised(ranks, whiteCards, 6.5, 1);

  simosRevised.generateWeights();
  return 0;
}
