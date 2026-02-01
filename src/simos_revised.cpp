#include "../include/simos_revised.hpp"
#include "../include/utils.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

SimosRevised::SimosRevised(const RanksMapType &ranks,
                           const WhiteCardsMapType &whiteCards,
                           DataPrecisionType zRatio, unsigned int decimals)
    : ranks(ranks), whiteCards(whiteCards), zRatio(zRatio), decimals(decimals) {
}

void SimosRevised::generateWeights() {

  const auto whiteCardsNonZero = getWhiteCardsWithNonZeroValues();
  const auto whiteCardsNonZeroCount = getWhiteCardsCount(whiteCardsNonZero);
  const auto ratio = getRatio(whiteCardsNonZeroCount);

  const auto nonNormalizedWeights =
      getNonNormalizedWeights(whiteCardsNonZero, ratio);

  const auto sumNonNormalizedWeights = getWeightsSum(nonNormalizedWeights);

  const auto normalizedWeights =
      getNormalizedWeights(nonNormalizedWeights, sumNonNormalizedWeights);

  const auto normalizedWeightsTruncated =
      getNormalizedWeightsTruncated(normalizedWeights);

  const auto sumNormalizedWeightsTruncated =
      getWeightsSum(normalizedWeightsTruncated);

  const auto difference = 100.0 - sumNormalizedWeightsTruncated;

  int vValue = pow(10, decimals) * difference;

  const auto d1 =
      getNearestUpWeights(normalizedWeights, normalizedWeightsTruncated);

  const auto d2 =
      getNearestDownWeights(normalizedWeights, normalizedWeightsTruncated);

  const auto mGreaterValues = getGreaterElements(d1, d2);

  const auto mElementsCount = getSumGreaterElements(mGreaterValues);
  const auto sortedD1 = sortWeights(d1);
  const auto sortedD2 = sortWeights(d2, false);

  const auto n = getCriteriaCount();

  RanksMapType f1, f2;
  if ((mElementsCount + vValue) <= n) {
    f1 = getF1Ranks(sortedD2, mGreaterValues,
                    mElementsCount + (n - vValue - mElementsCount),
                    mElementsCount);
    f2 = getF2Ranks(sortedD2, mGreaterValues, vValue);

  } else {
  }

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
    std::cout << "Difference: " << difference << std::endl;
    std::cout << "V Value: " << vValue << std::endl;
    std::cout << "==== Ratio  UP" << std::endl;
    SimosUtils::printWeights(d1, 9);
    std::cout << "==== Ratio  DOWN" << std::endl;
    SimosUtils::printWeights(d2, 9);
    std::cout << "==== M" << std::endl;
    for (const auto &m : mGreaterValues)
      std::cout << m.first << ": " << m.second.size() << std::endl;
    std::cout << "M Total: " << mElementsCount << std::endl;
    std::cout << "==== Sorted" << std::endl;
    SimosUtils::printUnorderedRanksData(sortedD1);
    std::cout << "==== Sorted" << std::endl;
    SimosUtils::printUnorderedRanksData(sortedD2);
    std::cout << "==== Sorted" << std::endl;
    SimosUtils::printRankGroups(f1);
    SimosUtils::printRankGroups(f2);
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
    std::cout << "White Cards: " << whiteCardsCount << std::endl;
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

WeightsMapType SimosRevised::getNearestUpWeights(
    const WeightsMapType &normalizedWeights,
    const WeightsMapType &normalizedWeightsTruncated) {
  WeightsMapType nearestWeights;
  for (const auto &r : ranks) {

    DataPrecisionType normalizedWeight = normalizedWeights.at(r.first);
    DataPrecisionType normalizedWeightTruncated =
        normalizedWeightsTruncated.at(r.first);
    nearestWeights[r.first] =
        (pow(10, -decimals) - (normalizedWeight - normalizedWeightTruncated)) /
        normalizedWeight;
  }

  return nearestWeights;
}

WeightsMapType SimosRevised::getNearestDownWeights(
    const WeightsMapType &normalizedWeights,
    const WeightsMapType &normalizedWeightsTruncated) {
  WeightsMapType nearestWeights;
  for (const auto &r : ranks) {
    DataPrecisionType normalizedWeight = normalizedWeights.at(r.first);
    DataPrecisionType normalizedWeightTruncated =
        normalizedWeightsTruncated.at(r.first);
    nearestWeights[r.first] =
        (normalizedWeight - normalizedWeightTruncated) / normalizedWeight;
  }

  return nearestWeights;
}

RanksMapType SimosRevised::getGreaterElements(const WeightsMapType &d1,
                                              const WeightsMapType &d2) {
  RanksMapType mRanks;
  for (const auto &r : ranks) {
    if (d1.at(r.first) > d2.at(r.first))
      mRanks[r.first] = r.second;
  }

  return mRanks;
}

unsigned int SimosRevised::getSumGreaterElements(const RanksMapType &mRanks) {
  unsigned int total = 0;
  for (const auto &r : mRanks) {
    total += r.second.size();
  }

  return total;
}

ListWeightType SimosRevised::sortWeights(const WeightsMapType &weights,
                                         bool asc) {
  ListWeightType orderedWeights;
  for (const auto &w : weights) {
    std::pair<unsigned int, DataPrecisionType> val;
    val.first = w.first;
    val.second = w.second;
    orderedWeights.push_back(val);
  }

  std::sort(orderedWeights.begin(), orderedWeights.end(),
            [asc](const auto &a, const auto &b) {
              if (asc)
                return a.second < b.second;
              return a.second > b.second;
            });

  return orderedWeights;
}

RanksMapType SimosRevised::getF1Ranks(const ListWeightType &weightsList,
                                      const RanksMapType &mList,
                                      unsigned int totalElements,
                                      unsigned int mCount) {
  RanksMapType ranksF1;

  // Map M values
  ranksF1.insert(mList.begin(), mList.end());

  unsigned int counter = mCount;
  for (int i = weightsList.size() - 1; i >= 0; i--) {
    if (counter >= totalElements)
      break;

    const auto criteriaGroupKey = weightsList[i].first;

    if (mList.find(criteriaGroupKey) != mList.end())
      continue;

    ranksF1[criteriaGroupKey] = ranks[criteriaGroupKey];

    counter += ranks[criteriaGroupKey].size();
  }

  return ranksF1;
}

RanksMapType SimosRevised::getF2Ranks(const ListWeightType &weightsList,
                                      const RanksMapType &mList,
                                      unsigned int totalElements) {
  RanksMapType ranksF2;

  unsigned int counter = 0;

  for (const auto &w : weightsList) {
    if (counter >= totalElements)
      break;
    if (mList.find(w.first) != mList.end())
      continue;

    ranksF2[w.first] = ranks[w.first];

    counter += ranks[w.first].size();
  }

  return ranksF2;
}
