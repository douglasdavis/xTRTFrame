// ATLAS
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <AsgTools/MessageCheck.h>
#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>

// ROOT
#include <TSystem.h>
#include <TFile.h>

// TRTFrame
#include <TRTFrame/LoopAlg.h>

// this is needed to distribute the algorithm to the workers
ClassImp(xTRT::LoopAlg)

xTRT::LoopAlg::LoopAlg() {
  SetName("xTRT::LoopAlg");
  m_usePRWTool   = false;
  m_useTrigTools = false;
  m_pidSvc = std::make_shared<xTRT::ParticleIdSvc>();
}

xTRT::LoopAlg::~LoopAlg() {}

EL::StatusCode xTRT::LoopAlg::setupJob(EL::Job& job) {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  //setMsgLevel(MSG::DEBUG);
  job.options()->setDouble(EL::Job::optXAODSummaryReport, 0);
  job.useXAOD();
  ANA_CHECK(xAOD::Init("xTRTrame"));

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::LoopAlg::histInitialize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  TH1::SetDefaultSumw2();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::LoopAlg::fileExecute() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::LoopAlg::changeInput(bool firstFile) {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  (void)firstFile;
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::LoopAlg::initialize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  ATH_MSG_INFO("Number of events = " << m_event->getEntries());

  m_eventCounter = 0;

  if ( m_usePRWTool       ) ANA_CHECK(enablePRWTool());
  if ( config()->useGRL() ) ANA_CHECK(enableGRLTool());
  if ( m_useTrigTools     ) ANA_CHECK(enableTriggerTools());

  ANA_CHECK(setupTrackSelectionTools());

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::LoopAlg::execute() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);

  if ( m_eventCounter % 5000 == 0 ) {
    ATH_MSG_INFO("Event number = " << m_eventCounter);
  }
  m_eventCounter++;

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  m_eventInfo = eventInfo();

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::LoopAlg::postExecute() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::LoopAlg::finalize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(m_trackSelToolHandle->finalize());
  ANA_CHECK(m_trackElecSelToolHandle->finalize());
  ANA_CHECK(m_trackMuonSelToolHandle->finalize());
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::LoopAlg::histFinalize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  return EL::StatusCode::SUCCESS;
}

// saving this for later use
/*
  if ( m_useTrigTools ) {
  std::vector<std::string> dielTrigs;
  dielTrigs.push_back("HLT_e5_lhtight_nod0_e4_etcut");
  dielTrigs.push_back("HLT_e5_lhtight_nod0_e4_etcut_Jpsiee");
  dielTrigs.push_back("HLT_e9_lhtight_nod0_e4_etcut_Jpsiee");
  dielTrigs.push_back("HLT_e14_lhtight_nod0_e4_etcut_Jpsiee");
  dielTrigs.push_back("HLT_e9_etcut_e5_lhtight_nod0_Jpsiee");
  dielTrigs.push_back("HLT_e14_etcut_e5_lhtight_nod0_Jpsiee");
  std::vector<std::string> dimuTrigs;
  dimuTrigs.push_back("HLT_2mu6_bJpsimumu");
  dimuTrigs.push_back("HLT_2mu6_bJpsimumu_delayed");
  dimuTrigs.push_back("HLT_2mu6_bJpsimumu_Lxy0_delayed");
  dimuTrigs.push_back("HLT_2mu10_bJpsimumu");
  dimuTrigs.push_back("HLT_2mu10_bJpsimumu_delayed");
  dimuTrigs.push_back("HLT_2mu10_bJpsimumu_noL2");
  dimuTrigs.push_back("HLT_2mu14");
  dimuTrigs.push_back("HLT_2mu14_nomucomb");
  dimuTrigs.push_back("HLT_2mu15");

  if ( triggersPassed(dimuTrigs) ) { std::cout << "@@@@@ GOT ONE! MU" << std::endl; }
  if ( triggersPassed(dielTrigs) ) { std::cout << "@@@@@ GOT ONE! EL" << std::endl; }
  }
*/
