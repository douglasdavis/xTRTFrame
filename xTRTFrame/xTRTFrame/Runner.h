/** @file  Runer.h
 *  @brief xTRT::Runner function for running an algorithm.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef xTRTFrame_Runner_h
#define xTRTFrame_Runner_h

namespace xTRT {
  class Algo;
}

namespace xTRT {
  int Runner(int argc, char **argv, xTRT::Algo *alg);
}

#endif
