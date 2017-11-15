/** @file  Config.h
 *  @brief xTRT::Config class header
 *  @class xTRT::Config
 *  @brief Class for handling algorithm configuration options
 *
 *  This class stores all of the options defined in the configuration
 *  file. There are standard options which must be defined and users
 *  also have the ability to define custom options.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef xTRTFrame_Config_h
#define xTRTFrame_Config_h

#include <vector>
#include <string>
#include <map>
#include <type_traits>
#include <memory>

#include <TEnv.h>

#include <xTRTFrame/Utils.h>

namespace xTRT {

  class Config {

  private:

    std::unique_ptr<TEnv>    m_rootEnv;

    bool                     m_mcMode;
    bool                     m_useGRL;
    bool                     m_usePRW;
    bool                     m_useTrig;
    bool                     m_useIDTS;
    std::vector<std::string> m_GRLFiles;
    std::vector<std::string> m_PRWConfFiles;
    std::vector<std::string> m_PRWLumiFiles;

    std::vector<std::string> m_elTrigs;
    std::vector<std::string> m_dielTrigs;
    std::vector<std::string> m_muTrigs;
    std::vector<std::string> m_dimuTrigs;
    std::vector<std::string> m_miscTrigs;

    int m_eventPrintCounter;

    float cut_track_p;
    float cut_track_pT;
    float cut_track_eta;
    int cut_track_nSi;
    int cut_track_nPix;
    int cut_track_nTRT;
    int cut_track_nTRTprec;

    float cut_elec_p;
    float cut_elec_pT;
    float cut_elec_eta;
    float cut_elec_relpT;
    bool  cut_elec_UTC;
    bool  cut_elec_TM;
    bool  cut_elec_fromZ;
    bool  cut_elec_fromJPsi;
    bool  cut_elec_fromZorJPsi;

    float cut_muon_p;
    float cut_muon_pT;
    float cut_muon_eta;
    float cut_muon_relpT;
    bool  cut_muon_UTC;
    bool  cut_muon_TM;
    bool  cut_muon_fromZ;
    bool  cut_muon_fromJPsi;
    bool  cut_muon_fromZorJPsi;

    void printConf() const;

  public:
    Config();
    virtual ~Config();

    /// sets all the config members
    bool parse(const std::string fileName, bool print_conf, bool mcMode = false);

    /// true if MC mode has been declared
    bool mcMode()  const;
    /// true if config says use GRL
    bool useGRL()  const;
    /// true if config says use pileup reweighting
    bool usePRW()  const;
    /// true if config says use trigger tools
    bool useTrig() const;
    /// true if config says use InDetTrackSelectionTools
    bool useIDTS() const;

    /// get list of GRL files defined in the config file
    const std::vector<std::string>& GRLFiles()     const;
    /// get list of PRW config files defined in the config file
    const std::vector<std::string>& PRWConfFiles() const;
    /// get list of PRW lumi files defined in the config file
    const std::vector<std::string>& PRWLumiFiles() const;

    /// get list of electron triggers defined in the config file
    const std::vector<std::string>& electronTriggers()   const;
    /// get list of dielectron triggers defined in the config file
    const std::vector<std::string>& dielectronTriggers() const;
    const std::vector<std::string>& muonTriggers()       const;
    /// get list of dimuon triggers defined in the config file
    const std::vector<std::string>& dimuonTriggers()     const;
    /// get list of misc. triggers defined in the config file
    const std::vector<std::string>& miscTriggers()       const;

    /// get the event print "on factors of" value.
    int eventPrintCounter() const;

    /// get the track momentum cut (minimum cut)
    float track_p()        const;
    /// get the track transverse momentum cut (minimum cut)
    float track_pT()       const;
    /// get the track abs(eta) cut (maximum cut)
    float track_eta()      const;
    /// get the track number of silicon hits cut (Pixel +SCT) (minimum cut)
    int   track_nSi()      const;
    /// get the track number of pixel hits cut (minimum cut)
    int   track_nPix()     const;
    /// get the number of total TRT hits cut (minimum cut)
    int   track_nTRT()     const;
    /// get the number of precision TRT hits cut (minimum cut)
    int   track_nTRTprec() const;

    /// get the muon momentum cut (minimum cut)
    float muon_p()            const;
    /// get the muon transverse momentum cut (minimum cut)
    float muon_pT()           const;
    /// get the muon abs(eta) cut (maximum cut)
    float muon_eta()          const;
    /// get the muon relative transverse momentum cut
    float muon_relpT()        const;
    /// true if config says muons should also use track cuts
    bool  muon_UTC()          const;
    /// true if config says muons should be truth matched
    bool  muon_truthMatched() const;
    /// true if config says muons shoud be from Z
    bool  muon_fromZ()        const;
    /// true if config says muons should be from JPsi
    bool  muon_fromJPsi()     const;
    /// true if config says muons should be from either Z or JPsi
    bool  muon_fromZorJPsi()  const;

    /// get the electron momentum cut (minimum cut)
    float elec_p()            const;
    /// get the electron transverse momentum cut (minimum cut)
    float elec_pT()           const;
    /// get the abs(eta) cut (maximum cut)
    float elec_eta()          const;
    /// get the electron relative transverse momentum cut
    float elec_relpT()        const;
    /// true if config says electrons should also use track cuts
    bool  elec_UTC()          const;
    /// true of config says electrons should be truth matched
    bool  elec_truthMatched() const;
    /// true if config says electrons shoud be from Z
    bool  elec_fromZ()        const;
    /// true if config says electrons should be from JPsi
    bool  elec_fromJPsi()     const;
    /// true if config says electrons should be from either Z or JPsi
    bool  elec_fromZorJPsi()  const;

    /** Get a value defined from in config file
     *
     *  This is for all non string types (bools, ints, floats, etc.).
     *  For strings, use getStr
     *
     *  @param name the name of the variable in the configuration file
     *  @param def the default value
     */
    template <typename T>
    const T getOpt(const char* name, const T def) const;

    /** Get string defined from the config file
     *
     *  This is a special function for getting strings defined in the
     *  config file. We have to make a special case here because the
     *  TEnv class can only handle const char* for default values and
     *  we can't call c_str() on the templated version (getOpt<T>)
     *  which we use for all other types.
     *
     *  @param name the name of the variable in the configuration file
     *  @param def the default string value.
     */
    const std::string getStrOpt(const char* name, const std::string def) const;

  };
}

