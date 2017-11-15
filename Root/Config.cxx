#include <xTRTFrame/Config.h>
#include <iostream>
#include <sstream>
#include <fstream>

xTRT::Config::Config() {}

xTRT::Config::~Config() {}

bool xTRT::Config::parse(const std::string fileName, bool print_conf, bool mcMode) {
  m_rootEnv = std::make_unique<TEnv>(fileName.c_str());

  m_mcMode  = mcMode;
  m_useGRL  = m_rootEnv->GetValue("GRL",false);
  m_usePRW  = m_rootEnv->GetValue("PRW",false);
  m_useTrig = m_rootEnv->GetValue("Trig",false);
  m_useIDTS = m_rootEnv->GetValue("IDTS",false);

  auto fillVecFromSplit = [](const std::string& fullString, std::vector<std::string>& strVec) {
    if ( fullString.find(",") != std::string::npos ) {
      auto splits = xTRT::stringSplit(fullString,',');
      for ( auto sitr : splits ) {
        strVec.push_back(sitr);
      }
    }
  };

  if ( m_useGRL ) {
    std::string grlfull = m_rootEnv->GetValue("GRLFiles","None");
    fillVecFromSplit(grlfull,m_GRLFiles);
  }

  if ( m_usePRW ) {
    std::string prwconffull = m_rootEnv->GetValue("PRWConf","None");
    std::string prwlumifull = m_rootEnv->GetValue("PRWLumi","None");
    fillVecFromSplit(prwconffull,m_PRWConfFiles);
    fillVecFromSplit(prwlumifull,m_PRWLumiFiles);
  }

  if ( m_useTrig ) {
    std::string eltrigs   = m_rootEnv->GetValue("Trig.Electron","none");
    std::string mutrigs   = m_rootEnv->GetValue("Trig.Muon","none");
    std::string dieltrigs = m_rootEnv->GetValue("Trig.Dielectron","none");
    std::string dimuTrigs = m_rootEnv->GetValue("Trig.Dimuon","none");
    std::string miscTrigs = m_rootEnv->GetValue("Trig.Misc","none");
    fillVecFromSplit(eltrigs,m_elTrigs);
    fillVecFromSplit(mutrigs,m_muTrigs);
    fillVecFromSplit(dieltrigs,m_dielTrigs);
    fillVecFromSplit(dimuTrigs,m_dimuTrigs);
    fillVecFromSplit(miscTrigs,m_miscTrigs);
  }

  m_eventPrintCounter = m_rootEnv->GetValue("EventPrintCounter",1000);

  cut_track_p        = m_rootEnv->GetValue("Tracks.p",0.0);
  cut_track_pT       = m_rootEnv->GetValue("Tracks.pT",0.0);
  cut_track_eta      = m_rootEnv->GetValue("Tracks.eta",2.0);
  cut_track_nSi      = m_rootEnv->GetValue("Tracks.nSi",-1);
  cut_track_nPix     = m_rootEnv->GetValue("Tracks.nPix",-1);
  cut_track_nTRT     = m_rootEnv->GetValue("Tracks.nTRT",15);
  cut_track_nTRTprec = m_rootEnv->GetValue("Tracks.nTRTprec",5);

  cut_elec_p           = m_rootEnv->GetValue("Electrons.p",0.0);
  cut_elec_pT          = m_rootEnv->GetValue("Electrons.pT",0.0);
  cut_elec_eta         = m_rootEnv->GetValue("Electrons.eta",2.0);
  cut_elec_UTC         = m_rootEnv->GetValue("Electrons.UseTrackCuts",false);
  cut_elec_relpT       = m_rootEnv->GetValue("Electrons.RelpT",0.0);
  cut_elec_TM          = m_rootEnv->GetValue("Electrons.TruthMatched",false);
  cut_elec_fromZ       = m_rootEnv->GetValue("Electrons.FromZ",false);
  cut_elec_fromJPsi    = m_rootEnv->GetValue("Electrons.FromJPsi",false);
  cut_elec_fromZorJPsi = m_rootEnv->GetValue("Electrons.FromZorJPsi",false);

  cut_muon_p           = m_rootEnv->GetValue("Muons.p",0.0);
  cut_muon_pT          = m_rootEnv->GetValue("Muons.pT",0.0);
  cut_muon_eta         = m_rootEnv->GetValue("Muons.eta",2.0);
  cut_muon_UTC         = m_rootEnv->GetValue("Muons.UseTrackCuts",false);
  cut_muon_relpT       = m_rootEnv->GetValue("Muons.RelpT",0.0);
  cut_muon_TM          = m_rootEnv->GetValue("Muons.TruthMatched",false);
  cut_muon_fromZ       = m_rootEnv->GetValue("Muons.FromZ",false);
  cut_muon_fromJPsi    = m_rootEnv->GetValue("Muons.FromJPsi",false);
  cut_muon_fromZorJPsi = m_rootEnv->GetValue("Muons.FromZorJPsi",false);

  if ( print_conf ) {
    printConf();
  }

  return true;
}

