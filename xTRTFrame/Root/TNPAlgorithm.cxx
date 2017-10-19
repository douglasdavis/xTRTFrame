#include <xTRTFrame/TNPAlgorithm.h>

ClassImp(xTRT::TNPAlgorithm)

xTRT::TNPAlgorithm::TNPAlgorithm() : xTRT::Algorithm(),
  m_selectionCalled(false),
  m_containersMade(false),
  m_probeIndices(),
  m_tagIndices()
{}

xTRT::TNPAlgorithm::~TNPAlgorithm() {}

EL::StatusCode xTRT::TNPAlgorithm::setupJob(EL::Job& job) {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::setupJob(job));
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::TNPAlgorithm::histInitialize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::histInitialize());
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::TNPAlgorithm::fileExecute() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::fileExecute());
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::TNPAlgorithm::changeInput(bool firstFile) {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::changeInput(firstFile));
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::TNPAlgorithm::initialize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::initialize());
  clear();
  m_tag_pT      = config()->getOpt<float>("TNP.Tag.pT",      25);
  m_probe_pT    = config()->getOpt<float>("TNP.Probe.pT",    15);
  m_probe_relpT = config()->getOpt<float>("TNP.Probe.relpT", 0.25);
  m_probe_nTRT  = config()->getOpt<int>  ("TNP.Probe.nTRT",  15);
  m_tag_nTRT    = config()->getOpt<int>  ("TNP.Tag.nTRT",    15);
  m_probe_nPix  = config()->getOpt<int>  ("TNP.Probe.nPix",  1);
  m_tag_nPix    = config()->getOpt<int>  ("TNP.Tag.nPix",    1);
  m_probe_nSi   = config()->getOpt<int>  ("TNP.Probe.nSi",   7);
  m_tag_nSi     = config()->getOpt<int>  ("TNP.Tag.nSi",     7);
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::TNPAlgorithm::execute() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::execute());
  clear();
  ANA_CHECK(makeContainers());
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::TNPAlgorithm::postExecute() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::postExecute());
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::TNPAlgorithm::finalize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::finalize());
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::TNPAlgorithm::histFinalize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::histFinalize());
  return EL::StatusCode::SUCCESS;
}
