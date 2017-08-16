#ifndef TRTFrame_ParticleId_h
#define TRTFrame_ParticleId_h

// C++
#include <array>

// TRTFrame
#include <TRTFrame/Helpers.h>

namespace TRTF {
  enum Hyp {
    Electron,
    Pion,
    Muon
  };
}

namespace TRTF {

  class ParticleIdSvc {

  private:

    std::array<float,5> m_bbParams;
    std::array<float,4> m_resParamsEle;
    std::array<float,4> m_resParamsMIP;

    std::array<float,5> m_bbParams_brl;
    std::array<float,5> m_bbParams_eca;
    std::array<float,5> m_bbParams_ecb;

  public:

    ParticleIdSvc();
    virtual ~ParticleIdSvc();

    float combinedProb(float pHT, float pToT) const;

    float ToT_getTest(float dEdx, float trackP, TRTF::Hyp hyp, TRTF::Hyp ahyp,
                      int nhits, TRTF::StrawRegion stype) const;
    float ToT_getProbability(float dEdx, float trackP, TRTF::Hyp hyp,
                             int nhits, TRTF::StrawRegion stype) const;
    float ToT_predictdEdx(float trackP, TRTF::Hyp hyp, TRTF::StrawRegion stype) const;

  };

}

inline float TRTF::ParticleIdSvc::combinedProb(float pHT, float pToT) const {
  return pHT*pToT/(pHT*pToT+(1-pHT)*(1-pToT));
}

#endif
