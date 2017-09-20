#ifndef xTRTFrame_Helpers_h
#define xTRTFrame_Helpers_h

namespace xTRT {
  enum StrawRegion {
    BRL    = 0,
    BRLECA = 1,
    ECA    = 2,
    ECAECB = 3,
    ECB    = 4,
    NOTTRT = 5
  };
}

namespace xTRT {
  StrawRegion getStrawRegion(const float eta);
}

#endif
