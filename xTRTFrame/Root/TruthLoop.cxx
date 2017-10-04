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
#include <xTRTFrame/TruthLoop.h>

// C++
#include <sstream>

ClassImp(xTRT::TruthLoop)

xTRT::TruthLoop::TruthLoop() : xTRT::Algorithm() {}

xTRT::TruthLoop::~TruthLoop() {}

EL::StatusCode xTRT::TruthLoop::histInitialize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::histInitialize());

  m_fillLeptonsOnly = config()->customOpt<bool>("LeptonsOnly");
  m_saveHits        = config()->customOpt<bool>("StoreHits");
  m_type0only       = config()->customOpt<bool>("Type0HitOnly");
  m_divZweightBy1k  = config()->customOpt<bool>("DivZweightBy1k");

  m_anaTracks = config()->customOpt<bool>("AnalyzeTracks");
  m_anaElecs  = config()->customOpt<bool>("AnalyzeElectrons");
  m_anaMuons  = config()->customOpt<bool>("AnalyzeMuons");

  create(TH1F("h_averageMu","",70,-0.5,69.5));

  TFile *outFile = wk()->getOutputFile(m_outputName);

  m_zdTree = new TTree("zdTree","zdTree");
  m_jdTree = new TTree("jdTree","jdTree");

  for ( TTree* itree : { m_zdTree, m_jdTree } ) {
    itree->SetDirectory(outFile);
    itree->Branch("avgmu",      &m_avgMu);
    itree->Branch("weight",     &m_weight);
    itree->Branch("pdgId",      &m_pdgId);
    itree->Branch("trkOcc",     &m_trkOcc);
    itree->Branch("truthMass",  &m_truthMass);
    itree->Branch("p",          &m_p);
    itree->Branch("pT",         &m_pT);
    itree->Branch("eta",        &m_eta);
    itree->Branch("phi",        &m_phi);
    itree->Branch("theta",      &m_theta);
    itree->Branch("eProbHT",    &m_eProbHT);
    itree->Branch("nTRThits",   &m_nTRThits);
    itree->Branch("nTRTouts",   &m_nTRTouts);
    itree->Branch("nArhits",    &m_nArhits);
    itree->Branch("nXehits",    &m_nXehits);
    itree->Branch("nHThitsMan", &m_nHThitsMan);
    itree->Branch("dEdxNoHT",   &m_dEdxNoHT);
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

EL::StatusCode xTRT::TruthLoop::initialize() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::initialize());

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::TruthLoop::execute() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::execute());

  if ( !passGRL() ) return EL::StatusCode::SUCCESS;

  m_weight = eventWeight();
  if ( m_divZweightBy1k ) m_weight = m_weight/1000.0;
  m_avgMu  = averageMu();
  grab<TH1F>("h_averageMu")->Fill(m_avgMu,m_weight);

  if ( m_anaTracks ) {
    auto tracks = selectedTracks();
    for ( const auto& track : *tracks ) {
      analyzeTrack(track);
    }
  }

  if ( m_anaElecs ) {
    auto electrons = selectedElectrons();
    for ( const auto& electron : *electrons ) {
      analyzeTrack(getTrack(electron));
    }
  }

  if ( m_anaMuons ) {
    auto muons = selectedMuons();
    for ( const auto& muon : *muons ) {
      analyzeTrack(getTrack(muon));
    }
  }

  return EL::StatusCode::SUCCESS;
}

void xTRT::TruthLoop::analyzeTrack(const xAOD::TrackParticle* track) {
  // check for truth particle and el or mu
  auto truth = getTruth(track);
  if ( truth == nullptr ) {
    ANA_MSG_DEBUG("truth is nullptr!");
    return;
  }
  m_pdgId = std::abs(truth->pdgId());
  bool el_or_mu = (m_pdgId == 11 || m_pdgId == 13);
  if ( !el_or_mu ) return;

  // check for Z or jpsi
  auto parent = truth->parent();
  if ( parent == nullptr ) return;
  bool is_z_or_jpsi = (parent->isZ() || (parent->pdgId() == 443));
  if ( !is_z_or_jpsi ) return;

  // check for vertex
  auto vtx = track->vertex();
  if ( vtx == nullptr ) return;

  // more track selection
  bool failTrkSel = false;
  if ( config()->useIDTS() ) {
    if      ( m_pdgId == 11 ) failTrkSel = !(trackSelElecToolHandle()->accept(*track,vtx));
    else if ( m_pdgId == 13 ) failTrkSel = !(trackSelMuonToolHandle()->accept(*track,vtx));
    else return;
  }
  if ( failTrkSel ) return;

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

  m_truthMass = truth->m();
  m_pT        = track->pt();
  m_p         = track->p4().P();
  m_eta       = track->eta();
  m_phi       = track->phi();
  m_theta     = track->theta();

  m_trkOcc    = get(xTRT::Acc::TRTTrackOccupancy,track,"TRTTrackOccupancy");
  //m_eProbToT  = get(xTRT::Acc::eProbabilityToT,track,"eProbabilityToT");
  m_eProbHT   = get(xTRT::Acc::eProbabilityHT,track,"eProbabilityHT");

  //m_dEdxNoHT  = get(xTRT::Acc::ToT_dEdx_noHT_divByL,track,"ToT_dEdx_noHT_divByL");
  //m_nHitsdEdx = get(xTRT::Acc::ToT_usedHits_noHT_divByL,track,"ToT_usedHits_noHT_divByL");

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

  if ( parent->isZ()          ) { m_zdTree->Fill(); }
  if ( parent->pdgId() == 443 ) { m_jdTree->Fill(); }

  clearVectors();
}

bool xTRT::TruthLoop::fillHitBasedVariables(const xAOD::TrackParticle* track,
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

void xTRT::TruthLoop::clearVectors() {
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
