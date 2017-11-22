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
    \enum IDTSCut
    One can look at the definitions here
    https://twiki.cern.ch/twiki/bin/view/AtlasProtected/InDetTrackSelectionTool
  */
  enum IDTSCut {
    TightPrimary  = 0, ///< self explanatory
    LoosePrimary  = 1, ///< self explanatory
    LooseElectron = 2, ///< self explanatory
    LooseMuon     = 3  ///< self explanatory
  };

  /*!
    \enum StrawRegion
    Enum for different straw regions of the TRT
  */
  enum StrawRegion {
    BRL    = 0, ///< Barrel
    BRLECA = 1, ///< Barrel/End Cap type A combination
    ECA    = 2, ///< End Cap Type A
    ECAECB = 3, ///< End Cap Type A/End Cap Type B combination
    ECB    = 4, ///< End Cap Type B
    NOTTRT = 5  ///< outside of the TRT
  };

  /*!
    \enum SideRegion
    Enum for different sides of the TRT
  */
  enum SideRegion {
    BARREL = 0, ///< Barrel ("bec" = +/- 1)
    SIDE_A = 1, ///< Side A ("bec" = 2)
    SIDE_C = 2, ///< Side C ("bec" = -2)
    NONTRT = 3  ///< not in TRT
  };

  /** \addtogroup GenHelpers Generic Helper Functions
   *  \brief Some misc. functions to make life easier
   *  @{
   */

  /// get the straw region based on the eta value.
  /**
   * @param eta the pseudorapidity of the track.
   */
  StrawRegion getStrawRegion(const float eta);

  /// get the side region based on the bec variable.
  /**
   * @param bec the barrel/endcap value ("bec" is a decoration on the
   * drift circle object).
   */
  SideRegion  getSideRegion(const int bec);

  /// get the side region from the drift circle.
  /**
   * @param driftCircle the drift circle object retrieved from the
   * surface measurement (msos).
   */
  SideRegion getSideRegion(const xAOD::TrackMeasurementValidation* driftCircle);

  /// get the absolute straw layer in the barrel.
  /**
   * @param sl the straw layer ("strawlayer" decoration on the drift
   * circle).
   * @param layer the layer ("layer" decoration on the drift circle).
   */
  int absoluteBarrelSL(const int sl, const int layer);

  /// get the absolute straw layer in the end cap.
  /**
   * @param sl the straw layer ("strawlayer" decoration on the drift
   * circle).
   * @param wheel the wheel layer ("layer" decoration on the drift
   * circle).
   */
  int absoluteEndCapSL(const int sl, const int wheel);

  /** @} */ // end of Helpers

}

#endif
