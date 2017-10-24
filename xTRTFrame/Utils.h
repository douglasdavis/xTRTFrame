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

#include <xTRTFrame/spdlog/spdlog.h>

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

/*! SETUP_TREE
  initializes a TTree to be saved to xTRTFrame algorithm output
*/
#define SETUP_OUTPUT_TREE(TREE,NAME)                            \
  { TREE = new TTree(NAME,NAME);                                \
    TREE->SetDirectory(wk()->getOutputFile(m_outputName)); }

/*! XTRT_WARNING
  Print warning message ussing spdlog
*/
#define XTRT_WARNING(MSG)                               \
  { if ( spdlog::get("xTRTFrame") == nullptr ) {        \
      spdlog::stdout_color_mt("xTRTFrame");             \
    }                                                   \
    spdlog::get("xTRTFrame")->warn(MSG); }

/*!
  \def XTRT_FATAL
  Exit and print message
*/
#define XTRT_FATAL(MSG)                                                 \
  { if ( spdlog::get("xTRTFrame") == nullptr ) {                        \
      spdlog::stdout_color_mt("xTRTFrame");                             \
    }                                                                   \
    spdlog::get("xTRTFrame")->critical("Died in {}, more: {}",          \
                                       __PRETTY_FUNCTION__, MSG);       \
    std::exit(EXIT_FAILURE); }

#endif
