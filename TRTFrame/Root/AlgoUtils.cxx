#include <TRTFrame/Algo.h>

#include <xAODTracking/Vertex.h>
#include <xAODTracking/TrackParticlexAODHelpers.h>

const xAOD::TrackParticleContainer* xTRT::Algo::trackContainer() {
  const xAOD::TrackParticleContainer* trackContainerPtr = nullptr;
  if ( evtStore()->retrieve(trackContainerPtr,"InDetTrackParticles").isFailure() ) {
    ANA_MSG_WARNING("InDetTrackParticles unavailable!");
    return nullptr;
  }
  return trackContainerPtr;
}

const xAOD::ElectronContainer* xTRT::Algo::electronContainer() {
  const xAOD::ElectronContainer* electronContainerPtr = nullptr;
  if ( evtStore()->retrieve(electronContainerPtr,"Electrons").isFailure() ) {
    ANA_MSG_WARNING("Electrons unavailable!");
    return nullptr;
  }
  return electronContainerPtr;
}

const xAOD::MuonContainer* xTRT::Algo::muonContainer() {
  const xAOD::MuonContainer* muonContainerPtr = nullptr;
  if ( evtStore()->retrieve(muonContainerPtr,"Muons").isFailure() ) {
    ANA_MSG_WARNING("Muons unavailable!");
    return nullptr;
  }
  return muonContainerPtr;
}

const xAOD::TruthParticle* xTRT::Algo::truthParticle(const xAOD::TrackParticle* track) {
  const xAOD::TruthParticle* truthParticle = nullptr;
  if ( xTRT::Acc::truthParticleLink.isAvailable(*track) ) {
    const auto truthLink = xTRT::Acc::truthParticleLink(*track);
    if ( truthLink.isValid() ) {
      truthParticle = *truthLink;
    }
  }
  return truthParticle;
}

bool xTRT::Algo::triggerPassed(const std::string trigName) const {
  auto chainGroup = m_trigDecToolHandle->getChainGroup(trigName);
  auto passed = chainGroup->isPassed();
  return passed;
}

bool xTRT::Algo::triggersPassed(const std::vector<std::string>& trigNames) const {
  for ( auto const& name : trigNames ) {
    auto chainGroup = m_trigDecToolHandle->getChainGroup(name);
    if ( chainGroup->isPassed() ) return true;
  }
  return false;
}

float xTRT::Algo::eventWeight() const {
  if ( m_eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION) ) {
    auto weights = m_eventInfo->mcEventWeights();
    if ( !(weights.empty()) ) return weights.at(0);
    return 1.0;
  }
  return 1.0;
}

float xTRT::Algo::averageMu() {
  if ( !(m_eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION)) && m_usePRWTool ) {
    return m_PRWToolHandle->getCorrectedAverageInteractionsPerCrossing(*m_eventInfo,true);
  }
  else {
    return m_eventInfo->averageInteractionsPerCrossing();
  }
}

bool xTRT::Algo::passGRL() const {
  if ( m_eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION) || !m_config->useGRL() ) {
    return true;
  }
  return m_GRLToolHandle->passRunLB(*m_eventInfo);
}

xTRT::HitSummary xTRT::Algo::getHitSummary(const xAOD::TrackParticle* track,
                                              const xAOD::TrackStateValidation* msos,
                                              const xAOD::TrackMeasurementValidation* driftCircle) {
  xTRT::HitSummary hit;
  hit.HTMB        = (get(xTRT::Acc::bitPattern, driftCircle,"bitPattern") & 131072) ? 1 : 0;
  hit.tot         =  get(xTRT::Acc::tot,        driftCircle,"tot");
  hit.gasType     =  get(xTRT::Acc::gasType,    driftCircle,"gasType");
  hit.bec         =  get(xTRT::Acc::bec,        driftCircle,"bec");
  hit.layer       =  get(xTRT::Acc::layer,      driftCircle,"layer");
  hit.strawlayer  =  get(xTRT::Acc::strawlayer, driftCircle,"strawlayer");
  hit.strawnumber =  get(xTRT::Acc::strawnumber,driftCircle,"strawnumber");
  hit.drifttime   =  get(xTRT::Acc::drifttime,  driftCircle,"drifttime");
  hit.T0          =  get(xTRT::Acc::T0,         driftCircle,"T0");

  hit.type       = get(xTRT::Acc::type,      msos,"type");
  hit.localX     = get(xTRT::Acc::localX,    msos,"localX");
  hit.localY     = get(xTRT::Acc::localY,    msos,"localY");
  hit.localTheta = get(xTRT::Acc::localTheta,msos,"localTheta");
  hit.localPhi   = get(xTRT::Acc::localPhi,  msos,"localPhi");
  hit.HitZ       = get(xTRT::Acc::HitZ,      msos,"HitZ");
  hit.HitR       = get(xTRT::Acc::HitR,      msos,"HitR");
  hit.rTrkWire   = get(xTRT::Acc::rTrkWire,  msos,"rTrkWire");

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

uint8_t xTRT::Algo::nSilicon(const xAOD::TrackParticle* track) const {
  uint8_t nPix = -1;
  uint8_t nSCT = -1;
  if ( !track->summaryValue(nPix,xAOD::numberOfPixelHits) ) ANA_MSG_ERROR("No Pix hits?");
  if ( !track->summaryValue(nSCT,xAOD::numberOfSCTHits)   ) ANA_MSG_ERROR("No SCT hits?");
  return (nPix + nSCT);
}

uint8_t xTRT::Algo::nSiliconHoles(const xAOD::TrackParticle* track) const {
  uint8_t nPixHole = -1;
  uint8_t nSCTHole = -1;
  if ( !track->summaryValue(nPixHole,xAOD::numberOfPixelHoles) ) ANA_MSG_ERROR("No Pix holes?");
  if ( !track->summaryValue(nSCTHole,xAOD::numberOfSCTHoles)   ) ANA_MSG_ERROR("No SCT holes?");
  return (nPixHole + nSCTHole);
}

uint8_t xTRT::Algo::nSiliconShared(const xAOD::TrackParticle* track) const {
  uint8_t nSCTSh = -1;
  uint8_t nPixSh = -1;
  if ( !track->summaryValue(nPixSh,xAOD::numberOfPixelSharedHits) ) ANA_MSG_ERROR("No Pix shared?");
  if ( !track->summaryValue(nSCTSh,xAOD::numberOfSCTSharedHits)   ) ANA_MSG_ERROR("No SCT shared?");
  return (nPixSh + nSCTSh);
}

float xTRT::Algo::deltaz0sinTheta(const xAOD::TrackParticle *track, const xAOD::Vertex* vtx) const {
  float delta_z0 = std::fabs(track->z0() + track->vz() - vtx->z());
  float dz0sinth = std::fabs(delta_z0*std::sin(track->theta()));
  return dz0sinth;
}

double xTRT::Algo::d0signif(const xAOD::TrackParticle *track) const {
  double d0sig = xAOD::TrackingHelpers::d0significance(track,
                                                       m_eventInfo->beamPosSigmaX(),
                                                       m_eventInfo->beamPosSigmaY(),
                                                       m_eventInfo->beamPosSigmaXY());
  return d0sig;
}
