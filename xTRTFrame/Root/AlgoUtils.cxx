#include <xTRTFrame/Algo.h>

#include <AsgTools/StatusCode.h>
#include <xAODCore/AuxContainerBase.h>
#include <xAODTracking/Vertex.h>
#include <xAODTracking/TrackParticlexAODHelpers.h>

const xAOD::TrackParticleContainer* xTRT::Algo::trackContainer() {
  const xAOD::TrackParticleContainer* trackContainerPtr = nullptr;
  if ( evtStore()->retrieve(trackContainerPtr,"InDetTrackParticles").isFailure() ) {
    ANA_MSG_ERROR("InDetTrackParticles unavailable!");
    return nullptr;
  }
  return trackContainerPtr;
}

const xAOD::ElectronContainer* xTRT::Algo::electronContainer() {
  const xAOD::ElectronContainer* electronContainerPtr = nullptr;
  if ( evtStore()->retrieve(electronContainerPtr,"Electrons").isFailure() ) {
    ANA_MSG_ERROR("Electrons unavailable!");
    return nullptr;
  }
  return electronContainerPtr;
}

const xAOD::MuonContainer* xTRT::Algo::muonContainer() {
  const xAOD::MuonContainer* muonContainerPtr = nullptr;
  if ( evtStore()->retrieve(muonContainerPtr,"Muons").isFailure() ) {
    ANA_MSG_ERROR("Muons unavailable!");
    return nullptr;
  }
  return muonContainerPtr;
}

const xAOD::TrackParticleContainer* xTRT::Algo::selectedTracks() {
  return selectedContainer<xAOD::TrackParticleContainer,xAOD::TrackParticle>
    (trackContainer(),passTrackSelection,"GoodTracks");
}

const xAOD::ElectronContainer* xTRT::Algo::selectedElectrons() {
  return selectedContainer<xAOD::ElectronContainer,xAOD::Electron>
    (electronContainer(),passElectronSelection,"GoodElectrons");
}

const xAOD::MuonContainer* xTRT::Algo::selectedMuons() {
  return selectedContainer<xAOD::MuonContainer,xAOD::Muon>
    (muonContainer(),passMuonSelection,"GoodMuons");
}

const xAOD::TruthParticle* xTRT::Algo::getTruth(const xAOD::TrackParticle* track) {
  const xAOD::TruthParticle* truthParticle = nullptr;
  if ( xTRT::Acc::truthParticleLink.isAvailable(*track) ) {
    const auto truthLink = xTRT::Acc::truthParticleLink(*track);
    if ( truthLink.isValid() ) {
      truthParticle = *truthLink;
    }
  }
  return truthParticle;
}

const xAOD::TrackParticle* xTRT::Algo::getTrack(const xAOD::Electron* electron) {
  const xAOD::TrackParticle *track = xAOD::EgammaHelpers::getOriginalTrackParticle(electron);
  if ( not track ) {
    XTRT_FATAL("No original track particle from electron");
    return nullptr;
  }
  return track;
}

const xAOD::TrackParticle* xTRT::Algo::getTrack(const xAOD::Muon* muon) {
  auto idtl = muon->inDetTrackParticleLink();
  if ( not idtl.isValid() ) {
    XTRT_FATAL("No valid muon->inDetTrackParticleLink()");
    return nullptr;
  }
  auto trk = *idtl;
  if ( not trk ) {
    XTRT_FATAL("Track is null after muon track link dereference");
    return nullptr;
  }
  return trk;
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
  if ( !(m_eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION)) && config()->usePRW() ) {
    return m_PRWToolHandle->getCorrectedAverageInteractionsPerCrossing(*m_eventInfo,true);
  }
  else {
    return m_eventInfo->averageInteractionsPerCrossing();
  }
}

std::size_t xTRT::Algo::NPV() const {
  const xAOD::VertexContainer* verts = nullptr;
  if ( evtStore()->retrieve(verts,"PrimaryVertices").isFailure() ) {
    ANA_MSG_WARNING("Cannot retrieve PrimaryVertices, returning 0");
    return 0;
  }
  return verts->size();
}

bool xTRT::Algo::passGRL() const {
  if ( m_eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION) || !config()->useGRL() ) {
    return true;
  }
  return m_GRLToolHandle->passRunLB(*m_eventInfo);
}

int xTRT::Algo::nTRT(const xAOD::TrackParticle* track) {
  uint8_t nTRThits = -1;
  uint8_t nTRTouts = -1;
  if ( !track->summaryValue(nTRThits,xAOD::numberOfTRTHits)     ) XTRT_FATAL("No TRT hits");
  if ( !track->summaryValue(nTRTouts,xAOD::numberOfTRTOutliers) ) XTRT_FATAL("No TRT outliers");
  return ((int)nTRThits + (int)nTRTouts);
}

