#include <TRTLite/LoopAlg.h>

void TRTLite::LoopAlg::buildTrackContainer() {
  if ( m_trackContainer ) return;

  const xAOD::TrackParticleContainer* trackContainerPtr = nullptr;
  if ( m_event->retrieve(trackContainerPtr,"InDetTrackParticles").isFailure() ) {
    // error or warning message needed
    return;
  }
  std::pair<xAOD::TrackParticleContainer*,xAOD::ShallowAuxContainer*> shallowCopy =
    xAOD::shallowCopyContainer(*trackContainerPtr);
  m_trackContainer = std::make_shared<xAOD::TrackParticleContainer>(shallowCopy.first->begin(),
                                                                    shallowCopy.first->end(),
                                                                    SG::VIEW_ELEMENTS);
  if ( !m_store->record(shallowCopy.first, "ShallowTracks") ||
       !m_store->record(shallowCopy.second,"ShallowTracksAux") ) {
    // error or warning message needed
    return;
  }
}

void TRTLite::LoopAlg::buildElectronContainer() {
  if ( m_electronContainer ) return;

  const xAOD::ElectronContainer* electronContainerPtr = nullptr;
  if ( m_event->retrieve(electronContainerPtr,"Electrons").isFailure() ) {
    // error or warning message needed
    return;
  }
  std::pair<xAOD::ElectronContainer*,xAOD::ShallowAuxContainer*> shallowCopy =
    xAOD::shallowCopyContainer(*electronContainerPtr);
  m_electronContainer = std::make_shared<xAOD::ElectronContainer>(shallowCopy.first->begin(),
                                                                  shallowCopy.first->end(),
                                                                  SG::VIEW_ELEMENTS);
  if ( !m_store->record(shallowCopy.first, "ShallowElectrons") ||
       !m_store->record(shallowCopy.second,"ShallowElectronsAux") ) {
    // error or warning message needed
    return;
  }
}

void TRTLite::LoopAlg::buildMuonContainer() {
  if ( m_muonContainer ) return;

  const xAOD::MuonContainer* muonContainerPtr = nullptr;
  if ( m_event->retrieve(muonContainerPtr,"Muons").isFailure() ) {
    // error or warning message needed
    return;
  }
  std::pair<xAOD::MuonContainer*,xAOD::ShallowAuxContainer*> shallowCopy =
    xAOD::shallowCopyContainer(*muonContainerPtr);
  m_muonContainer = std::make_shared<xAOD::MuonContainer>(shallowCopy.first->begin(),
                                                          shallowCopy.first->end(),
                                                          SG::VIEW_ELEMENTS);
  if ( !m_store->record(shallowCopy.first, "ShallowMuons") ||
       !m_store->record(shallowCopy.second,"ShallowMuonsAux") ) {
    // error or warning message needed
    return;
  }
}

const xAOD::TrackParticleContainer* TRTLite::LoopAlg::trackContainer() {
  //buildTrackContainer();
  //return m_trackContainer.get();

  const xAOD::TrackParticleContainer* trackContainerPtr = nullptr;
  if ( evtStore()->retrieve(trackContainerPtr,"InDetTrackParticles").isFailure() ) {
    // error or warning message needed
    return nullptr;
  }
  return trackContainerPtr;

}

const xAOD::ElectronContainer* TRTLite::LoopAlg::electronContainer() {
  //buildElectronContainer();
  //return m_electronContainer.get();

  const xAOD::ElectronContainer* electronContainerPtr = nullptr;
  if ( m_event->retrieve(electronContainerPtr,"Electrons").isFailure() ) {
    // error or warning message needed
    return nullptr;
  }
  return electronContainerPtr;

}

const xAOD::MuonContainer* TRTLite::LoopAlg::muonContainer() {
  //buildMuonContainer();
  //return m_muonContainer.get();

  const xAOD::MuonContainer* muonContainerPtr = nullptr;
  if ( m_event->retrieve(muonContainerPtr,"Muons").isFailure() ) {
    // error or warning message needed
    return nullptr;
  }
  return muonContainerPtr;

}

