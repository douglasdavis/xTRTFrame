#include <TRTFrame/LoopAlg.h>

#include <xAODTracking/Vertex.h>
#include <xAODTracking/TrackParticlexAODHelpers.h>

const xAOD::TrackParticleContainer* TRTF::LoopAlg::trackContainer() {
  const xAOD::TrackParticleContainer* trackContainerPtr = nullptr;
  if ( evtStore()->retrieve(trackContainerPtr,"InDetTrackParticles").isFailure() ) {
    ANA_MSG_WARNING("InDetTrackParticles unavailable!");
    return nullptr;
  }
  return trackContainerPtr;
}

const xAOD::ElectronContainer* TRTF::LoopAlg::electronContainer() {
  const xAOD::ElectronContainer* electronContainerPtr = nullptr;
  if ( evtStore()->retrieve(electronContainerPtr,"Electrons").isFailure() ) {
    ANA_MSG_WARNING("Electrons unavailable!");
    return nullptr;
  }
  return electronContainerPtr;
}

const xAOD::MuonContainer* TRTF::LoopAlg::muonContainer() {
  const xAOD::MuonContainer* muonContainerPtr = nullptr;
  if ( evtStore()->retrieve(muonContainerPtr,"Muons").isFailure() ) {
    ANA_MSG_WARNING("Muons unavailable!");
    return nullptr;
  }
  return muonContainerPtr;
}

const xAOD::TruthParticle* TRTF::LoopAlg::truthParticle(const xAOD::TrackParticle* track) {
  const xAOD::TruthParticle* truthParticle = nullptr;
  if ( TRTF::Acc::truthParticleLink.isAvailable(*track) ) {
    const auto truthLink = TRTF::Acc::truthParticleLink(*track);
    if ( truthLink.isValid() ) {
      truthParticle = *truthLink;
    }
  }
  return truthParticle;
}

bool TRTF::LoopAlg::triggerPassed(const std::string trigName) const {
  auto chainGroup = m_trigDecToolHandle->getChainGroup(trigName);
  auto passed = chainGroup->isPassed();
  return passed;
}

bool TRTF::LoopAlg::triggersPassed(const std::vector<std::string>& trigNames) const {
  for ( auto const& name : trigNames ) {
    auto chainGroup = m_trigDecToolHandle->getChainGroup(name);
    if ( chainGroup->isPassed() ) return true;
  }
  return false;
}

float TRTF::LoopAlg::eventWeight() const {
  if ( m_eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION) ) {
    auto weights = m_eventInfo->mcEventWeights();
    if ( !(weights.empty()) ) return weights.at(0);
    return 1.0;
  }
  return 1.0;
}

float TRTF::LoopAlg::averageMu() {
  if ( !(m_eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION)) && m_usePRWTool ) {
    return m_PRWToolHandle->getCorrectedAverageInteractionsPerCrossing(*m_eventInfo,true);
  }
  else {
    return m_eventInfo->averageInteractionsPerCrossing();
  }
}

TRTF::HitSummary TRTF::LoopAlg::getHitSummary(const xAOD::TrackParticle* track,
                                              const xAOD::TrackStateValidation* msos,
                                              const xAOD::TrackMeasurementValidation* driftCircle) {
  TRTF::HitSummary hit;
  hit.HTMB        = (get(TRTF::Acc::bitPattern, driftCircle,"bitPattern") & 131072) ? 1 : 0;
  hit.tot         =  get(TRTF::Acc::tot,        driftCircle,"tot");
  hit.gasType     =  get(TRTF::Acc::gasType,    driftCircle,"gasType");
  hit.bec         =  get(TRTF::Acc::bec,        driftCircle,"bec");
  hit.layer       =  get(TRTF::Acc::layer,      driftCircle,"layer");
  hit.strawlayer  =  get(TRTF::Acc::strawlayer, driftCircle,"strawlayer");
  hit.strawnumber =  get(TRTF::Acc::strawnumber,driftCircle,"strawnumber");
  hit.drifttime   =  get(TRTF::Acc::drifttime,  driftCircle,"drifttime");
  hit.T0          =  get(TRTF::Acc::T0,         driftCircle,"T0");

  hit.type       = get(TRTF::Acc::type,      msos,"type");
  hit.localX     = get(TRTF::Acc::localX,    msos,"localX");
  hit.localY     = get(TRTF::Acc::localY,    msos,"localY");
  hit.localTheta = get(TRTF::Acc::localTheta,msos,"localTheta");
  hit.localPhi   = get(TRTF::Acc::localPhi,  msos,"localPhi");
  hit.HitZ       = get(TRTF::Acc::HitZ,      msos,"HitZ");
  hit.HitR       = get(TRTF::Acc::HitR,      msos,"HitR");
  hit.rTrkWire   = get(TRTF::Acc::rTrkWire,  msos,"rTrkWire");

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

uint8_t TRTF::LoopAlg::nSilicon(const xAOD::TrackParticle* track) const {
  uint8_t nPix = -1;
  uint8_t nSCT = -1;
  if ( !track->summaryValue(nPix,xAOD::numberOfPixelHits) ) ANA_MSG_ERROR("No Pix hits?");
  if ( !track->summaryValue(nSCT,xAOD::numberOfSCTHits)   ) ANA_MSG_ERROR("No SCT hits?");
  return (nPix + nSCT);
}

uint8_t TRTF::LoopAlg::nSiliconHoles(const xAOD::TrackParticle* track) const {
  uint8_t nPixHole = -1;
  uint8_t nSCTHole = -1;
  if ( !track->summaryValue(nPixHole,xAOD::numberOfPixelHoles) ) ANA_MSG_ERROR("No Pix holes?");
  if ( !track->summaryValue(nSCTHole,xAOD::numberOfSCTHoles)   ) ANA_MSG_ERROR("No SCT holes?");
  return (nPixHole + nSCTHole);
}

uint8_t TRTF::LoopAlg::nSiliconShared(const xAOD::TrackParticle* track) const {
  uint8_t nSCTSh = -1;
  uint8_t nPixSh = -1;
  if ( !track->summaryValue(nPixSh,xAOD::numberOfPixelSharedHits) ) ANA_MSG_ERROR("No Pix shared?");
  if ( !track->summaryValue(nSCTSh,xAOD::numberOfSCTSharedHits)   ) ANA_MSG_ERROR("No SCT shared?");
  return (nPixSh + nSCTSh);
}

float TRTF::LoopAlg::deltaz0sinTheta(const xAOD::TrackParticle *track, const xAOD::Vertex* vtx) const {
  float delta_z0 = std::fabs(track->z0() + track->vz() - vtx->z());
  float dz0sinth = std::fabs(delta_z0*std::sin(track->theta()));
  return dz0sinth;
}

double TRTF::LoopAlg::d0signif(const xAOD::TrackParticle *track) const {
  double d0sig = xAOD::TrackingHelpers::d0significance(track,
                                                       m_eventInfo->beamPosSigmaX(),
                                                       m_eventInfo->beamPosSigmaY(),
                                                       m_eventInfo->beamPosSigmaXY());
  return d0sig;
}
