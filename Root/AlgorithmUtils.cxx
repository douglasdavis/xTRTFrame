#include <xTRTFrame/Algorithm.h>

#include <AsgTools/StatusCode.h>
#include <xAODCore/AuxContainerBase.h>
#include <xAODTracking/Vertex.h>
#include <xAODTracking/TrackParticlexAODHelpers.h>

const xAOD::TrackParticleContainer* xTRT::Algorithm::trackContainer() {
  const xAOD::TrackParticleContainer* trackContainerPtr = nullptr;
  if ( evtStore()->retrieve(trackContainerPtr,"InDetTrackParticles").isFailure() ) {
    ANA_MSG_ERROR("InDetTrackParticles unavailable!");
    return nullptr;
  }
  return trackContainerPtr;
}

const xAOD::ElectronContainer* xTRT::Algorithm::electronContainer() {
  const xAOD::ElectronContainer* electronContainerPtr = nullptr;
  if ( evtStore()->retrieve(electronContainerPtr,"Electrons").isFailure() ) {
    ANA_MSG_ERROR("Electrons unavailable!");
    return nullptr;
  }
  return electronContainerPtr;
}

const xAOD::MuonContainer* xTRT::Algorithm::muonContainer() {
  const xAOD::MuonContainer* muonContainerPtr = nullptr;
  if ( evtStore()->retrieve(muonContainerPtr,"Muons").isFailure() ) {
    ANA_MSG_ERROR("Muons unavailable!");
    return nullptr;
  }
  return muonContainerPtr;
}

const xAOD::TrackParticleContainer* xTRT::Algorithm::selectedTracks() {
  return selectedContainer<xAOD::TrackParticleContainer,xAOD::TrackParticle>
    (trackContainer(),passTrackSelection,"xTRT_GoodTracks");
}

const xAOD::ElectronContainer* xTRT::Algorithm::selectedElectrons() {
  return selectedContainer<xAOD::ElectronContainer,xAOD::Electron>
    (electronContainer(),passElectronSelection,"xTRT_GoodElectrons");
}

const xAOD::MuonContainer* xTRT::Algorithm::selectedMuons() {
  return selectedContainer<xAOD::MuonContainer,xAOD::Muon>
    (muonContainer(),passMuonSelection,"xTRT_GoodMuons");
}

const xAOD::TruthParticle* xTRT::Algorithm::getTruth(const xAOD::TrackParticle* track) {
  const xAOD::TruthParticle* truthParticle = nullptr;
  if ( xTRT::Acc::truthParticleLink.isAvailable(*track) ) {
    const auto truthLink = xTRT::Acc::truthParticleLink(*track);
    if ( truthLink.isValid() ) {
      truthParticle = *truthLink;
    }
    else {
      return nullptr;
    }
  }
  else {
    return nullptr;
  }
  return truthParticle;
}

const xAOD::TrackParticle* xTRT::Algorithm::getTrack(const xAOD::Electron* electron) {
  const xAOD::TrackParticle* track = xAOD::EgammaHelpers::getOriginalTrackParticle(electron);
  if ( not track )  return nullptr;
  return track;
}

const xAOD::TrackParticle* xTRT::Algorithm::getGSFTrack(const xAOD::Electron* electron) {
  const xAOD::TrackParticle* track = electron->trackParticle();
  if ( not track ) return nullptr;
  return track;
}

const xAOD::TrackParticle* xTRT::Algorithm::getTrack(const xAOD::Muon* muon) {
  auto idtl = muon->inDetTrackParticleLink();
  if ( not idtl.isValid() ) return nullptr;
  auto trk = *idtl;
  if ( not trk )  return nullptr;
  return trk;
}

bool xTRT::Algorithm::triggerPassed(const std::string trigName) const {
  auto chainGroup = m_trigDecToolHandle->getChainGroup(trigName);
  auto passed = chainGroup->isPassed();
  return passed;
}

bool xTRT::Algorithm::triggersPassed(const std::vector<std::string>& trigNames) const {
  for ( const auto& name : trigNames ) {
    auto chainGroup = m_trigDecToolHandle->getChainGroup(name);
    if ( chainGroup->isPassed() ) return true;
  }
  return false;
}

bool xTRT::Algorithm::singleElectronTrigMatched(const xAOD::Electron* electron) {
  if ( not config()->useTrig() ) {
    ANA_MSG_WARNING("Asking for trigger matching without trigger enabled? Ret false");
    return false;
  }
  for ( const std::string trigstr : config()->electronTriggers() ) {
    if ( m_trigMatchingToolHandle->match(*electron,trigstr) ) {
      return true;
    }
  }
  return false;
}

bool xTRT::Algorithm::singleMuonTrigMatched(const xAOD::Muon* muon) {
  if ( not config()->useTrig() ) {
    ANA_MSG_WARNING("Asking for trigger matching without trigger enabled? Ret false");
    return false;
  }
  for ( const std::string trigstr : config()->muonTriggers() ) {
    if ( m_trigMatchingToolHandle->match(*muon,trigstr) ) {
      return true;
    }
  }
  return false;
}

