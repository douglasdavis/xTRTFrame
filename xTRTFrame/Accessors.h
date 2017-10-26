/** @file  Accessors.h
 *  @brief xTRT supplied Accessors header
 *  @namespace xTRT::Acc
 *  @brief namespace for const xAOD AuxElement accessors
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef xTRTFrame_Accessors_h
#define xTRTFrame_Accessors_h

// ATLAS
#include <AthContainers/AuxElement.h>
#include <xAODTracking/TrackStateValidationContainer.h>
#include <xAODTruth/TruthParticleContainer.h>

using uint = unsigned int;

namespace xTRT {
  namespace Acc {

    // Track
    const SG::AuxElement::ConstAccessor<float> eProbabilityToT          {"eProbabilityToT"};
    const SG::AuxElement::ConstAccessor<float> eProbabilityHT           {"eProbabilityHT"};
    const SG::AuxElement::ConstAccessor<float> TRTTrackOccupancy        {"TRTTrackOccupancy"};
    const SG::AuxElement::ConstAccessor<float> ToT_dEdx_noHT_divByL     {"ToT_dEdx_noHT_divByL"};
    const SG::AuxElement::ConstAccessor<float> ToT_usedHits_noHT_divByL {"ToT_usedHits_noHT_divByL"};


    const SG::AuxElement::ConstAccessor<unsigned char> numberOfTRTHits     {"numberOfTRTHits"};
    const SG::AuxElement::ConstAccessor<unsigned char> numberOfTRTOutliers {"numberOfTRTOutliers"};
    const SG::AuxElement::ConstAccessor<unsigned char> numberOfInnermostPixelLayerHits
    {"numberOfInnermostPixelLayerHits"};
    const SG::AuxElement::ConstAccessor<unsigned char> numberOfNextToInnermostPixelLayerHits
    {"numberOfNextToInnermostPixelLayerHits"};
    const SG::AuxElement::ConstAccessor<unsigned char> numberOfPixelHits {"numberOfPixelHits"};
    const SG::AuxElement::ConstAccessor<unsigned char> numberOfSCTHits   {"numberOfSCTHits"};

    const SG::AuxElement::ConstAccessor<float> numberDoF  {"numberDoF"};
    const SG::AuxElement::ConstAccessor<float> chiSquared {"chiSquared"};

    const SG::AuxElement::ConstAccessor<
      ElementLink<xAOD::TruthParticleContainer>
      > truthParticleLink{"truthParticleLink"};

    const SG::AuxElement::ConstAccessor<
      std::vector<ElementLink<xAOD::TrackStateValidationContainer>>
      > msosLink{"msosLink"};

    // Drift Circle
    const SG::AuxElement::ConstAccessor<uint>  bitPattern  {"bitPattern"};
    const SG::AuxElement::ConstAccessor<char>  gasType     {"gasType"};
    const SG::AuxElement::ConstAccessor<int>   bec         {"bec"};
    const SG::AuxElement::ConstAccessor<int>   layer       {"layer"};
    const SG::AuxElement::ConstAccessor<int>   strawlayer  {"strawlayer"};
    const SG::AuxElement::ConstAccessor<int>   strawnumber {"strawnumber"};
    const SG::AuxElement::ConstAccessor<float> drifttime   {"drifttime"};
    const SG::AuxElement::ConstAccessor<float> tot         {"tot"};
    const SG::AuxElement::ConstAccessor<float> T0          {"T0"};

    // MSOS
    const SG::AuxElement::ConstAccessor<int>   type       {"type"};
    const SG::AuxElement::ConstAccessor<float> localX     {"localX"};
    const SG::AuxElement::ConstAccessor<float> localY     {"localY"};
    const SG::AuxElement::ConstAccessor<float> localTheta {"localTheta"};
    const SG::AuxElement::ConstAccessor<float> localPhi   {"localPhi"};
    const SG::AuxElement::ConstAccessor<float> HitZ       {"HitZ"};
    const SG::AuxElement::ConstAccessor<float> HitR       {"HitR"};
    const SG::AuxElement::ConstAccessor<float> rTrkWire   {"rTrkWire"};

  }
}

#endif
