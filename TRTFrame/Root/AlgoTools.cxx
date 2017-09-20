// TRTFrame
#include <TRTFrame/Algo.h>

// ROOT
#include <TSystem.h>

// ATLAS
#include <PileupReweighting/PileupReweightingTool.h>
#include <GoodRunsLists/GoodRunsListSelectionTool.h>

EL::StatusCode xTRT::Algo::setupTrackSelectionTools() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_MSG_INFO("Setting up track selections tools");

  ANA_CHECK(ASG_MAKE_ANA_TOOL(m_trackSelToolHandle, InDet::InDetTrackSelectionTool));
  ANA_CHECK(m_trackSelToolHandle.setProperty("CutLevel","TightPrimary"));
  ANA_CHECK(m_trackSelToolHandle.retrieve());
  ANA_MSG_INFO("Set up InDetTrackSelectionTool for Tracks");

  ANA_CHECK(ASG_MAKE_ANA_TOOL(m_trackElecSelToolHandle, InDet::InDetTrackSelectionTool));
  ANA_CHECK(m_trackElecSelToolHandle.setProperty("CutLevel","LooseElectron"));
  ANA_CHECK(m_trackElecSelToolHandle.retrieve());
  ANA_MSG_INFO("Set up InDetTrackSelectionTool for Electrons");

  ANA_CHECK(ASG_MAKE_ANA_TOOL(m_trackMuonSelToolHandle, InDet::InDetTrackSelectionTool));
  ANA_CHECK(m_trackMuonSelToolHandle.setProperty("CutLevel","LooseMuon"));
  ANA_CHECK(m_trackMuonSelToolHandle.retrieve());
  ANA_MSG_INFO("Set up InDetTrackSelectionTool for Muons");

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algo::enableGRLTool() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_MSG_INFO("Setting up GRL tool");

  for ( auto const& entry : config()->GRLFiles() ) {
    ANA_MSG_INFO("GRL File: " << entry.c_str());
  }

  ANA_CHECK(ASG_MAKE_ANA_TOOL(m_GRLToolHandle, GoodRunsListSelectionTool));
  ANA_CHECK(m_GRLToolHandle.setProperty("GoodRunsListVec", config()->GRLFiles()));
  ANA_CHECK(m_GRLToolHandle.setProperty("PassThrough",     false));
  ANA_CHECK(m_GRLToolHandle.setProperty("OutputLevel",     msg().level()));
  ANA_CHECK(m_GRLToolHandle.retrieve());
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algo::enablePRWTool() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_MSG_INFO("Setting up PRW tool");

  ANA_CHECK(ASG_MAKE_ANA_TOOL(m_PRWToolHandle, CP::PileupReweightingTool));
  ANA_CHECK(m_PRWToolHandle.setProperty("ConfigFiles",        config()->PRWConfFiles()));
  ANA_CHECK(m_PRWToolHandle.setProperty("LumiCalcFiles",      config()->PRWLumiFiles()));
  ANA_CHECK(m_PRWToolHandle.setProperty("DataScaleFactor",    1.0/1.09));
  ANA_CHECK(m_PRWToolHandle.setProperty("DataScaleFactorUP",  1.0));
  ANA_CHECK(m_PRWToolHandle.setProperty("DataScaleFactorDOWN",1.0/1.18));
  ANA_CHECK(m_PRWToolHandle.setProperty("OutputLevel",        msg().level()));
  ANA_CHECK(m_PRWToolHandle.retrieve());

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algo::enableTriggerTools() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_MSG_INFO("Setting up trigger tools");

  ANA_CHECK(ASG_MAKE_ANA_TOOL(m_trigConfToolHandle, TrigConf::xAODConfigTool));
  ANA_CHECK(m_trigConfToolHandle.setProperty("OutputLevel", msg().level()));
  ANA_CHECK(m_trigConfToolHandle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_trigConfToolHandle);

  ANA_CHECK(ASG_MAKE_ANA_TOOL(m_trigDecToolHandle, Trig::TrigDecisionTool));
  ANA_CHECK(m_trigDecToolHandle.setProperty("ConfigTool",      m_trigConfToolHandle.getHandle()));
  ANA_CHECK(m_trigDecToolHandle.setProperty("TrigDecisionKey", "xTrigDecision"));
  ANA_CHECK(m_trigDecToolHandle.setProperty("OutputLevel",     msg().level()));
  ANA_CHECK(m_trigDecToolHandle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_trigDecToolHandle);

  ANA_CHECK(ASG_MAKE_ANA_TOOL(m_trigMatchingToolHandle, Trig::MatchingTool));
  ANA_CHECK(m_trigMatchingToolHandle.setProperty("TrigDecisionTool",m_trigDecToolHandle.getHandle()));
  ANA_CHECK(m_trigMatchingToolHandle.setProperty("OutputLevel",     msg().level()));
  ANA_CHECK(m_trigMatchingToolHandle.retrieve());
  ANA_MSG_DEBUG("Retrieved tool: " << m_trigMatchingToolHandle);

  return EL::StatusCode::SUCCESS;
}
