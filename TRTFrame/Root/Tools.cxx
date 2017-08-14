// TRTFrame
#include <TRTFrame/LoopAlg.h>

// ROOT
#include <TSystem.h>

// ATLAS
#include <PileupReweighting/PileupReweightingTool.h>
#include <GoodRunsLists/GoodRunsListSelectionTool.h>

#define SET_DUAL_TOOL(TOOLHANDLE, TOOLTYPE, TOOLNAME) \
  ASG_SET_ANA_TOOL_TYPE(TOOLHANDLE, TOOLTYPE);        \
  TOOLHANDLE.setName(TOOLNAME);

EL::StatusCode TRTF::LoopAlg::setupTrackSelectionTools() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);

  if ( !m_trackSelToolHandle.isUserConfigured() ) {
    SET_DUAL_TOOL(m_trackSelToolHandle, InDet::InDetTrackSelectionTool, "TrackSelTool");
    ANA_CHECK(m_trackSelToolHandle.setProperty("CutLevel","TightPrimary"));
    ANA_CHECK(m_trackSelToolHandle.setProperty("maxAbsEta",2.0));
    ANA_CHECK(m_trackSelToolHandle.setProperty("maxTrtEtaAcceptance",0.0));
    ANA_CHECK(m_trackSelToolHandle.setProperty("maxEtaForTrtHitCuts",2.0));
    ANA_CHECK(m_trackSelToolHandle.setProperty("minNTrtHits",15));
    ANA_CHECK(m_trackSelToolHandle.retrieve());
  }

  if ( !m_trackElecSelToolHandle.isUserConfigured() ) {
    SET_DUAL_TOOL(m_trackElecSelToolHandle, InDet::InDetTrackSelectionTool, "TrackElecSelTool");
    ANA_CHECK(m_trackElecSelToolHandle.setProperty("CutLevel","LooseElectron"));
    ANA_CHECK(m_trackElecSelToolHandle.setProperty("maxAbsEta",2.0));
    ANA_CHECK(m_trackElecSelToolHandle.setProperty("maxTrtEtaAcceptance",0.0));
    ANA_CHECK(m_trackElecSelToolHandle.setProperty("maxEtaForTrtHitCuts",2.0));
    ANA_CHECK(m_trackElecSelToolHandle.setProperty("minNTrtHits",15));
    ANA_CHECK(m_trackElecSelToolHandle.retrieve());
  }

  if ( !m_trackMuonSelToolHandle.isUserConfigured() ) {
    SET_DUAL_TOOL(m_trackMuonSelToolHandle, InDet::InDetTrackSelectionTool, "TrackMuonSelTool");
    ANA_CHECK(m_trackMuonSelToolHandle.setProperty("CutLevel","LooseMuon"));
    ANA_CHECK(m_trackMuonSelToolHandle.setProperty("maxAbsEta",2.0));
    ANA_CHECK(m_trackMuonSelToolHandle.setProperty("maxTrtEtaAcceptance",0.1));
    ANA_CHECK(m_trackMuonSelToolHandle.setProperty("maxEtaForTrtHitCuts",2.0));
    ANA_CHECK(m_trackMuonSelToolHandle.setProperty("minNTrtHits",15));
    ANA_CHECK(m_trackMuonSelToolHandle.retrieve());
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TRTF::LoopAlg::enableGRLTool() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ATH_MSG_INFO("Setting up GRL tool");

  for ( auto const& entry : config()->GRLFiles() ) {
    ATH_MSG_INFO("GRL File: " << entry.c_str());
  }

  SET_DUAL_TOOL(m_GRLToolHandle, GoodRunsListSelectionTool, "GRLTool");
  ANA_CHECK(m_GRLToolHandle.setProperty("GoodRunsListVec", config()->GRLFiles()));
  ANA_CHECK(m_GRLToolHandle.setProperty("PassThrough",     false));
  ANA_CHECK(m_GRLToolHandle.setProperty("OutputLevel",     msg().level()));
  ANA_CHECK(m_GRLToolHandle.retrieve());
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TRTF::LoopAlg::enablePRWTool() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ATH_MSG_INFO("Setting up PRW tool");

  if ( !m_PRWToolHandle.isUserConfigured() ) {
    SET_DUAL_TOOL(m_PRWToolHandle, CP::PileupReweightingTool, "PRWTool");
    ANA_CHECK(m_PRWToolHandle.setProperty("ConfigFiles",        config()->PRWConfFiles()));
    ANA_CHECK(m_PRWToolHandle.setProperty("LumiCalcFiles",      config()->PRWLumiFiles()));
    ANA_CHECK(m_PRWToolHandle.setProperty("DataScaleFactor",    1.0/1.09));
    ANA_CHECK(m_PRWToolHandle.setProperty("DataScaleFactorUP",  1.0));
    ANA_CHECK(m_PRWToolHandle.setProperty("DataScaleFactorDOWN",1.0/1.18));
    ANA_CHECK(m_PRWToolHandle.setProperty("OutputLevel",        msg().level()));
    ANA_CHECK(m_PRWToolHandle.retrieve());
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode TRTF::LoopAlg::enableTriggerTools() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  std::string toolName = "";
  if ( !m_trigDecToolHandle.isUserConfigured() ) {
    toolName = "xAODConfigTool";
    SET_DUAL_TOOL(m_trigConfToolHandle, TrigConf::xAODConfigTool, toolName);
    ANA_CHECK(m_trigConfToolHandle.setProperty("OutputLevel", msg().level()));
    ANA_CHECK(m_trigConfToolHandle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_trigConfToolHandle);

    toolName = "TrigDecisionTool";
    SET_DUAL_TOOL(m_trigDecToolHandle, Trig::TrigDecisionTool, toolName);
    ANA_CHECK(m_trigDecToolHandle.setProperty("ConfigTool",       m_trigConfToolHandle.getHandle()));
    ANA_CHECK(m_trigDecToolHandle.setProperty("TrigDecisionKey", "xTrigDecision"));
    ANA_CHECK(m_trigDecToolHandle.setProperty("OutputLevel",     msg().level()));
    ANA_CHECK(m_trigDecToolHandle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_trigDecToolHandle);
  }
  if ( !m_trigMatchingToolHandle.isUserConfigured() ) {
    toolName = "TrigMatchingTool";
    SET_DUAL_TOOL(m_trigMatchingToolHandle, Trig::MatchingTool, toolName);
    ANA_CHECK(m_trigMatchingToolHandle.setProperty("TrigDecisionTool",m_trigDecToolHandle.getHandle()));
    ANA_CHECK(m_trigMatchingToolHandle.setProperty("OutputLevel",     msg().level()));
    ANA_CHECK(m_trigMatchingToolHandle.retrieve());
    ANA_MSG_DEBUG("Retrieved tool: " << m_trigMatchingToolHandle);
  }
  return EL::StatusCode::SUCCESS;
}
