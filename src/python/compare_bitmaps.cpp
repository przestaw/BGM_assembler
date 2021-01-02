//
// Created by przestaw on 10.11.2020.
//

#include <python/compare_bitmaps.h>
#include <utility>

using namespace bgm;
using namespace bgm::py;

CompareBitmaps::CompareBitmaps(BinaryMap l, BinaryMap r)
    : l_(std::move(l)), r_(std::move(r)) {}

vector<CompareBitmaps::MarkerType> CompareBitmaps::alignMapToRef() {
  const auto align  = bgm::AlignAlgorithm::pyAlignFromBegins(l_.getMap(), r_.getMap());
  const size_t lLen = l_.getMap().size();
  const size_t rLen = r_.getMap().size();

  const size_t totalLen = (lLen + rLen) - align.getOverlap();

  const size_t startL = align.getShift() < 0 ? -align.getShift() : 0;
  const size_t startR = align.getShift() > 0 ? +align.getShift() : 0;

  auto data = vector<MarkerType>(3 * totalLen);

  static const auto notEqualMarker = [](MarkerType lOp, MarkerType rOp) {
    return lOp != rOp ? MarkerType::Marker : MarkerType::CompareOrReferenceMap;
  };

  for (size_t i = 0; i < totalLen; ++i) {
    const auto lVal = getMapValAtPos(i, startL, l_);
    const auto rVal = getMapValAtPos(i, startR, r_);

    data[i]            = lVal;
    data[i + totalLen] = rVal;

    const auto xorVal = (lVal != MarkerType::Empty && rVal != MarkerType::Empty)
                            ? notEqualMarker(lVal, rVal)
                            : MarkerType::Empty;

    data[i + 2 * totalLen] = xorVal;
  }

  return data;
}
