// TRTLite
#include <TRTLite/ParticleId.h>

// C++
#include <cmath>

TRTLite::ParticleIdSvc::ParticleIdSvc() {
  m_bbParams = {
    -1.56411e-01,
    -7.78249e+00,
    -9.55779e-01,
    1.16393e+00,
    -2.03921e-02
  };
  /*
  m_bbParams = {
    -0.192682,
    -6.956870,
    -0.974757,
    1.27633,
    -0.014986,
  };
  */
  m_resParamsEle = {
    0.1606,
    -0.008239,
    0.0002552,
    -0.000002989
  };
  m_resParamsMIP = {
    0.173916,
    -0.009003,
    0.00028175,
    -0.000003394
  };
}

TRTLite::ParticleIdSvc::~ParticleIdSvc() {}

float TRTLite::ParticleIdSvc::ToT_getTest(float dEdx, float trackP,
                                          TRTLite::Hyp hyp, TRTLite::Hyp ahyp, int nhits) const {
  double Pone = ToT_getProbability(dEdx, trackP, hyp,  nhits);
  double Ptwo = ToT_getProbability(dEdx, trackP, ahyp, nhits);
  if ( (Pone+Ptwo) != 0.0 ) {
    return Pone/(Pone+Ptwo);
  }
  else {
    return 0.5;
  }
}

float TRTLite::ParticleIdSvc::ToT_getProbability(float dEdx, float trackP, TRTLite::Hyp hyp, int nhits) const {
  float dEdx_pred = ToT_predictdEdx(trackP, hyp);
  if ( dEdx_pred == 0.0 ) return 0.0;

  float res = 0.0;
  if ( hyp == TRTLite::Hyp::Electron ) {
    float factor  = 1.0;
    //float correct = 1+factor*(0.045*std::log10(trackP)+0.885-1);
    float correct = 1+factor*(0.025*std::log10(trackP)+0.885-1);
    dEdx_pred = dEdx_pred/correct;

    res = m_resParamsEle.at(0) + m_resParamsEle.at(1)*(nhits+0.5) +
      m_resParamsEle.at(2)*std::pow(nhits+0.5,2) + m_resParamsEle.at(3)*std::pow(nhits+0.5,3);
  }
  else {
    res = m_resParamsMIP.at(0) + m_resParamsMIP.at(1)*(nhits+0.5) +
      m_resParamsMIP.at(2)*std::pow(nhits+0.5,2) + m_resParamsMIP.at(3)*std::pow(nhits+0.5,3);
  }
  float prob = std::exp(-0.5*std::pow((dEdx-dEdx_pred)/(res*dEdx_pred),2));
  return prob;
}

float TRTLite::ParticleIdSvc::ToT_predictdEdx(float trackP, TRTLite::Hyp hyp) const {
  float mass = 0.510998;
  if ( hyp == TRTLite::Hyp::Pion ) {
    mass = 139.98;
  }
  float bg = trackP/mass;
  if ( trackP < 100.0 ) return 0.0;

  return m_bbParams.at(0) /
    std::pow(std::sqrt((bg*bg)/(1.+(bg*bg))), m_bbParams.at(3)) * (m_bbParams.at(1) -
     std::pow(std::sqrt( (bg*bg)/(1.+(bg*bg))), m_bbParams.at(3) ) -
     std::log(m_bbParams.at(2)+1./( std::pow(bg,m_bbParams.at(4)))));
}
