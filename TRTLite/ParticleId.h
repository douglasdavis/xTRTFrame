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
  enum StrawType {
    BRL = 0,
    ECA = 1,
    ECB = 2,
    NON = 3
  };
}

namespace TRTLite {

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

    float ToT_getTest(float dEdx, float trackP, TRTLite::Hyp hyp, TRTLite::Hyp ahyp,
                      int nhits, TRTLite::StrawType stype) const;
    float ToT_getProbability(float dEdx, float trackP, TRTLite::Hyp hyp,
                             int nhits, TRTLite::StrawType stype) const;
    float ToT_predictdEdx(float trackP, TRTLite::Hyp hyp, TRTLite::StrawType stype) const;

  };

}

#endif
