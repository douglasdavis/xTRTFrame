#ifndef TRTLite_Hit_h
#define TRTLite_Hit_h

namespace TRTLite {

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
    float localTheta;
    float localPhi;
    float HitZ;
    float HitR;
    float rTrkWire;
    float L;
  };

}

#endif
