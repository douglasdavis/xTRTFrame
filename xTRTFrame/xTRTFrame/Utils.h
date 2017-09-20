#ifndef xTRTFrame_Utils_h
#define xTRTFrame_Utils_h

#include <cstdlib>
#include <iostream>

#define XTRT_FATAL(MESSAGE)                                     \
  { std::cerr << "xTRTFrame FAILURE\t" << MESSAGE << std::endl;  \
    std::exit(EXIT_FAILURE); }

#endif
