#include <xTRTFrame/Helpers.h>
#include <xAODCore/AuxContainerBase.h>
#include <cmath>

xTRT::StrawRegion xTRT::getStrawRegion(const float eta) {
  float absEta = std::fabs(eta);
  if      ( absEta < 0.625 )  return xTRT::StrawRegion::BRL;
  else if ( absEta < 1.070 )  return xTRT::StrawRegion::BRLECA;
  else if ( absEta < 1.304 )  return xTRT::StrawRegion::ECA;
  else if ( absEta < 1.752 )  return xTRT::StrawRegion::ECAECB;
  else if ( absEta < 2.000 )  return xTRT::StrawRegion::ECB;
  else                        return xTRT::StrawRegion::NOTTRT;
}

xTRT::SideRegion xTRT::getSideRegion(const int bec) {
  switch ( bec ) {
  case -2:
    return xTRT::SideRegion::SIDE_C;
  case -1:
  case  1:
    return xTRT::SideRegion::BARREL;
  case  2:
    return xTRT::SideRegion::SIDE_A;
  default:
    std::string msg = "bad value for bec: " + std::to_string(bec);
    XTRT_FATAL(msg);
    break;
  }
  return xTRT::SideRegion::NONTRT;
}

xTRT::SideRegion xTRT::getSideRegion(const xAOD::TrackMeasurementValidation* dc) {
  static const SG::AuxElement::ConstAccessor<int> acc{"bec"};
  if ( acc.isAvailable(*dc) ) {
    return xTRT::getSideRegion(acc(*dc));
  }
  else {
    XTRT_FATAL("Drift circle does not contain 'bec'!");
    return xTRT::SideRegion::NONTRT;
  }
}

int xTRT::absoluteBarrelSL(const int sl, const int layer) {
  int abs_SL = sl;
  if ( layer > 0 )
    abs_SL += 19;
  if ( layer > 1 )
    abs_SL += 24;
  if ( layer > 2 || abs_SL > 72 )
    XTRT_FATAL("absoluteBarrelSL: Layer information is bad!");
  return abs_SL;
}

int xTRT::absoluteEndCapSL(const int sl, const int wheel) {
  int abs_SL = sl;
  if ( wheel < 6 )
    abs_SL += 16 * wheel;
  else
    abs_SL += 96 + (wheel - 6) * 8;
  if ( abs_SL > 159 )
    XTRT_FATAL("absoluteEndCapSL: Layer information is bad!")
  return abs_SL;
}