const xAOD::TruthParticle* TRTLite::LoopAlg::truthParticle(const xAOD::TrackParticle* track) {
  const xAOD::TruthParticle* truthParticle = nullptr;
  if ( TRT::Acc::truthParticleLink.isAvailable(*track) ) {
    const auto truthLink = TRT::Acc::truthParticleLink(*track);
    if ( truthLink.isValid() ) {
      truthParticle = *truthLink;
    }
  }
  return truthParticle;
}

bool TRTLite::LoopAlg::triggerPassed(const std::string trigName) const {
  auto chainGroup = m_trigDecToolHandle->getChainGroup(trigName);
  auto passed = chainGroup->isPassed();
  return passed;
}

bool TRTLite::LoopAlg::triggersPassed(const std::vector<std::string>& trigNames) const {
  for ( auto const& name : trigNames ) {
    auto chainGroup = m_trigDecToolHandle->getChainGroup(name);
    if ( chainGroup->isPassed() ) return true;
  }
  return false;
}

float TRTLite::LoopAlg::eventWeight() const {
  if ( m_eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION) ) {
    auto weights = m_eventInfo->mcEventWeights();
    if ( !(weights.empty()) ) return weights.at(0);
    return 1.0;
  }
  return 1.0;
}

float TRTLite::LoopAlg::averageMu() {
  if ( !(m_eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION)) && m_usePRWTool ) {
    return m_PRWToolHandle->getCorrectedAverageInteractionsPerCrossing(*m_eventInfo,true);
  }
  else {
    return m_eventInfo->averageInteractionsPerCrossing();
  }
}

TRTLite::HitSummary TRTLite::LoopAlg::getHitSummary(const xAOD::TrackParticle* track,
						    const xAOD::TrackStateValidation* msos,
						    const xAOD::TrackMeasurementValidation* driftCircle) {
  TRTLite::HitSummary hit;
  hit.HTMB        = (get(TRT::Acc::bitPattern, driftCircle,"bitPattern") & 131072) ? 1 : 0;
  hit.tot         =  get(TRT::Acc::tot,        driftCircle,"tot");
  hit.gasType     =  get(TRT::Acc::gasType,    driftCircle,"gasType");
  hit.bec         =  get(TRT::Acc::bec,        driftCircle,"bec");
  hit.layer       =  get(TRT::Acc::layer,      driftCircle,"layer");
  hit.strawlayer  =  get(TRT::Acc::strawlayer, driftCircle,"strawlayer");
  hit.strawnumber =  get(TRT::Acc::strawnumber,driftCircle,"strawnumber");
  hit.drifttime   =  get(TRT::Acc::drifttime,  driftCircle,"drifttime");
  hit.T0          =  get(TRT::Acc::T0,         driftCircle,"T0");

  hit.localTheta = get(TRT::Acc::localTheta,msos,"localTheta");
  hit.localPhi   = get(TRT::Acc::localPhi,  msos,"localPhi");
  hit.HitZ       = get(TRT::Acc::HitZ,      msos,"HitZ");
  hit.HitR       = get(TRT::Acc::HitR,      msos,"HitR");
  hit.rTrkWire   = get(TRT::Acc::rTrkWire,  msos,"rTrkWire");

  int part = 0; // 0: barrel ... 1: ECA ... 2: ECB
  if ( std::abs(hit.bec) == 2 ) {
    part = ( hit.layer < 6 ) ? 1 : 2;
  }
  if ( hit.bec < 0 ) part = -part;

  hit.L = 999.0;
  float trackTheta = track->theta();
  float trackPhi   = track->phi();
  float strawPhi   = hit.localPhi;
  if ( part == 0 ) {
    hit.L = 2*std::sqrt(4.0-hit.rTrkWire*hit.rTrkWire)*1.0/std::fabs(std::sin(trackTheta));
  }
  else {
    hit.L = 2*std::sqrt(4.0-hit.rTrkWire*hit.rTrkWire)*1.0/
      std::sqrt(1.0-std::sin(trackTheta)*std::sin(trackTheta)*
                std::cos(trackPhi-strawPhi)*std::cos(trackPhi-strawPhi));
  }

  return hit;
}
