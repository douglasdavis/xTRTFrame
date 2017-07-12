// TRTLite
#include <TRTLite/LoopAlg.h>

// ATLAS
#include <xAODTracking/Vertex.h>
#include <xAODTracking/TrackParticlexAODHelpers.h>

// This file contains functions for performing object selection based on the following wiki:
// https://twiki.cern.ch/twiki/bin/view/AtlasProtected/TrackingCPMoriond2017#Tracking_CP_Pre_Recommendations

// This should be a set of backup functions.
// We should prefer to use the InDetTrackSelectionTool, which is set up.

bool TRTLite::LoopAlg::passStandardTRTTrkSel(const xAOD::TrackParticle* track) const {
  uint8_t nTRT = -1;
  if ( !track->summaryValue(nTRT,xAOD::numberOfTRTHits) ) ANA_MSG_ERROR("No TRT hits?") ;
  if ( nTRT < 10 ) return false;
  if ( std::fabs(track->eta()) > 2.0 ) return false;
  return true;
}

bool TRTLite::LoopAlg::passLooseChargedPionTrkSel(const xAOD::TrackParticle* track) const {
  if ( !passStandardTRTTrkSel(track) ) return false;

  if ( track->pt()             < 400 ) return false;
  if ( std::fabs(track->eta()) > 2.5 ) return false;

  if ( nSilicon(track)         < 7   ) return false;
  if ( nSiliconHoles(track)    > 2   ) return false;

  uint8_t nPixHole = -1;
  if ( !track->summaryValue(nPixHole,xAOD::numberOfPixelHoles) ) ANA_MSG_ERROR("No Pix holes?");
  if ( nPixHole > 1 ) return false;

  uint8_t nSCTSh = -1;
  uint8_t nPixSh = -1;
  if ( !track->summaryValue(nPixSh,xAOD::numberOfPixelSharedHits) ) ANA_MSG_ERROR("No Pix shared?");
  if ( !track->summaryValue(nSCTSh,xAOD::numberOfSCTSharedHits)   ) ANA_MSG_ERROR("No SCT shared?");
  if ( (nPixSh + nSCTSh/2.0) > 1 ) return false;

  return true;
}

bool TRTLite::LoopAlg::passLoosePrimaryChargedPionTrkSel(const xAOD::TrackParticle* track) const {
  if ( !passLooseChargedPionTrkSel(track) ) return false;
  if ( (nSilicon(track) >= 7 && nSiliconShared(track) == 0) || nSilicon(track) >= 10 ) return true;
  return false;
}

bool TRTLite::LoopAlg::passTightChargedPionTrkSel(const xAOD::TrackParticle* track) const {
  if ( !passLoosePrimaryChargedPionTrkSel(track) ) return false;
  float eta = std::fabs(track->eta());
  if ( eta <= 1.65 && nSilicon(track) < 9  ) return false;
  if ( eta >  1.65 && nSilicon(track) < 11 ) return false;
  uint8_t nPixHole = -1;
  if ( !track->summaryValue(nPixHole,xAOD::numberOfPixelHoles) ) ANA_MSG_ERROR("No Pix holes?");
  if ( nPixHole != 0 ) return false;

  //?NIBL + NB-layer >  0 (if both IBL hit and B-layer hit are expected)
  //?NIBL + NB-layer >= 0 (if either IBL hit or B-layer hit is not expected)
  return true;
}

bool TRTLite::LoopAlg::passLooseMuonTrkSel(const xAOD::TrackParticle* track) const {
  if ( !passStandardTRTTrkSel(track) ) return false;

  uint8_t nPix = -1;
  uint8_t nSCT = -1;
  if ( !track->summaryValue(nPix,xAOD::numberOfPixelHits) ) ANA_MSG_ERROR("No Pix hits?");
  if ( !track->summaryValue(nSCT,xAOD::numberOfSCTHits)   ) ANA_MSG_ERROR("No SCT hits?");
  if ( nPix < 1 ) return false;
  if ( nSCT < 5 ) return false;
  if ( nSiliconHoles(track) > 2 ) return false;
  float eta = std::fabs(track->eta());
  if ( eta > 0.1 && eta < 1.9 ) {
    uint8_t nTRT   = -1;
    uint8_t nTRTOL = -1;
    if ( !track->summaryValue(nTRTOL,xAOD::numberOfTRTHits)     ) ANA_MSG_ERROR("No TRT Hits?");
    if ( !track->summaryValue(nTRTOL,xAOD::numberOfTRTOutliers) ) ANA_MSG_ERROR("No TRT Outliers?");
    if ( (nTRT + nTRTOL) < 6 ) return false;
    if ( nTRTOL < (0.9*(nTRT+nTRTOL)) ) return false;
  }
  return true;
}