int xTRT::Algo::nTRT_PrecTube(const xAOD::TrackParticle* track) {
  uint8_t nTRThits = -1;
  if ( !track->summaryValue(nTRThits,xAOD::numberOfTRTHits) ) XTRT_FATAL("No TRT hits");
  return ((int)nTRThits);
}

int xTRT::Algo::nTRT_Outlier(const xAOD::TrackParticle* track) {
  uint8_t nTRTouts = -1;
  if ( !track->summaryValue(nTRTouts,xAOD::numberOfTRTOutliers) ) XTRT_FATAL("No TRT outliers");
  return ((int)nTRTouts);
}

int xTRT::Algo::nSilicon(const xAOD::TrackParticle* track) {
  uint8_t nPix = -1;
  uint8_t nSCT = -1;
  if ( !track->summaryValue(nPix,xAOD::numberOfPixelHits) ) XTRT_FATAL("No Pix hits?");
  if ( !track->summaryValue(nSCT,xAOD::numberOfSCTHits)   ) XTRT_FATAL("No SCT hits?");
  return ((int)nPix + (int)nSCT);
}

int xTRT::Algo::nSiliconHoles(const xAOD::TrackParticle* track) {
  uint8_t nPixHole = -1;
  uint8_t nSCTHole = -1;
  if ( !track->summaryValue(nPixHole,xAOD::numberOfPixelHoles) ) XTRT_FATAL("No Pix holes?");
  if ( !track->summaryValue(nSCTHole,xAOD::numberOfSCTHoles)   ) XTRT_FATAL("No SCT holes?");
  return ((int)nPixHole + (int)nSCTHole);
}

int xTRT::Algo::nSiliconShared(const xAOD::TrackParticle* track) {
  uint8_t nSCTSh = -1;
  uint8_t nPixSh = -1;
  if ( !track->summaryValue(nPixSh,xAOD::numberOfPixelSharedHits) ) XTRT_FATAL("No Pix shared?");
  if ( !track->summaryValue(nSCTSh,xAOD::numberOfSCTSharedHits)   ) XTRT_FATAL("No SCT shared?");
  return ((int)nSCTSh + (int)nPixSh);
}

float xTRT::Algo::deltaz0sinTheta(const xAOD::TrackParticle *track, const xAOD::Vertex* vtx) {
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

bool xTRT::Algo::passTrackSelection(const xAOD::TrackParticle* track, const xTRT::Config* conf) {
  if ( xTRT::Algo::nTRT(track) < conf->track_nTRT() ) return false;
  if ( xTRT::Algo::nTRT_PrecTube(track) < conf->track_nTRTprec() ) return false;
  if ( track->pt() < conf->track_pT() ) return false;
  if ( std::abs(track->eta()) > conf->track_eta() ) return false;
  if ( track->p4().P() < conf->track_p() ) return false;
  if ( xTRT::Algo::nSilicon(track) < conf->track_nSi() ) return false;
  return true;
}

bool xTRT::Algo::passElectronSelection(const xAOD::Electron* electron, const xTRT::Config* conf) {
  auto trk = xAOD::EgammaHelpers::getOriginalTrackParticle(electron);
  if ( conf->elec_UTC() ) {
    if ( not trk ) return false;
    if ( not passTrackSelection(trk,conf) ) return false;
  }
  if ( conf->elec_relpT() > 0 ) {
    if ( not trk ) {
      XTRT_FATAL("No track particle from electron");
      return false;
    }
    if ( trk->pt() < conf->elec_relpT()*electron->pt() ) return false;
  }
  if ( electron->pt() < conf->elec_pT() ) return false;
  if ( electron->p4().P() < conf->elec_p() ) return false;
  if ( std::abs(electron->eta()) > conf->elec_eta() ) return false;
  return true;
}

bool xTRT::Algo::passMuonSelection(const xAOD::Muon* muon, const xTRT::Config* conf) {
  auto idtl = muon->inDetTrackParticleLink();
  if ( not idtl.isValid() ) return false;
  auto trk = *idtl;
  if ( not trk ) {
    XTRT_FATAL("No valid muon->inDetTrackParticleLink()");
    return false;
  }
  if ( conf->muon_UTC() && (not passTrackSelection(trk,conf)) ) {
    return false;
  }
  if ( conf->muon_relpT() > 0 && (trk->pt() < conf->muon_relpT()*muon->pt()) ) {
    return false;
  }
  if ( muon->pt() < conf->muon_pT() ) return false;
  if ( muon->p4().P() < conf->muon_p() ) return false;
  if ( std::abs(muon->eta()) > conf->muon_eta() ) return false;
  return true;
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
