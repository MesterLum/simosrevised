#include "../include/utils.hpp"
#include "../include/simos_revised.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>

DataPrecisionType SimosUtils::roundToXDecimals(DataPrecisionType value,
                                               unsigned int decimals) {
  if (decimals == 0) {
    return round(value);
  }
  return round(value * pow(10, decimals)) / pow(10, decimals);
}

DataPrecisionType SimosUtils::truncateToXDecimals(DataPrecisionType value,
                                                  unsigned int decimals) {
  return int(value * pow(10, decimals)) / pow(10, decimals);
}

void SimosUtils::printWeights(WeightsMapType weights, int decimals) {
  for (const auto &w : weights) {
    // std::cout << "[" << w.first << "] = " << w.second << std::endl;
    std::cout << "[" << w.first << "] = " << std::fixed
              << std::setprecision(decimals) << w.second << std::endl;
  }
}
void SimosUtils::printUnorderedRanksData(ListWeightType ratio) {
  for (const auto &val : ratio) {
    std::cout << "[" << val.first << "]" << " = " << val.second << std::endl;
  }
}
void SimosUtils::printRankGroups(const RanksMapType &ranks) {
  for (const auto &r : ranks) {
    std::cout << "[" << r.first << "] = [";
    for (const auto &value : r.second)
      std::cout << " " << value;
    std::cout << " ]" << std::endl;
  }
  std::cout << std::endl;
}

DataPrecisionType
SimosUtils::getWeightsSum(const std::vector<RankWeight> &weights) {
  DataPrecisionType total = 0;

  for (const auto &w : weights) {
    total += w.weight;
  }

  return total;
}

DataPrecisionType SimosUtils::roundUpToXDecimals(DataPrecisionType value,
                                                 unsigned int decimals) {

  if (decimals == 0) {
    return ceil(value);
  }
  return ceil(value * pow(10, decimals)) / pow(10, decimals);
}
