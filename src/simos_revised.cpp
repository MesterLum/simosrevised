#include "../include/simos_revised.hpp"
#include "../include/utils.hpp"
#include <iostream>

SimosRevised::SimosRevised(const RanksMapType &ranks,
                           const WhiteCardsMapType &whiteCards,
                           DataPrecisionType zRatio, unsigned int decimals)
    : ranks(ranks), whiteCards(whiteCards), zRatio(zRatio), decimals(decimals) {
}

void SimosRevised::generateWeights() {

  WhiteCardsMapType whiteCardsNonZero = getWhiteCardsWithNonZeroValues();
  unsigned int whiteCardsNonZeroCount = getWhiteCardsCount(whiteCardsNonZero);
  DataPrecisionType ratio = getRatio(whiteCardsNonZeroCount);

  if (debug) {
    std::cout << "Ratio:" << ratio << std::endl;
    std::cout << "Total white cards: " << whiteCardsNonZeroCount << std::endl;
  }

  WeightsMapType nonNormalizedWeights =
      getNonNormalizedWeights(whiteCardsNonZero, ratio);

  if (debug) {
    SimosUtils::printWeights(nonNormalizedWeights);
  }
}

WeightsMapType
SimosRevised::getNonNormalizedWeights(const WhiteCardsMapType &whiteCards,
                                      DataPrecisionType ratio) {
  WeightsMapType weights;
  bool firstSkipped = false;
  int idx = 1;
  for (const auto &r : ranks) {
    if (!firstSkipped) {
      weights[r.first] = 1.0;
      firstSkipped = true;
      continue;
    }
    int whiteCardsCount = 0;
    int internalIdx = 0;
    for (const auto &wc : whiteCards) {
      whiteCardsCount += wc.second;
      internalIdx++;
      if (idx == internalIdx)
        break;
    }
    weights[r.first] =
        SimosUtils::roundToXDecimals(1 + whiteCardsCount * ratio, 2);
    idx++;
  }
  return weights;
}

unsigned int SimosRevised::getCriteriaCount() {
  unsigned int total = 0;
  for (const auto &rank : ranks) {
    total += rank.second.size();
  }
  return total;
}

WhiteCardsMapType SimosRevised::getWhiteCardsWithNonZeroValues() {
  WhiteCardsMapType nonZeroWhiteCards;
  for (const auto &wc : whiteCards) {
    nonZeroWhiteCards[wc.first] = wc.second + 1;
  }
  return nonZeroWhiteCards;
}

unsigned int
SimosRevised::getWhiteCardsCount(const WhiteCardsMapType &whiteCards) {
  unsigned int total = 0;

  for (const auto &wc : whiteCards) {
    total += wc.second;
  }

  return total;
}

DataPrecisionType SimosRevised::getRatio(unsigned int whiteCardsCount) {
  return SimosUtils::truncateToXDecimals((zRatio - 1) / whiteCardsCount,
                                         decimalsToRetain);
}