float xTRT::Algorithm::eventWeight() {
  const xAOD::EventInfo* evtinfo = eventInfo();
  if ( evtinfo->eventType(xAOD::EventInfo::IS_SIMULATION) ) {
    auto weights = evtinfo->mcEventWeights();
    if ( not weights.empty() ) return weights.at(0);
    return 1.0;
  }
  return 1.0;
}

float xTRT::Algorithm::averageMu() {
  const xAOD::EventInfo* evtinfo = eventInfo();
  if ( !(evtinfo->eventType(xAOD::EventInfo::IS_SIMULATION)) && config()->usePRW() ) {
    return m_PRWToolHandle->getCorrectedAverageInteractionsPerCrossing(*evtinfo,true);
  }
  else {
    return eventInfo()->averageInteractionsPerCrossing();
  }
}

std::size_t xTRT::Algorithm::NPV() const {
  const xAOD::VertexContainer* verts = nullptr;
  if ( evtStore()->retrieve(verts,"PrimaryVertices").isFailure() ) {
    ANA_MSG_WARNING("Cannot retrieve PrimaryVertices, returning 0");
    return 0;
  }
  return verts->size();
}

bool xTRT::Algorithm::passGRL() const {
  if ( isMC() || !config()->useGRL() ) {
    return true;
  }
  return m_GRLToolHandle->passRunLB(*eventInfo());
}

float xTRT::Algorithm::deltaz0sinTheta(const xAOD::TrackParticle *track, const xAOD::Vertex* vtx) {
  float delta_z0 = std::fabs(track->z0() + track->vz() - vtx->z());
  float dz0sinth = std::fabs(delta_z0*std::sin(track->theta()));
  return dz0sinth;
}

double xTRT::Algorithm::d0signif(const xAOD::TrackParticle *track, const xAOD::EventInfo* evtinfo) {
  double d0sig = xAOD::TrackingHelpers::d0significance(track,
                                                       evtinfo->beamPosSigmaX(),
                                                       evtinfo->beamPosSigmaY(),
                                                       evtinfo->beamPosSigmaXY());
  return d0sig;
}

bool xTRT::Algorithm::passTrackSelection(const xAOD::TrackParticle* track, const xTRT::Config* conf) {
  if ( xTRT::Algorithm::nTRT(track) < conf->track_nTRT() ) return false;
  if ( xTRT::Algorithm::nTRT_PrecTube(track) < conf->track_nTRTprec() ) return false;
  if ( track->pt() < conf->track_pT() ) return false;
  if ( std::abs(track->eta()) > conf->track_eta() ) return false;
  if ( track->p4().P() < conf->track_p() ) return false;
  if ( xTRT::Algorithm::nSilicon(track) < conf->track_nSi() ) return false;
  return true;
}

bool xTRT::Algorithm::passElectronSelection(const xAOD::Electron* electron, const xTRT::Config* conf) {
  auto trk = xAOD::EgammaHelpers::getOriginalTrackParticle(electron);

  if ( conf->elec_UTC() ) {
    if ( trk == nullptr ) {
      XTRT_WARNING("No track from electron! failing selection");
      return false;
    }
    else {
      if ( not passTrackSelection(trk,conf) ) return false;
    }
  }

  if ( conf->elec_relpT() > 0 ) {
    if ( trk == nullptr ) {
      XTRT_WARNING("No track particle from electron! relative pT cut failing");
      return false;
    }
    else {
      if ( trk->pt() < (conf->elec_relpT() * electron->pt()) ) return false;
    }
  }

  if ( electron->pt() < conf->elec_pT() ) return false;
  if ( electron->p4().P() < conf->elec_p() ) return false;
  if ( std::abs(electron->eta()) > conf->elec_eta() ) return false;

  if ( conf->elec_truthMatched() ) {
    if ( not truthMatched(electron) ) return false;
  }

  return true;
}

bool xTRT::Algorithm::passMuonSelection(const xAOD::Muon* muon, const xTRT::Config* conf) {
  auto idtl = muon->inDetTrackParticleLink();
  if ( not idtl.isValid() ) return false;
  auto trk = *idtl;

  if ( conf->muon_UTC() ) {
    if ( trk == nullptr ) {
      XTRT_WARNING("No valid muon->inDetTrackParticleLink()! auto failing track selection");
      return false;
    }
    else {
      if ( not passTrackSelection(trk,conf) ) return false;
    }
  }

  if ( conf->muon_relpT() > 0 ) {
    if ( trk == nullptr ) {
      XTRT_WARNING("No valid muon->inDetTrackParticleLink()! auto failing rel pT selection");
      return false;
    }
    else {
      if ( trk->pt() < (conf->muon_relpT() * muon->pt()) ) return false;
    }
  }

  if ( muon->pt() < conf->muon_pT() ) return false;
  if ( muon->p4().P() < conf->muon_p() ) return false;
  if ( std::abs(muon->eta()) > conf->muon_eta() ) return false;

  if ( conf->muon_truthMatched() ) {
    if ( not truthMatched(muon) ) return false;
  }

  return true;
}

xTRT::HitSummary xTRT::Algorithm::getHitSummary(const xAOD::TrackParticle* track,
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
