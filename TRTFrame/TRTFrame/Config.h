#ifndef TRTFrame_Config_h
#define TRTFrame_Config_h

#include <vector>
#include <string>
#include <map>

namespace TRTF {

  class Config {

  private:
    bool                     m_useGRL;
    bool                     m_usePRW;
    std::vector<std::string> m_GRLFiles;
    std::vector<std::string> m_PRWConfFiles;
    std::vector<std::string> m_PRWLumiFiles;

    std::map<std::string,std::string> m_customOptions;

  public:
    Config();
    virtual ~Config();

    bool parse(const std::string fileName);

    bool useGRL() const;
    bool usePRW() const;
    const std::vector<std::string>& GRLFiles()     const;
    const std::vector<std::string>& PRWConfFiles() const;
    const std::vector<std::string>& PRWLumiFiles() const;

    const std::string& custom(const std::string& name) const;

  };
}

#include "Config.icc"

#endif
