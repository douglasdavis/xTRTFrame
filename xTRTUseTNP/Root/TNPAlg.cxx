// ATLAS
#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <AsgTools/MessageCheck.h>
#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>

// ROOT
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>

// xTRTFrame
#include <xTRTUseTNP/TNPAlg.h>

// C++
#include <sstream>

ClassImp(xTRT::TNPAlg)

xTRT::TNPAlg::TNPAlg() : xTRT::TNPAlgorithm() {}

xTRT::TNPAlg::~TNPAlg() {}

EL::StatusCode xTRT::TNPAlg::histInitialize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::TNPAlgorithm::histInitialize());

  m_saveHits        = config()->getOpt<bool>("StoreHits",false);
  m_type0only       = config()->getOpt<bool>("Type0HitOnly",false);

  create(TH1F("h_averageMu","",70,-0.5,69.5));
  create(TH1F("h_invMass","",30,75,105));

  TFile *outFile = wk()->getOutputFile(m_outputName);
  m_tree_el_probes   = new TTree("electrons_probes","electrons_probes");
  m_tree_el_tags     = new TTree("electrons_tags",  "electrons_tags");
  m_tree_mu          = new TTree("muons",           "muons");

  for ( TTree* itree : { m_tree_el_tags, m_tree_el_probes, m_tree_mu } ) {
    itree->SetDirectory(outFile);
    itree->Branch("avgmu",      &m_avgMu);
    itree->Branch("weight",     &m_weight);
    itree->Branch("trkOcc",     &m_trkOcc);
    itree->Branch("p",          &m_p);
    itree->Branch("pT",         &m_pT);
    itree->Branch("eta",        &m_eta);
    itree->Branch("phi",        &m_phi);
    itree->Branch("eProbHT",    &m_eProbHT);
    itree->Branch("nTRThits",   &m_nTRThits);
    itree->Branch("nTRTouts",   &m_nTRTouts);
    itree->Branch("nArhits",    &m_nArhits);
    itree->Branch("nXehits",    &m_nXehits);
    itree->Branch("nHThitsMan", &m_nHThitsMan);
    itree->Branch("NhitsdEdx",  &m_nHitsdEdx);
    itree->Branch("sumToT",     &m_sumToT);
    itree->Branch("sumL",       &m_sumL);
    itree->Branch("fAr",        &m_fAr);
    itree->Branch("fHTMB",      &m_fHTMB);

    if ( m_saveHits ) {
      itree->Branch("hit_HTMB",       &m_HTMB);
      itree->Branch("hit_gasType",    &m_gasType);
      itree->Branch("hit_bec",        &m_bec);
      itree->Branch("hit_layer",      &m_layer);
      itree->Branch("hit_strawlayer", &m_strawlayer);
      itree->Branch("hit_strawnumber",&m_strawnumber);
      itree->Branch("hit_drifttime",  &m_drifttime);
      itree->Branch("hit_tot",        &m_tot);
      itree->Branch("hit_T0",         &m_T0);
      itree->Branch("hit_localTheta", &m_localTheta);
      itree->Branch("hit_localPhi",   &m_localPhi);
      itree->Branch("hit_HitZ",       &m_HitZ);
      itree->Branch("hit_HitR",       &m_HitR);
      itree->Branch("hit_rTrkWire",   &m_rTrkWire);
      itree->Branch("hit_L",          &m_L);
    }
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::TNPAlg::initialize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::TNPAlgorithm::initialize());

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::TNPAlg::execute() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::TNPAlgorithm::execute());

  if ( !passGRL() ) return EL::StatusCode::SUCCESS;

  float w = eventWeight();

  m_avgMu  = averageMu();
  grab<TH1F>("h_averageMu")->Fill(m_avgMu,w);

  auto probes = probeElectrons();
  auto tags   = tagElectrons();

  ANA_MSG_INFO("N probes: " << probes->size());
  ANA_MSG_INFO("N tags:   " << tags->size());

  for ( const float m : invMasses() ) {
    grab<TH1F>("h_invMass")->Fill(m*toGeV,w);
  }

  return EL::StatusCode::SUCCESS;
}

