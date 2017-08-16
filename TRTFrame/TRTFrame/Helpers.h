#ifndef TRTFrame_Helpers_h
#define TRTFrame_Helpers_h

namespace TRTF {
  enum StrawRegion {
    BRL    = 0,
    BRLECA = 1,
    ECA    = 2,
    ECAECB = 3,
    ECB    = 4,
    NOTTRT = 5
  };
}

namespace TRTF {
  StrawRegion getStrawRegion(const float eta);
}

#endif
