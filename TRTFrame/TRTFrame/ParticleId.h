#ifndef TRTFrame_ParticleId_h
#define TRTFrame_ParticleId_h

// C++
#include <array>

// TRTFrame
#include <TRTFrame/Helpers.h>

namespace xTRT {
  enum Hyp {
    Electron,
    Pion,
    Muon
  };
}

namespace xTRT {

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

    float ToT_getTest(float dEdx, float trackP, xTRT::Hyp hyp, xTRT::Hyp ahyp,
                      int nhits, xTRT::StrawRegion stype) const;
    float ToT_getProbability(float dEdx, float trackP, xTRT::Hyp hyp,
                             int nhits, xTRT::StrawRegion stype) const;
    float ToT_predictdEdx(float trackP, xTRT::Hyp hyp, xTRT::StrawRegion stype) const;

  };

}

inline float xTRT::ParticleIdSvc::combinedProb(float pHT, float pToT) const {
  return pHT*pToT/(pHT*pToT+(1-pHT)*(1-pToT));
}

#endif
