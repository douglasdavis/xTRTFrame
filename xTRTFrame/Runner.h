/** @file  Runner.h
 *  @brief xTRT::Runner function for running an algorithm.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef xTRTFrame_Runner_h
#define xTRTFrame_Runner_h

namespace xTRT {
  class Algorithm;
}

namespace xTRT {
  int Runner(int argc, char **argv, xTRT::Algorithm *alg);
}

#endif
