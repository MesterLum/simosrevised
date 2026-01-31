#include "../include/utils.hpp"
#include "../include/simos_revised.hpp"
#include <cmath>
#include <iostream>

DataPrecisionType SimosUtils::roundToXDecimals(DataPrecisionType value,
                                               unsigned int decimals) {
  return round(value * pow(10, decimals)) / pow(10, decimals);
}

DataPrecisionType SimosUtils::truncateToXDecimals(DataPrecisionType value,
                                                  unsigned int decimals) {
  return int(value * pow(10, decimals)) / pow(10, decimals);
}

void SimosUtils::printWeights(WeightsMapType weights) {
  for (const auto &w : weights) {
    std::cout << "[" << w.first << "] = " << w.second << std::endl;
  }
}
