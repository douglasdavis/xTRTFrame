/** @file  Utils.h
 *  @brief xTRTFrame utilities
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef xTRTFrame_Utils_h
#define xTRTFrame_Utils_h

#include <cstdlib>
#include <iostream>

/*!
  \def XTRT_FATAL
  Print MESSAGE to std::cerr and exit with EXIT_FAILURE
*/
#define XTRT_FATAL(MESSAGE)                                      \
  { std::cerr << "xTRTFrame FAILURE\t" << MESSAGE << std::endl;  \
    std::exit(EXIT_FAILURE); }

#endif
