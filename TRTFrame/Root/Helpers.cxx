#include <TRTFrame/Helpers.h>

#include <cmath>

TRTF::StrawRegion TRTF::getStrawRegion(const float eta) {
  float absEta = std::fabs(eta);
  if      ( absEta < 0.625 )  return TRTF::StrawRegion::BRL;
  else if ( absEta < 1.070 )  return TRTF::StrawRegion::BRLECA;
  else if ( absEta < 1.304 )  return TRTF::StrawRegion::ECA;
  else if ( absEta < 1.752 )  return TRTF::StrawRegion::ECAECB;
  else if ( absEta < 2.000 )  return TRTF::StrawRegion::ECB;
  else                        return TRTF::StrawRegion::NOTTRT;
}