inline bool xTRT::Config::mcMode()  const { return m_mcMode;  }
inline bool xTRT::Config::useGRL()  const { return m_useGRL;  }
inline bool xTRT::Config::usePRW()  const { return m_usePRW;  }
inline bool xTRT::Config::useTrig() const { return m_useTrig; }
inline bool xTRT::Config::useIDTS() const { return m_useIDTS; }

inline const std::vector<std::string>& xTRT::Config::GRLFiles()     const { return m_GRLFiles;     }
inline const std::vector<std::string>& xTRT::Config::PRWConfFiles() const { return m_PRWConfFiles; }
inline const std::vector<std::string>& xTRT::Config::PRWLumiFiles() const { return m_PRWLumiFiles; }

inline const std::vector<std::string>& xTRT::Config::electronTriggers()   const { return m_elTrigs;   }
inline const std::vector<std::string>& xTRT::Config::dielectronTriggers() const { return m_dielTrigs; }
inline const std::vector<std::string>& xTRT::Config::muonTriggers()       const { return m_muTrigs;   }
inline const std::vector<std::string>& xTRT::Config::dimuonTriggers()     const { return m_dimuTrigs; }
inline const std::vector<std::string>& xTRT::Config::miscTriggers()       const { return m_miscTrigs; }

inline int xTRT::Config::eventPrintCounter() const { return m_eventPrintCounter; }

inline float xTRT::Config::track_p()        const { return cut_track_p;        }
inline float xTRT::Config::track_pT()       const { return cut_track_pT;       }
inline float xTRT::Config::track_eta()      const { return cut_track_eta;      }
inline int   xTRT::Config::track_nSi()      const { return cut_track_nSi;      }
inline int   xTRT::Config::track_nPix()     const { return cut_track_nPix;     }
inline int   xTRT::Config::track_nTRT()     const { return cut_track_nTRT;     }
inline int   xTRT::Config::track_nTRTprec() const { return cut_track_nTRTprec; }

inline float xTRT::Config::elec_p()            const { return cut_elec_p;           }
inline float xTRT::Config::elec_pT()           const { return cut_elec_pT;          }
inline float xTRT::Config::elec_eta()          const { return cut_elec_eta;         }
inline bool  xTRT::Config::elec_UTC()          const { return cut_elec_UTC;         }
inline float xTRT::Config::elec_relpT()        const { return cut_elec_relpT;       }
inline bool  xTRT::Config::elec_truthMatched() const { return cut_elec_TM;          }
inline bool  xTRT::Config::elec_fromZ()        const { return cut_elec_fromZ;       }
inline bool  xTRT::Config::elec_fromJPsi()     const { return cut_elec_fromJPsi;    }
inline bool  xTRT::Config::elec_fromZorJPsi()  const { return cut_elec_fromZorJPsi; }

inline float xTRT::Config::muon_p()            const { return cut_muon_p;           }
inline float xTRT::Config::muon_pT()           const { return cut_muon_pT;          }
inline float xTRT::Config::muon_eta()          const { return cut_muon_eta;         }
inline bool  xTRT::Config::muon_UTC()          const { return cut_muon_UTC;         }
inline float xTRT::Config::muon_relpT()        const { return cut_muon_relpT;       }
inline bool  xTRT::Config::muon_truthMatched() const { return cut_muon_TM;          }
inline bool  xTRT::Config::muon_fromZ()        const { return cut_muon_fromZ;       }
inline bool  xTRT::Config::muon_fromJPsi()     const { return cut_muon_fromJPsi;    }
inline bool  xTRT::Config::muon_fromZorJPsi()  const { return cut_muon_fromZorJPsi; }

template <typename T>
inline const T xTRT::Config::getOpt(const char* name, const T def) const {
  if ( not m_rootEnv->Defined(name) ) {
    XTRT_WARNING(name << " not defined in confg! using default: " << def);
  }
  return m_rootEnv->GetValue(name,def);
}

inline const std::string xTRT::Config::getStrOpt(const char* name, const std::string def) const {
  if ( not m_rootEnv->Defined(name) ) {
    XTRT_WARNING(name << " not defined in confg! using default: " << def);
  }
  std::string ret = m_rootEnv->GetValue(name,def.c_str());
  return ret;
}

#endif
