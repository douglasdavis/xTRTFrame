#include <xTRTFrame/Helpers.h>

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
