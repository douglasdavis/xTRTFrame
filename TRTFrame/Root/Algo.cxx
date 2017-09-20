// ATLAS
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <AsgTools/MessageCheck.h>
#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>

// ROOT
#include <TSystem.h>
#include <TFile.h>
#include <TH1.h>

// TRTFrame
#include <TRTFrame/Algo.h>

// this is needed to distribute the algorithm to the workers
ClassImp(xTRT::Algo)

xTRT::Algo::Algo()
  : m_config(),
    m_GRLToolHandle("GRLTool",this),
    m_PRWToolHandle("PRWTool",this),
    m_trigConfToolHandle("xAODConfigTool",this),
    m_trigDecToolHandle("TrigDecisionTool",this),
    m_trigMatchingToolHandle("TrigMatchingTool",this),
    m_trackSelToolHandle("TrackSelTool",this),
    m_trackElecSelToolHandle("TrackElecSelTool",this),
    m_trackMuonSelToolHandle("TrackMuonSelTool",this)
{
  SetName("xTRT::Algo");
}

xTRT::Algo::~Algo() {}

EL::StatusCode xTRT::Algo::setupJob(EL::Job& job) {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  job.options()->setDouble(EL::Job::optXAODSummaryReport, 0);
  job.useXAOD();
  ANA_CHECK(xAOD::Init("TRTFrame"));

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algo::histInitialize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  TH1::SetDefaultSumw2();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algo::fileExecute() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algo::changeInput(bool firstFile) {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  (void)firstFile;
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algo::initialize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();
  ATH_MSG_INFO("Number of events = " << m_event->getEntries());

  m_eventCounter = 0;

  if ( config()->usePRW()  ) ANA_CHECK(enablePRWTool());
  if ( config()->useGRL()  ) ANA_CHECK(enableGRLTool());
  if ( config()->useTrig() ) ANA_CHECK(enableTriggerTools());
  if ( config()->useIDTS() ) ANA_CHECK(setupTrackSelectionTools());

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algo::execute() {
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

EL::StatusCode xTRT::Algo::postExecute() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algo::finalize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  if ( config()->useIDTS() ) {
    ANA_CHECK(m_trackSelToolHandle->finalize());
    ANA_CHECK(m_trackElecSelToolHandle->finalize());
    ANA_CHECK(m_trackMuonSelToolHandle->finalize());
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algo::histFinalize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  return EL::StatusCode::SUCCESS;
}