void xTRT::Config::printConf() const {
  std::cout << "======== xTRT Config ========" << std::endl;

  std::cout << std::boolalpha;
  std::cout << "GRL: " << m_useGRL  << std::endl;
  for ( auto const& gf : m_GRLFiles ) {
    std::cout << "GRLFile: " << gf << std::endl;
  }
  std::cout << "PRW: " << m_usePRW  << std::endl;
  for ( auto const& pf : m_PRWConfFiles ) {
    std::cout << "PRWConfFile: " << pf << std::endl;
  }
  for ( auto const& pf : m_PRWConfFiles ) {
    std::cout << "PRWLumiFile: " << pf << std::endl;
  }
  std::cout << "Trig: " << m_useTrig << std::endl;
  std::cout << "IDTS: " << m_useIDTS << std::endl;
  auto printtrig = [](const std::string& pref, const std::vector<std::string>& v) {
    for ( const auto& t : v ) {
      std::cout << pref << ": " << t << std::endl;
    }
  };
  printtrig("Electron Trigs",m_elTrigs);
  printtrig("Muon Trigs",m_muTrigs);
  printtrig("Dielectron Trigs",m_dielTrigs);
  printtrig("Dimuon Trigs",m_dimuTrigs);
  printtrig("Misc Trigs",m_miscTrigs);

  std::cout << "Event print counter: " << m_eventPrintCounter << std::endl;

  std::cout << "Track p cut: " << cut_track_p << std::endl;
  std::cout << "Track pT cut: " << cut_track_pT << std::endl;
  std::cout << "Track eta cut: " << cut_track_eta << std::endl;
  std::cout << "Track nSi cut: " << cut_track_nSi << std::endl;
  std::cout << "Track nPix cut: " << cut_track_nPix << std::endl;
  std::cout << "Track nTRT cut: " << cut_track_nTRT << std::endl;
  std::cout << "Track nTRTprec cut: " << cut_track_nTRTprec << std::endl;

  std::cout << "Electron p cut: " << cut_elec_p << std::endl;
  std::cout << "Electron pT cut: " << cut_elec_pT << std::endl;
  std::cout << "Electron eta cut: " << cut_elec_eta << std::endl;
  std::cout << "Electron use track cuts: " << cut_elec_UTC << std::endl;
  std::cout << "Electron relative pT cut: " << cut_elec_relpT << std::endl;
  std::cout << "Electron require truth matched: " << cut_elec_TM << std::endl;
  std::cout << "Electron require from Z: " << cut_elec_fromZ << std::endl;
  std::cout << "Electron require from JPsi: " << cut_elec_fromJPsi << std::endl;
  std::cout << "Electron require from Z or JPsi: " << cut_elec_fromZorJPsi << std::endl;

  std::cout << "Muon p cut: " << cut_muon_p << std::endl;
  std::cout << "Muon pT cut: " << cut_muon_pT << std::endl;
  std::cout << "Muon eta cut: " << cut_muon_eta << std::endl;
  std::cout << "Muon use track cuts: " << cut_muon_UTC << std::endl;
  std::cout << "Muon relative pT cut: " << cut_muon_relpT << std::endl;
  std::cout << "Muon require truth matched: " << cut_muon_TM << std::endl;
  std::cout << "Muon require from Z: " << cut_muon_fromZ << std::endl;
  std::cout << "Muon require from JPsi: " << cut_muon_fromJPsi << std::endl;
  std::cout << "Muon require from Z or JPsi: " << cut_muon_fromZorJPsi << std::endl;
}
