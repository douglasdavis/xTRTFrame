#ifndef TRTLite_ParticleId_h
#define TRTLite_ParticleId_h

// C++
#include <array>

namespace TRTLite {
  enum Hyp {
    Electron,
    Pion,
    Muon
  };
}

namespace TRTLite {

  class ParticleIdSvc {

  private:

    std::array<float,5> m_bbParams;
    std::array<float,4> m_resParamsEle;
    std::array<float,4> m_resParamsMIP;

  public:

    ParticleIdSvc();
    virtual ~ParticleIdSvc();

    float ToT_getTest(float dEdx, float trackP, TRTLite::Hyp hyp, TRTLite::Hyp ahyp, int nhits) const;
    float ToT_getProbability(float dEdx, float trackP, TRTLite::Hyp hyp, int nhits) const;
    float ToT_predictdEdx(float trackP, TRTLite::Hyp hyp) const;

  };

}

#endif
