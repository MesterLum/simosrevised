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

class SimosRevised {
private:
  bool debug = true;

protected:
  RanksMapType ranks = {};
  WhiteCardsMapType whiteCards = {};
  DataPrecisionType zRatio;
  unsigned int decimals;
  unsigned int decimalsToRetain = 6;

public:
  SimosRevised(const RanksMapType &ranks, const WhiteCardsMapType &whiteCards,
               DataPrecisionType zRatio, unsigned int decimals);

  void generateWeights();
  WeightsMapType getNonNormalizedWeights(const WhiteCardsMapType &whiteCards,
                                         DataPrecisionType ratio);
  WeightsMapType getNormalizedWeights();
  unsigned int getCriteriaCount();
  WhiteCardsMapType getWhiteCardsWithNonZeroValues();
  unsigned int getWhiteCardsCount(const WhiteCardsMapType &whiteCards);
  DataPrecisionType getRatio(unsigned int whiteCardsCount);
};
#endif