void xTRT::TNPAlg::analyzeTrack(const xAOD::TrackParticle* track, const bool is_el, const bool is_tag) {
  // check for vertex
  auto vtx = track->vertex();
  if ( vtx == nullptr ) return;

  uint8_t ntrthits = -1;
  uint8_t nxehits  = -1;
  uint8_t nouthits = -1;
  track->summaryValue(ntrthits,xAOD::numberOfTRTHits); // type 0 only!
  track->summaryValue(nxehits, xAOD::numberOfTRTXenonHits);
  track->summaryValue(nouthits,xAOD::numberOfTRTOutliers);

  m_nTRThits = ntrthits; // type 0 only
  m_nTRTouts = nouthits;
  m_nXehits  = nxehits;  // all xenon hits
  m_nArhits  = ntrthits + nouthits -nxehits; // all argon hits
  m_nHThitsMan  = 0;
  m_sumL        = 0.0;
  m_sumToT      = 0.0;

  m_pT        = track->pt();
  m_p         = track->p4().P();
  m_eta       = track->eta();
  m_phi       = track->phi();
  m_theta     = track->theta();

  m_trkOcc    = get(xTRT::Acc::TRTTrackOccupancy,track,"TRTTrackOccupancy");
  m_eProbHT   = get(xTRT::Acc::eProbabilityHT,track,"eProbabilityHT");

  const xAOD::TrackStateValidation* msos = nullptr;
  const xAOD::TrackMeasurementValidation* driftCircle = nullptr;
  if ( xTRT::Acc::msosLink.isAvailable(*track) ) {
    for ( auto trackMeasurement : xTRT::Acc::msosLink(*track) ) {
      if ( trackMeasurement.isValid() ) {
        msos = *trackMeasurement;
        if ( msos->detType() != 3 ) continue; // TRT hits only.
        if ( !(msos->trackMeasurementValidationLink().isValid()) ) continue;
        driftCircle = *(msos->trackMeasurementValidationLink());
        if ( !fillHitBasedVariables(track,msos,driftCircle,m_type0only) ) continue;
      }
    }
  }
  else {
    ANA_MSG_WARNING("msosLink was not available!");
  }

  m_fHTMB = (float)m_nHThitsMan / (m_nTRThits+m_nTRTouts);
  m_fAr   = (float)m_nArhits / (m_nTRThits+m_nTRTouts);

  if      ( is_el and  is_tag ) m_tree_el_tags->Fill();
  else if ( is_el and !is_tag ) m_tree_el_probes->Fill();
  else if ( not is_el         ) m_tree_mu->Fill();
  else {
    XTRT_FATAL("Not tag/probe el or just a mu?");
  }

  clearVectors();
}

bool xTRT::TNPAlg::fillHitBasedVariables(const xAOD::TrackParticle* track,
                                          const xAOD::TrackStateValidation* msos,
                                          const xAOD::TrackMeasurementValidation* driftCircle,
                                          const bool type0only) {
  auto hit = getHitSummary(track,msos,driftCircle);
  if ( type0only && (hit.type != 0) ) return false;
  m_type.push_back(hit.type);
  m_HTMB.push_back(hit.HTMB);
  if ( hit.HTMB ) m_nHThitsMan++;
  m_gasType.push_back(hit.gasType);
  m_bec.push_back(hit.bec);
  m_layer.push_back(hit.layer);
  m_strawlayer.push_back(hit.strawlayer);
  m_strawnumber.push_back(hit.strawnumber);
  m_drifttime.push_back(hit.drifttime);
  m_tot.push_back(hit.tot);
  m_T0.push_back(hit.T0);

  m_localTheta.push_back(hit.localTheta);
  m_localPhi.push_back(hit.localPhi);
  m_HitZ.push_back(hit.HitZ);
  m_HitR.push_back(hit.HitR);
  m_rTrkWire.push_back(hit.rTrkWire);
  m_L.push_back(hit.L);

  if ( hit.tot > 0.005 && hit.L > 0.00005 ) {
    m_sumL += hit.L;
    m_sumToT += hit.tot;
  }
  return true;
}

void xTRT::TNPAlg::clearVectors() {
  m_HTMB       .clear();
  m_gasType    .clear();
  m_bec        .clear();
  m_layer      .clear();
  m_strawlayer .clear();
  m_strawnumber.clear();
  m_drifttime  .clear();
  m_tot        .clear();
  m_T0         .clear();
  m_type       .clear();
  m_localTheta .clear();
  m_localPhi   .clear();
  m_HitZ       .clear();
  m_HitR       .clear();
  m_rTrkWire   .clear();
  m_L          .clear();
}
