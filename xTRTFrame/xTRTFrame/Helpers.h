/** @file  Helpers.h
 *  @brief xTRTFrame helper functions
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef xTRTFrame_Helpers_h
#define xTRTFrame_Helpers_h

#include <xTRTFrame/Utils.h>
#include <xAODTracking/TrackMeasurementValidationContainer.h>

namespace xTRT {

  /*!
    \enum StrawRegion

    Enum for different straw regions of the TRT
    BRL: barrel only
    BRLECA: barrel & endcap type A
    ECA: just endcap type A
    ECAECB: endcap type A & endcap type B
    ECB: just endcap type B
  */
  enum StrawRegion {
    BRL    = 0,
    BRLECA = 1,
    ECA    = 2,
    ECAECB = 3,
    ECB    = 4,
    NOTTRT = 5
  };

  /*!
    \enum SideRegion

    Enum for different sides of the TRT
    Self explanatory values.
  */
  enum SideRegion {
    BARREL = 0,
    SIDE_A = 1,
    SIDE_C = 2,
    NONTRT = 3
  };

  /// get the straw region based on the eta value
  StrawRegion getStrawRegion(const float eta);
  /// get the side region based on the bec variable
  SideRegion  getSideRegion(const int bec);
  /// get the side region from the drift circle
  SideRegion getSideRegion(const xAOD::TrackMeasurementValidation* dc);
  /// get the absolute straw layer in the barrel
  int absoluteBarrelSL(const int sl, const int layer);
  /// get the absolute straw layer in the end cap
  int absoluteEndCapSL(const int sl, const int wheel);

}

#endif
