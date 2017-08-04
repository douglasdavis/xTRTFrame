#ifndef TRTFrame_Hit_h
#define TRTFrame_Hit_h

namespace TRTF {

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

}

#endif
