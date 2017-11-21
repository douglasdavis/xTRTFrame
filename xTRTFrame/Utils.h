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
  /// enum for defining different InDetTrackSelectionTool Cuts
  /**
   * One can look at the definitions here
   * https://twiki.cern.ch/twiki/bin/view/AtlasProtected/InDetTrackSelectionTool
   */
  enum IDTSCut {
    TightPrimary  = 0, ///< self explanatory
    LoosePrimary  = 1, ///< self explanatory
    LooseElectron = 2, ///< self explanatory
    LooseMuon     = 3  ///< self explanatory
  };
}

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

/*! \def SETUP_OUTPUT_TREE
  initializes a TTree to be saved to xTRTFrame algorithm output
*/
#define SETUP_OUTPUT_TREE(TREE,NAME)                                    \
  { TREE = new TTree(NAME,NAME);                                        \
    TREE->SetDirectory((TDirectory*)(wk()->getOutputFile(m_outputName))); }

/*! \def XTRT_WARNING
  Print warning message for non Algorithm class warnings
*/
#define XTRT_WARNING(MSG)						\
  { std::cout << "XTRT_WARNING: " << MSG << '\n' << std::flush; }

/*!
  \def XTRT_FATAL
  Exit and print message including function
*/
#define XTRT_FATAL(MSG)                                                 \
  { std::cerr << "XTRT_FATAL: "						\
	      << __PRETTY_FUNCTION__ << MSG << '\n' << std::flush;	\
    std::exit(EXIT_FAILURE); }

#endif
