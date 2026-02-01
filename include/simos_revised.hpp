#ifndef SIMOS_REVISED
#define SIMOS_REVISED
#include <map>
#include <vector>

typedef double DataPrecisionType;
typedef std::string RankType;
typedef std::vector<RankType> RankGroupType;
typedef std::map<unsigned int, RankGroupType> RanksMapType;
typedef std::map<unsigned int, unsigned int> WhiteCardsMapType;
typedef std::map<unsigned int, DataPrecisionType> WeightsMapType;
typedef std::vector<std::pair<unsigned int, DataPrecisionType>> ListWeightType;

class SimosRevised {
private:
  bool debug = true;

protected:
  RanksMapType ranks = {};
  WhiteCardsMapType whiteCards = {};
  DataPrecisionType zRatio;
  int decimals;
  int decimalsToRetain = 6;
  WeightsMapType getNonNormalizedWeights(const WhiteCardsMapType &whiteCards,
                                         DataPrecisionType ratio);
  WeightsMapType
  getNormalizedWeights(const WeightsMapType &nonNormalizedWeights,
                       DataPrecisionType sumNonNormalizedWeights);
  unsigned int getCriteriaCount();
  WhiteCardsMapType getWhiteCardsWithNonZeroValues();
  unsigned int getWhiteCardsCount(const WhiteCardsMapType &whiteCards);
  DataPrecisionType getRatio(unsigned int whiteCardsCount);
  DataPrecisionType getWeightsSum(const WeightsMapType &weights);
  WeightsMapType getNormalizedWeightsTruncated(const WeightsMapType &weights);
  WeightsMapType
  getNearestUpWeights(const WeightsMapType &normalizedWeights,
                      const WeightsMapType &normalizedWeightsTruncated);
  WeightsMapType
  getNearestDownWeights(const WeightsMapType &normalizedWeights,
                        const WeightsMapType &normalizedWeightsTruncated);
  RanksMapType getGreaterElements(const WeightsMapType &d1,
                                  const WeightsMapType &d2);
  unsigned int getSumGreaterElements(const RanksMapType &mRanks);
  ListWeightType sortWeights(const WeightsMapType &weights, bool asc = true);
  RanksMapType getF1Ranks(const ListWeightType &weightsList,
                          const RanksMapType &mList, unsigned int totalElements,
                          unsigned int mCount);
  RanksMapType getF2Ranks(const ListWeightType &weightsList,
                          const RanksMapType &mList,
                          unsigned int totalElements);

public:
  SimosRevised(const RanksMapType &ranks, const WhiteCardsMapType &whiteCards,
               DataPrecisionType zRatio, unsigned int decimals);

  void generateWeights();
};
#endif
