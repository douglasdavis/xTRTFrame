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

// xTRTFrame
#include <xTRTFrame/Algorithm.h>

// this is needed to distribute the algorithm to the workers
ClassImp(xTRT::Algorithm)

xTRT::Algorithm::Algorithm()
  : m_config()
{
  SetName("xTRT::Algorithm");
}

xTRT::Algorithm::~Algorithm() {}

EL::StatusCode xTRT::Algorithm::setupJob(EL::Job& job) {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  job.options()->setDouble(EL::Job::optXAODSummaryReport, 0);
  job.useXAOD();
  ANA_CHECK(xAOD::Init("xTRTFrame"));

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algorithm::histInitialize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  TH1::SetDefaultSumw2();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algorithm::fileExecute() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algorithm::changeInput(bool firstFile) {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  (void)firstFile;
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algorithm::initialize() {
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

EL::StatusCode xTRT::Algorithm::execute() {
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

EL::StatusCode xTRT::Algorithm::postExecute() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algorithm::finalize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  if ( config()->useIDTS() ) {
    ANA_CHECK(m_trackSelToolHandle->finalize());
    ANA_CHECK(m_trackSelElecToolHandle->finalize());
    ANA_CHECK(m_trackSelMuonToolHandle->finalize());
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::Algorithm::histFinalize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  return EL::StatusCode::SUCCESS;
}
