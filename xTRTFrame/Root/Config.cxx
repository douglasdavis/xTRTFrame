#include <xTRTFrame/Config.h>
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
    std::cout << "====== xTRTFrame YAML Configuration ======" << std::endl;
    std::cout << std::endl;
    std::cout << m_fileAsString << std::endl;
    std::cout << std::endl;
  }

  auto node_GRL = config["GRL"];
  m_useGRL = checkAndGet<bool>(node_GRL,"Use","(from GRL node)");
  if ( m_useGRL) {
    for ( const auto& grlf : node_GRL["XMLs"] ) {
      m_GRLFiles.push_back(grlf.as<std::string>());
    }
  }

  auto node_PRW = config["PRW"];
  m_usePRW = checkAndGet<bool>(node_PRW,"Use","(from PRW node)");
  if ( m_usePRW ) {
    if ( node_PRW["ConfFiles"] ) {
      for ( const auto& prwcf : node_PRW["ConfFiles"] ) {
        m_PRWConfFiles.push_back(prwcf.as<std::string>());
      }
    }
    else {
      XTRT_FATAL("No ConfFiles list in PRW section of  YAML config");
    }
    if ( node_PRW["LumiFiles"] ) {
      for ( const auto& prwlf : node_PRW["LumiFiles"] ) {
        m_PRWConfFiles.push_back(prwlf.as<std::string>());
      }
    }
    else {
      XTRT_FATAL("No LumiFiles list in PRW section of YAML config");
    }
  }

  auto node_Trig = config["Trig"];
  m_useTrig = checkAndGet<bool>(node_Trig,"Use","(from Trig node)");
  if ( m_useTrig ) {
    auto fillTrigs = [&node_Trig](auto& vec, const std::string& cat) {
      if ( node_Trig[cat] ) {
        for ( const auto& trig : node_Trig[cat] ) {
          vec.push_back(trig.as<std::string>());
        }
      }
      else {
        XTRT_FATAL("Cannot find " << cat << " in Trigger node.");
      }
    };
    fillTrigs(m_elTrigs,   "electron");
    fillTrigs(m_dielTrigs, "dielectron");
    fillTrigs(m_muTrigs,   "muon");
    fillTrigs(m_dimuTrigs, "dimuon");
    fillTrigs(m_miscTrigs, "misc");
  }

  auto node_IDTS = config["IDTS"];
  m_useIDTS = checkAndGet<bool>(node_IDTS,"Use","(from IDTS node)");

  auto node_tracks = config["Tracks"];
  auto node_elecs  = config["Electrons"];
  auto node_muons  = config["Muons"];

  cut_track_p      = checkAndGet<float>(node_tracks,"p","(from Tracks node)");
  cut_track_pT     = checkAndGet<float>(node_tracks,"pT","(from Tracks node)");
  cut_track_eta    = checkAndGet<float>(node_tracks,"eta","(from Tracks node)");
  cut_track_nSi    = checkAndGet<int>(node_tracks,"nSi","(from Tracks node)");
  cut_track_nPix   = checkAndGet<int>(node_tracks,"nPix","(from Tracks node)");
  cut_track_nTRT   = checkAndGet<int>(node_tracks,"nTRT","(from Tracks node)");

  cut_elec_p   = checkAndGet<float>(node_elecs,"p","(from Electrons node)");
  cut_elec_pT  = checkAndGet<float>(node_elecs,"pT","(from Electrons node)");
  cut_elec_eta = checkAndGet<float>(node_elecs,"eta","(from Electrons node)");

  cut_muon_p   = checkAndGet<float>(node_muons,"p","(from Muons node)");
  cut_muon_pT  = checkAndGet<float>(node_muons,"pT","(from Muons node)");
  cut_muon_eta = checkAndGet<float>(node_muons,"eta","(from Muons node)");

  return true;
}
