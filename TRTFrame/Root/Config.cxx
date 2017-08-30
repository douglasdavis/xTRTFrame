#include <TRTFrame/Config.h>
#include <iostream>
#include <sstream>
#include <fstream>

xTRT::Config::Config() {}

xTRT::Config::~Config() {}

bool xTRT::Config::parse(const std::string fileName) {
  YAML::Node config = YAML::LoadFile(fileName);
  std::ifstream ifs(fileName.c_str());
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  m_fileAsString = buffer.str();
  std::cout << m_fileAsString << std::endl;

  auto node_GRL = config["GRL"];
  m_useGRL = node_GRL["Use"].as<bool>();
  if ( m_useGRL) {
    for ( const auto grlf : node_GRL["XMLs"] ) {
      m_GRLFiles.push_back(grlf.as<std::string>());
    }
  }

  auto node_PRW = config["PRW"];
  m_usePRW = node_PRW["Use"].as<bool>();
  if ( m_usePRW ) {
    for ( const auto prwcf : node_PRW["ConfFiles"] ) {
      m_PRWConfFiles.push_back(prwcf.as<std::string>());
    }
    for ( const auto prwlf : node_PRW["LumiFiles"] ) {
      m_PRWConfFiles.push_back(prwlf.as<std::string>());
    }
  }

  return true;
}
