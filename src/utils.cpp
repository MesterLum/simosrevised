#include "../include/utils.hpp"
#include "../include/simos_revised.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>

DataPrecisionType SimosUtils::roundToXDecimals(DataPrecisionType value,
                                               unsigned int decimals) {
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
