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

  WeightsMapType nonNormalizedWeights =
      getNonNormalizedWeights(whiteCardsNonZero, ratio);

  DataPrecisionType sumNonNormalizedWeights =
      getWeightsSum(nonNormalizedWeights);

  WeightsMapType normalizedWeights =
      getNormalizedWeights(nonNormalizedWeights, sumNonNormalizedWeights);

  WeightsMapType normalizedWeightsTruncated =
      getNormalizedWeightsTruncated(normalizedWeights);

  DataPrecisionType sumNormalizedWeightsTruncated =
      getWeightsSum(normalizedWeightsTruncated);

  if (debug) {
    std::cout << "Ratio:" << ratio << std::endl;
    std::cout << "Total white cards: " << whiteCardsNonZeroCount << std::endl;
    SimosUtils::printWeights(nonNormalizedWeights);
    std::cout << "Sum non normalized weights: " << sumNonNormalizedWeights
              << std::endl;
    std::cout << "==== Normalized" << std::endl;
    SimosUtils::printWeights(normalizedWeights, 9);
    std::cout << "==== Normalized Truncated" << std::endl;
    SimosUtils::printWeights(normalizedWeightsTruncated, 1);
    std::cout << "Sum normalized weights: " << sumNormalizedWeightsTruncated
              << std::endl;
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

WeightsMapType
SimosRevised::getNormalizedWeights(const WeightsMapType &nonNormalizedWeights,
                                   DataPrecisionType sumNonNormalizedWeights) {
  WeightsMapType normalizedWeights;
  for (const auto &w : nonNormalizedWeights) {
    normalizedWeights[w.first] = 100 / sumNonNormalizedWeights * w.second;
  }
  return normalizedWeights;
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

DataPrecisionType SimosRevised::getWeightsSum(const WeightsMapType &weigths) {
  DataPrecisionType total = 0.0;

  for (const auto &w : weigths) {
    total += w.second * ranks[w.first].size();
  }

  return total;
}

WeightsMapType
SimosRevised::getNormalizedWeightsTruncated(const WeightsMapType &weights) {
  WeightsMapType truncatedWeights;
  for (const auto &w : weights) {
    truncatedWeights[w.first] =
        SimosUtils::truncateToXDecimals(w.second, decimals);
  }
  return truncatedWeights;
}
