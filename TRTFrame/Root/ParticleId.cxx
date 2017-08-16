// TRTFrame
#include <TRTFrame/ParticleId.h>

// C++
#include <cmath>
#include <iostream>

TRTF::ParticleIdSvc::ParticleIdSvc() {

  //m_bbParams = { -0.192682, -6.956870, -0.974757, 1.27633, -0.014986 }; // DEFAULT

  m_bbParams     = { -1.54912e-01, -7.85175e+00, -9.56614e-01, 1.16500e+00, -2.04535e-02 };
  m_bbParams_brl = { -3.78432e-01, -5.12826e+00, -9.76325e-01, 1.50707e+00, -6.70900e-03 };
  m_bbParams_eca = { -1.98921e-01, -7.37192e+00, -9.93165e-01, 2.12850e+00, -1.27547e-02 };
  m_bbParams_ecb = { -1.31162e-01, -9.41779e+00, -9.86885e-01, 1.44736e+00, -1.83808e-02 };

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

TRTF::ParticleIdSvc::~ParticleIdSvc() {}

float TRTF::ParticleIdSvc::ToT_getTest(float dEdx, float trackP,
                                          TRTF::Hyp hyp, TRTF::Hyp ahyp, int nhits,
                                          TRTF::StrawRegion stype) const {
  double Pone = ToT_getProbability(dEdx, trackP, hyp,  nhits, stype);
  double Ptwo = ToT_getProbability(dEdx, trackP, ahyp, nhits, stype);
  if ( (Pone+Ptwo) != 0.0 ) {
    return Pone/(Pone+Ptwo);
  }
  else {
    return 0.5;
  }
}

float TRTF::ParticleIdSvc::ToT_getProbability(float dEdx, float trackP,
                                                 TRTF::Hyp hyp, int nhits, TRTF::StrawRegion stype) const {
  float dEdx_pred = ToT_predictdEdx(trackP, hyp, stype);
  if ( dEdx_pred == 0.0 ) return 0.0;

  float res = 0.0;
  if ( hyp == TRTF::Hyp::Electron ) {
    float factor  = 1.0;
    float correct = 1+factor*(0.045*std::log10(trackP)+0.885-1);
    //float correct = 1+factor*(0.025*std::log10(trackP)+0.885-1);
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

float TRTF::ParticleIdSvc::ToT_predictdEdx(float trackP, TRTF::Hyp hyp, TRTF::StrawRegion stype) const {
  float mass = 0.510998;
  if ( hyp == TRTF::Hyp::Pion ) {
    mass = 139.98;
  }
  float bg = trackP/mass;
  if ( trackP < 100.0 ) return 0.0;

  const std::array<float,5>* paramArr;
  if      ( stype == TRTF::StrawRegion::BRL ) { paramArr = &m_bbParams_brl; }
  else if ( stype == TRTF::StrawRegion::ECA ) { paramArr = &m_bbParams_eca; }
  else if ( stype == TRTF::StrawRegion::ECB ) { paramArr = &m_bbParams_ecb; }
  else                                        { paramArr = &m_bbParams;     }

  return paramArr->at(0) /
    std::pow(std::sqrt((bg*bg)/(1.+(bg*bg))), paramArr->at(3)) * (paramArr->at(1) -
     std::pow(std::sqrt( (bg*bg)/(1.+(bg*bg))), paramArr->at(3) ) -
     std::log(paramArr->at(2)+1./( std::pow(bg,paramArr->at(4)))));
}
