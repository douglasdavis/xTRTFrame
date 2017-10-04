// xTRTFrame
#include <xTRTFrame/Algorithm.h>

// ROOT
#include <TSystem.h>

// ATLAS
#include <PileupReweighting/PileupReweightingTool.h>
#include <GoodRunsLists/GoodRunsListSelectionTool.h>

EL::StatusCode xTRT::Algorithm::setupTrackSelectionTools() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_MSG_INFO("Setting up track selections tools");

  ANA_CHECK(m_trackSelToolHandle.setProperty("CutLevel","TightPrimary"));
  ANA_CHECK(m_trackSelToolHandle.retrieve());
  ANA_MSG_INFO("Set up InDetTrackSelectionTool for Tracks");

  ANA_CHECK(m_trackSelElecToolHandle.setProperty("CutLevel","LooseElectron"));
  ANA_CHECK(m_trackSelElecToolHandle.retrieve());
  ANA_MSG_INFO("Set up InDetTrackSelectionTool for Electrons");

  ANA_CHECK(m_trackSelMuonToolHandle.setProperty("CutLevel","LooseMuon"));
  ANA_CHECK(m_trackSelMuonToolHandle.retrieve());
  ANA_MSG_INFO("Set up InDetTrackSelectionTool for Muons");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algorithm::enableGRLTool() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_MSG_INFO("Setting up GRL tool");

  std::vector<std::string> grlfiles = {
    "GoodRunsLists/data15_13TeV/20160720/physics_25ns_20.7.xml",
    "GoodRunsLists/data16_13TeV/20161101/physics_25ns_20.7.xml"
  };
  for ( auto const& entry : config()->GRLFiles() ) {
    if ( entry == "default" ) continue;
    grlfiles.push_back(entry);
  }
  for ( auto const& entry : grlfiles ) {
    ANA_MSG_INFO("GRL File: " << entry);
  }

  ANA_CHECK(m_GRLToolHandle.setProperty("GoodRunsListVec", grlfiles));
  ANA_CHECK(m_GRLToolHandle.setProperty("PassThrough",     false));
  ANA_CHECK(m_GRLToolHandle.setProperty("OutputLevel",     msg().level()));
  ANA_CHECK(m_GRLToolHandle.retrieve());
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algorithm::enablePRWTool() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_MSG_INFO("Setting up PRW tool");

  std::vector<std::string> cf;
  std::vector<std::string> lf;
  for ( const auto& s : config()->PRWConfFiles() ) {
    if ( s == "none" || s == "default" ) continue;
    cf.push_back(s);
  }
  for ( auto const& s : config()->PRWLumiFiles() ) {
    if ( s == "none" || s == "default" ) continue;
    lf.push_back(s);
  }

  ANA_CHECK(m_PRWToolHandle.setProperty("ConfigFiles",        cf));
  ANA_CHECK(m_PRWToolHandle.setProperty("LumiCalcFiles",      lf));
  ANA_CHECK(m_PRWToolHandle.setProperty("DataScaleFactor",    1.0/1.09));
  ANA_CHECK(m_PRWToolHandle.setProperty("DataScaleFactorUP",  1.0));
  ANA_CHECK(m_PRWToolHandle.setProperty("DataScaleFactorDOWN",1.0/1.18));
  ANA_CHECK(m_PRWToolHandle.setProperty("OutputLevel",        msg().level()));
  ANA_CHECK(m_PRWToolHandle.retrieve());

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algorithm::enableTriggerTools() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_MSG_INFO("Setting up trigger tools");

  ANA_CHECK(m_trigConfToolHandle.setProperty("OutputLevel", msg().level()));
  ANA_CHECK(m_trigConfToolHandle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_trigConfToolHandle);

  ANA_CHECK(m_trigDecToolHandle.setProperty("ConfigTool",      m_trigConfToolHandle.getHandle()));
  ANA_CHECK(m_trigDecToolHandle.setProperty("TrigDecisionKey", "xTrigDecision"));
  ANA_CHECK(m_trigDecToolHandle.setProperty("OutputLevel",     msg().level()));
  ANA_CHECK(m_trigDecToolHandle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_trigDecToolHandle);

  ANA_CHECK(m_trigMatchingToolHandle.setProperty("TrigDecisionTool",m_trigDecToolHandle.getHandle()));
  ANA_CHECK(m_trigMatchingToolHandle.setProperty("OutputLevel",     msg().level()));
  ANA_CHECK(m_trigMatchingToolHandle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_trigMatchingToolHandle);

  return EL::StatusCode::SUCCESS;
}
