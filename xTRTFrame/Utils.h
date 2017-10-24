/** @file  Utils.h
 *  @brief xTRTFrame utilities
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef xTRTFrame_Utils_h
#define xTRTFrame_Utils_h

#include <cstdlib>
#include <iostream>

#include <sstream>
#include <vector>

#define GeV   1000.0
#define toGeV 0.0010

namespace xTRT {
  auto stringSplit(const std::string &s, char delim, std::vector<std::string> &elems);
  auto stringSplit(const std::string &s, char delim);
}

inline auto xTRT::stringSplit(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.emplace_back(item);
  }
  return elems;
}

inline auto xTRT::stringSplit(const std::string &s, char delim) {
  std::vector<std::string> elems;
  xTRT::stringSplit(s, delim, elems);
  return elems;
}

/*!
  \def XTRT_FATAL
  Print MESSAGE to std::cerr and exit with EXIT_FAILURE
*/
#define XTRT_FATAL(MESSAGE)                                       \
  { std::cerr << "xTRTFrame FAILURE\t" << MESSAGE << std::endl;   \
    std::exit(EXIT_FAILURE); }

/*! XTRT_WARNING
  Print MESSAGE to std::cout
*/
#define XTRT_WARNING(MESSAGE)                                           \
  { std::cout << "xTRTFrame WARNING\t" << MESSAGE << std::endl; }

/*! SETUP_TREE
  initializes a TTree to be saved to xTRTFrame algorithm output
*/
#define SETUP_OUTPUT_TREE(TREE,NAME)                            \
  { TREE = new TTree(NAME,NAME);                                \
    TREE->SetDirectory(wk()->getOutputFile(m_outputName)); }

#endif
