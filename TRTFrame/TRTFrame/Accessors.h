#ifndef TRTFrame_Accessors_h
#define TRTFrame_Accessors_h

// ATLAS
#include <AthContainers/AuxElement.h>
#include <xAODTracking/TrackStateValidationContainer.h>
#include <xAODTruth/TruthParticleContainer.h>

using uint = unsigned int;

namespace xTRT {
  namespace Acc {

    // Track
    static const SG::AuxElement::ConstAccessor<float> eProbabilityToT          {"eProbabilityToT"};
    static const SG::AuxElement::ConstAccessor<float> eProbabilityHT           {"eProbabilityHT"};
    static const SG::AuxElement::ConstAccessor<float> TRTTrackOccupancy        {"TRTTrackOccupancy"};
    static const SG::AuxElement::ConstAccessor<float> ToT_dEdx_noHT_divByL     {"ToT_dEdx_noHT_divByL"};
    static const SG::AuxElement::ConstAccessor<float> ToT_usedHits_noHT_divByL {"ToT_usedHits_noHT_divByL"};

    static const SG::AuxElement::ConstAccessor<
      ElementLink<xAOD::TruthParticleContainer>
      > truthParticleLink{"truthParticleLink"};

    static const SG::AuxElement::ConstAccessor<
      std::vector<ElementLink<xAOD::TrackStateValidationContainer>>
      > msosLink{"msosLink"};

    // Drift Circle
    static const SG::AuxElement::ConstAccessor<uint>  bitPattern  {"bitPattern"};
    static const SG::AuxElement::ConstAccessor<char>  gasType     {"gasType"};
    static const SG::AuxElement::ConstAccessor<int>   bec         {"bec"};
    static const SG::AuxElement::ConstAccessor<int>   layer       {"layer"};
    static const SG::AuxElement::ConstAccessor<int>   strawlayer  {"strawlayer"};
    static const SG::AuxElement::ConstAccessor<int>   strawnumber {"strawnumber"};
    static const SG::AuxElement::ConstAccessor<float> drifttime   {"drifttime"};
    static const SG::AuxElement::ConstAccessor<float> tot         {"tot"};
    static const SG::AuxElement::ConstAccessor<float> T0          {"T0"};

    // MSOS
    static const SG::AuxElement::ConstAccessor<int>   type       {"type"};
    static const SG::AuxElement::ConstAccessor<float> localX     {"localX"};
    static const SG::AuxElement::ConstAccessor<float> localY     {"localY"};
    static const SG::AuxElement::ConstAccessor<float> localTheta {"localTheta"};
    static const SG::AuxElement::ConstAccessor<float> localPhi   {"localPhi"};
    static const SG::AuxElement::ConstAccessor<float> HitZ       {"HitZ"};
    static const SG::AuxElement::ConstAccessor<float> HitR       {"HitR"};
    static const SG::AuxElement::ConstAccessor<float> rTrkWire   {"rTrkWire"};

  }
}

#endif
