/** @file  HitSummary.h
 *  @brief xTRT::HitSummary class header
 *  @class xTRT::HitSummary
 *  @brief Class for storing hit information
 *
 *  This is a utility class for giving straightforward access to hit
 *  information instead of using the more complex xAOD Track
 *  Measurement class.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef xTRTFrame_Hit_h
#define xTRTFrame_Hit_h

namespace xTRT {

  /** \addtogroup HitHelpers Hit Helpers
   *  \brief Classes/functions to help dealing with hits
   *  @{
   */

  struct HitSummary {

    int   HTMB;
    int   gasType;
    int   bec;
    int   layer;
    int   strawlayer;
    int   strawnumber;
    float drifttime;
    float tot;
    float T0;

    int   type;
    float localX;
    float localY;
    float localTheta;
    float localPhi;
    float HitZ;
    float HitR;
    float rTrkWire;
    float L;

  };

  /** @}*/
}

#endif
