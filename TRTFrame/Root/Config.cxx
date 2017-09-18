#include <TRTFrame/Config.h>
#include <iostream>
#include <sstream>
#include <fstream>

xTRT::Config::Config() {}

xTRT::Config::~Config() {}

bool xTRT::Config::parse(const std::string fileName, bool print_conf) {
  YAML::Node config = YAML::LoadFile(fileName);
  std::ifstream ifs(fileName.c_str());
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  m_fileAsString = buffer.str();

  if ( print_conf ) {
    std::cout << std::endl;
    std::cout << "====== TRTFrame YAML Configuration ======" << std::endl;
    std::cout << std::endl;
    std::cout << m_fileAsString << std::endl;
    std::cout << std::endl;
  }

  auto node_GRL = config["GRL"];
  m_useGRL = node_GRL["Use"].as<bool>();
  if ( m_useGRL) {
    for ( const auto& grlf : node_GRL["XMLs"] ) {
      m_GRLFiles.push_back(grlf.as<std::string>());
    }
  }

  auto node_PRW = config["PRW"];
  m_usePRW = node_PRW["Use"].as<bool>();
  if ( m_usePRW ) {
    for ( const auto& prwcf : node_PRW["ConfFiles"] ) {
      m_PRWConfFiles.push_back(prwcf.as<std::string>());
    }
    for ( const auto& prwlf : node_PRW["LumiFiles"] ) {
      m_PRWConfFiles.push_back(prwlf.as<std::string>());
    }
  }

  auto node_Trig = config["Trig"];
  m_useTrig = node_Trig["Use"].as<bool>();
  if ( m_useTrig ) {
    auto fillTrigs = [&node_Trig](auto& vec, const std::string& cat) {
      for ( const auto& trig : node_Trig[cat] ) {
        vec.push_back(trig.as<std::string>());
      }
    };
    fillTrigs(m_elTrigs,   "electron");
    fillTrigs(m_dielTrigs, "dielectron");
    fillTrigs(m_muTrigs,   "muon");
    fillTrigs(m_dimuTrigs, "dimuon");
    fillTrigs(m_miscTrigs, "misc");
  }

  cut_track_p      = 0;
  cut_track_pT     = 1000;
  cut_track_eta    = 5;
  cut_track_nSi    = 0;
  cut_track_nPix   = 0;
  cut_track_nTRT   = 15;

  cut_elec_p       = 0;
  cut_elec_pT      = 1000;
  cut_elec_eta     = 5;

  cut_muon_p       = 0;
  cut_muon_pT      = 1000;
  cut_muon_eta     = 5;

  return true;
}
