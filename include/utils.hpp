#ifndef SIMOS_UTILS
#define SIMOS_UTILS
#include "./simos_revised.hpp"

class SimosUtils {
public:
  static DataPrecisionType roundToXDecimals(DataPrecisionType value,
                                            unsigned int decimals);
  static DataPrecisionType truncateToXDecimals(DataPrecisionType value,
                                               unsigned int decimals);
  static void printUnorderedRanksData(ListWeightType ratio);
  static void printWeights(WeightsMapType weights, int decimals = 2);
  static void printRankGroups(const RanksMapType &ranks);
};
#endif
