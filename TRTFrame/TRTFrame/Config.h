#ifndef TRTFrame_Config_h
#define TRTFrame_Config_h

#include <vector>
#include <string>
#include <map>
#include <yaml-cpp/yaml.h>

namespace xTRT {

  class Config {

  private:
    std::string              m_fileAsString;
    bool                     m_useGRL;
    bool                     m_usePRW;
    bool                     m_useTrig;
    std::vector<std::string> m_GRLFiles;
    std::vector<std::string> m_PRWConfFiles;
    std::vector<std::string> m_PRWLumiFiles;

    std::vector<std::string> m_elTrigs;
    std::vector<std::string> m_dielTrigs;
    std::vector<std::string> m_muTrigs;
    std::vector<std::string> m_dimuTrigs;
    std::vector<std::string> m_miscTrigs;

    float cut_track_p;
    float cut_track_pT;
    float cut_track_eta;
    int   cut_track_nSi;
    int   cut_track_nPix;
    int   cut_track_nTRT;

    float cut_elec_p;
    float cut_elec_pT;
    float cut_elec_eta;

    float cut_muon_p;
    float cut_muon_pT;
    float cut_muon_eta;

  public:
    Config();
    virtual ~Config();

    bool parse(const std::string fileName, bool print_conf);

    bool useGRL()  const;
    bool usePRW()  const;
    bool useTrig() const;

    const std::vector<std::string>& GRLFiles()     const;
    const std::vector<std::string>& PRWConfFiles() const;
    const std::vector<std::string>& PRWLumiFiles() const;

    const std::vector<std::string>& electronTriggers()   const;
    const std::vector<std::string>& dielectronTriggers() const;
    const std::vector<std::string>& muonTriggers()       const;
    const std::vector<std::string>& dimuonTriggers()     const;
    const std::vector<std::string>& miscTriggers()       const;

    template <typename T>
    T customOpt(const std::string& name) const;

    float track_p()    const;
    float track_pT()   const;
    float track_eta()  const;
    int   track_nSi()  const;
    int   track_nPix() const;
    int   track_nTRT() const;

    float muon_p()    const;
    float muon_pT()   const;
    float muon_eta()  const;

    float elec_p()    const;
    float elec_pT()   const;
    float elec_eta()  const;

  };
}

inline bool xTRT::Config::useGRL()  const { return m_useGRL;  }
inline bool xTRT::Config::usePRW()  const { return m_usePRW;  }
inline bool xTRT::Config::useTrig() const { return m_useTrig; }

inline const std::vector<std::string>& xTRT::Config::GRLFiles()     const { return m_GRLFiles;     }
inline const std::vector<std::string>& xTRT::Config::PRWConfFiles() const { return m_PRWConfFiles; }
inline const std::vector<std::string>& xTRT::Config::PRWLumiFiles() const { return m_PRWLumiFiles; }

inline const std::vector<std::string>& xTRT::Config::electronTriggers()   const { return m_elTrigs;   }
inline const std::vector<std::string>& xTRT::Config::dielectronTriggers() const { return m_dielTrigs; }
inline const std::vector<std::string>& xTRT::Config::muonTriggers()       const { return m_muTrigs;   }
inline const std::vector<std::string>& xTRT::Config::dimuonTriggers()     const { return m_dimuTrigs; }
inline const std::vector<std::string>& xTRT::Config::miscTriggers()       const { return m_miscTrigs; }

template <typename T>
inline T xTRT::Config::customOpt(const std::string& name) const {
  return YAML::Load(m_fileAsString)[name].as<T>();
}

inline float xTRT::Config::track_p()    const { return cut_track_p;    }
inline float xTRT::Config::track_pT()   const { return cut_track_pT;   }
inline float xTRT::Config::track_eta()  const { return cut_track_eta;  }
inline int   xTRT::Config::track_nSi()  const { return cut_track_nSi;  }
inline int   xTRT::Config::track_nPix() const { return cut_track_nPix; }
inline int   xTRT::Config::track_nTRT() const { return cut_track_nTRT; }

inline float xTRT::Config::elec_p()    const { return cut_elec_p;    }
inline float xTRT::Config::elec_pT()   const { return cut_elec_pT;   }
inline float xTRT::Config::elec_eta()  const { return cut_elec_eta;  }

inline float xTRT::Config::muon_p()    const { return cut_muon_p;    }
inline float xTRT::Config::muon_pT()   const { return cut_muon_pT;   }
inline float xTRT::Config::muon_eta()  const { return cut_muon_eta;  }

#endif
