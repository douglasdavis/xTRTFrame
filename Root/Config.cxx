#include <TRTFrame/Config.h>
#include <yaml-cpp/yaml.h>
#include <iostream>

TRTF::Config::Config() {}

TRTF::Config::~Config() {}

bool TRTF::Config::parse(const std::string fileName) {
  YAML::Node config = YAML::LoadFile(fileName);

  auto node_GRL = config["GRL"];
  m_useGRL = node_GRL["Use"].as<bool>();
  if ( m_useGRL) {
    for ( const auto grlf : node_GRL["XMLs"] ) {
      m_GRLFiles.push_back(grlf.as<std::string>());
    }
  }

  auto node_PRW = config["PRW"];
  m_usePRW = node_PRW["Use"].as<bool>();
  if ( m_usePRW) {
    for ( const auto prwcf : node_PRW["ConfFiles"] ) {
      m_PRWConfFiles.push_back(prwcf.as<std::string>());
    }
    for ( const auto prwlf : node_PRW["LumiFiles"] ) {
      m_PRWConfFiles.push_back(prwlf.as<std::string>());
    }
  }

  for ( auto const& entry : this->GRLFiles() ) {
    std::cout << "** " << entry << std::endl;
  }

  return true;
}