bool TRTLite::LoopAlg::passLooseElectronTrkSel(const xAOD::TrackParticle* track) const {
  if ( !passStandardTRTTrkSel(track) ) return false;

  if ( nSilicon(track) < 7 ) return false;
  uint8_t nPix = -1;
  if ( !track->summaryValue(nPix,xAOD::numberOfPixelHits) ) ANA_MSG_ERROR("No Pix hits?");
  if ( nPix < 1 ) return false;
  return true;
}

bool TRTLite::LoopAlg::passPionVtxSel(const xAOD::TrackParticle* track, const xAOD::Vertex* vtx) const {
  if ( deltaz0sinTheta(track,vtx) > 3.0 ) return false;
  return true;
}

bool TRTLite::LoopAlg::passElectronVtxSel(const xAOD::TrackParticle *track, const xAOD::Vertex* vtx) const {
  if ( deltaz0sinTheta(track,vtx) > 0.5 ) return false;
  if ( d0signif(track) > 5 ) return false;
  return true;
}

bool TRTLite::LoopAlg::passMuonVtxSel(const xAOD::TrackParticle *track, const xAOD::Vertex* vtx) const {
  if ( deltaz0sinTheta(track,vtx) > 0.5 ) return false;
  if ( d0signif(track) > 3 ) return false;
  return true;
}

uint8_t TRTLite::LoopAlg::nSilicon(const xAOD::TrackParticle* track) const {
  uint8_t nPix = -1;
  uint8_t nSCT = -1;
  if ( !track->summaryValue(nPix,xAOD::numberOfPixelHits) ) ANA_MSG_ERROR("No Pix hits?");
  if ( !track->summaryValue(nSCT,xAOD::numberOfSCTHits)   ) ANA_MSG_ERROR("No SCT hits?");
  return (nPix + nSCT);
}

uint8_t TRTLite::LoopAlg::nSiliconHoles(const xAOD::TrackParticle* track) const {
  uint8_t nPixHole = -1;
  uint8_t nSCTHole = -1;
  if ( !track->summaryValue(nPixHole,xAOD::numberOfPixelHoles) ) ANA_MSG_ERROR("No Pix holes?");
  if ( !track->summaryValue(nSCTHole,xAOD::numberOfSCTHoles)   ) ANA_MSG_ERROR("No SCT holes?");
  return (nPixHole + nSCTHole);
}

uint8_t TRTLite::LoopAlg::nSiliconShared(const xAOD::TrackParticle* track) const {
  uint8_t nSCTSh = -1;
  uint8_t nPixSh = -1;
  if ( !track->summaryValue(nPixSh,xAOD::numberOfPixelSharedHits) ) ANA_MSG_ERROR("No Pix shared?");
  if ( !track->summaryValue(nSCTSh,xAOD::numberOfSCTSharedHits)   ) ANA_MSG_ERROR("No SCT shared?");
  return (nPixSh + nSCTSh);
}

float TRTLite::LoopAlg::deltaz0sinTheta(const xAOD::TrackParticle *track, const xAOD::Vertex* vtx) const {
  float delta_z0 = std::fabs(track->z0() + track->vz() - vtx->z());
  float dz0sinth = std::fabs(delta_z0*std::sin(track->theta()));
  return dz0sinth;
}

double TRTLite::LoopAlg::d0signif(const xAOD::TrackParticle *track) const {
  double d0sig = xAOD::TrackingHelpers::d0significance(track,
                                                       m_eventInfo->beamPosSigmaX(),
                                                       m_eventInfo->beamPosSigmaY(),
                                                       m_eventInfo->beamPosSigmaXY());
  return d0sig;
}
