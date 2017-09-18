#ifndef TRTFrame_Utils_h
#define TRTFrame_Utils_h

#include <cstdlib>
#include <iostream>

#define XTRT_FATAL(MESSAGE)                                     \
  { std::cerr << "TRTFrame FAILURE\t" << MESSAGE << std::endl;  \
    std::exit(EXIT_FAILURE); }

#endif
