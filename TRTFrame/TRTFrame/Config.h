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

#endif
