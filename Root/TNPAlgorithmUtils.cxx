#include <xTRTFrame/TNPAlgorithm.h>

EL::StatusCode xTRT::TNPAlgorithm::performSelections() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  clear();
  ANA_CHECK(performZeeSelection());
  ANA_CHECK(performZmumuSelection());
  m_selectionCalled = true;
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::TNPAlgorithm::performZeeSelection() {
  // setting up EDM object skeleton
  auto electrons = electronContainer();
  if ( electrons->size() < 2 ) return EL::StatusCode::SUCCESS;

  // make sure a trigger fired
  bool trig1 = triggersPassed(config()->electronTriggers());
  bool trig2 = triggersPassed(config()->dielectronTriggers());
  if ( not (trig1 or trig2) ) {
    return EL::StatusCode::SUCCESS;
  }

  const xAOD::Electron* Tag   = nullptr;
  const xAOD::Electron* Probe = nullptr;
  idx_t finder; // for searching vectors

  // do the double loop
  for ( std::size_t itag = 0; itag < electrons->size(); ++itag ) {
    for ( std::size_t iprobe = 0; iprobe < electrons->size(); ++iprobe) {

      // never the same particle
      if ( itag == iprobe ) continue;

      // don't get repeated probes
      finder = std::find(m_probeIndices.begin(),m_probeIndices.end(),iprobe);
      if ( finder != m_probeIndices.end() ) continue;

      // get objects
      Tag   = electrons->at(itag);
      Probe = electrons->at(iprobe);

      if ( not passZeeTNP(Tag,Probe) ) continue;

      // all passes - save em
      float invMass = (Tag->p4()+Probe->p4()).M();
      m_probeIndices.push_back(iprobe);
      m_tagIndices.push_back(itag);
      m_invMassesEl.push_back(invMass);

    }
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::TNPAlgorithm::performZmumuSelection() {
  auto muons = muonContainer();
  if ( muons->size() < 2 ) return EL::StatusCode::SUCCESS;

  bool trig1 = triggersPassed(config()->muonTriggers());
  //bool trig2 = triggersPassed(config()->dimuonTriggers());
  if ( not trig1 ) {
    return EL::StatusCode::SUCCESS;
  }

  const xAOD::Muon* mu1 = nullptr;
  const xAOD::Muon* mu2 = nullptr;
  idx_t finder;

  for ( std::size_t i = 0; i < muons->size(); ++i ) {
    for ( std::size_t j = 0; j < muons->size(); ++j ) {

      if ( i == j ) continue;

      finder = std::find(m_muonIndices.begin(),m_muonIndices.end(),i);
      if ( finder != m_muonIndices.end() ) continue;

      mu1 = muons->at(i);
      mu2 = muons->at(j);

      if ( not passZmumuTNP(mu1,mu2) ) continue;

      float invMass = (mu1->p4() + mu2->p4()).M();
      m_muonIndices.push_back(i);
      m_invMassesMu.push_back(invMass);

    }
  }

  return EL::StatusCode::SUCCESS;
}

bool xTRT::TNPAlgorithm::passZeeTNP(const xAOD::Electron* Tag, const xAOD::Electron* Probe) {
  // check if tag matches to single electron trigger
  if ( not singleElectronTrigMatched(Tag) ) return false;
  auto Tag_trk   = getTrack(Tag);
  auto Probe_trk = getTrack(Probe);
  if ( not debugnullptr(Tag_trk,  "Tag_trk")   ) return false;
  if ( not debugnullptr(Probe_trk,"Probe_trk") ) return false;

  float tag_pT   = Tag->pt();
  float probe_pT = Probe->pt();

  // tag must be tight LH, probe must be loose non LH
  // both must pass author
  if ( not (passTightLH(Tag) and passLoose(Probe)) ) return false;
  if ( not (passAuthor(Tag) and passAuthor(Probe)) ) return false;

  // check kinematic (p, pT, eta,...) cuts
  if ( (tag_pT*toGeV) < m_tag_pT ) return false;
  if ( (probe_pT*toGeV) < m_probe_pT ) return false;
  if ( Probe_trk->pt() < (m_probe_relpT*probe_pT) ) return false;
  if ( std::abs(Tag->eta())   > 2.0 ) return false;
  if ( std::abs(Probe->eta()) > 2.0 ) return false;
  if ( (Tag->p4().P())*toGeV > m_tag_maxP ) return false;
  if ( (Probe->p4().P())*toGeV > m_probe_maxP ) return false;

  // check some track number of hits cuts
  if ( nTRT(Tag_trk) < m_tag_nTRT ) return false;
  if ( nTRT(Probe_trk) < m_probe_nTRT ) return false;
  if ( nPixel(Tag_trk) < m_tag_nPix ) return false;
  if ( nPixel(Probe_trk) < m_probe_nPix ) return false;
  if ( nSilicon(Tag_trk) < m_tag_nSi ) return false;
  if ( nSilicon(Probe_trk) < m_probe_nSi ) return false;

  // check iso cuts
  if ( caloIso(Tag) > (m_tag_iso_topoetcone20*tag_pT) ) return false;
  if ( trackIso(Tag) > (m_tag_iso_ptcone20*tag_pT) ) return false;

  // check OS
  if ( (Tag->charge() * Probe->charge()) > 0 ) return false;

  // check inv mass cuts
  float invMass   = (Tag->p4()+Probe->p4()).M();
  bool  inZwindow = (invMass > 80*GeV) and (invMass < 100*GeV);
  if ( not inZwindow ) return false;

  return true;
}

bool xTRT::TNPAlgorithm::passZmumuTNP(const xAOD::Muon* mu1, const xAOD::Muon* mu2) {
  // check for at least 1 single muon trig match
  if ( not ( singleMuonTrigMatched(mu1) || singleMuonTrigMatched(mu2) ) ) return false;

  auto mu1_trk = getTrack(mu1);
  auto mu2_trk = getTrack(mu2);
  if ( not debugnullptr(mu1_trk,"mu1_trk") ) return false;
  if ( not debugnullptr(mu2_trk,"mu2_trk") ) return false;

  float mu1_pT = mu1->pt();
  float mu2_pT = mu2->pt();

  // kinematics
  if ( mu1->p4().P()*toGeV > m_muon_maxP ) return false;
  if ( mu2->p4().P()*toGeV > m_muon_maxP ) return false;
  if ( mu1_pT*toGeV < m_muon_pT ) return false;
  if ( mu2_pT*toGeV < m_muon_pT ) return false;
  if ( std::abs(mu1->eta()) > 2.0 ) return false;
  if ( std::abs(mu2->eta()) > 2.0 ) return false;

  // hits
  if ( nTRT(mu1_trk) < m_muon_nTRT ) return false;
  if ( nTRT(mu2_trk) < m_muon_nTRT ) return false;
  if ( nPixel(mu1_trk) < m_muon_nPix ) return false;
  if ( nPixel(mu2_trk) < m_muon_nPix ) return false;
  if ( nSilicon(mu1_trk)  < m_muon_nSi  ) return false;
  if ( nSilicon(mu2_trk)  < m_muon_nSi  ) return false;

  // quality
  if ( not passQuality(mu1,m_muon_nPrec) || not passQuality(mu2,m_muon_nPrec) ) return false;

  // iso
  if ( caloIso(mu1) > (m_muon_iso_topoetcone20*mu1_pT) ) return false;
  if ( caloIso(mu2) > (m_muon_iso_topoetcone20*mu2_pT) ) return false;
  if ( trackIso(mu1) > (m_muon_iso_ptvarcone30*mu1_pT) ) return false;
  if ( trackIso(mu2) > (m_muon_iso_ptvarcone30*mu2_pT) ) return false;

  // check OS
  if ( (mu1->charge() * mu2->charge()) > 0 ) return false;

  float invMass = (mu1->p4()+mu2->p4()).M();
  bool  inZwindow = (invMass > 80*GeV) and (invMass < 100*GeV);
  if ( not inZwindow ) return false;

  return true;
}

EL::StatusCode xTRT::TNPAlgorithm::makeContainers() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  if ( m_containersMade ) {
    return EL::StatusCode::SUCCESS;
  }
  ANA_CHECK(performSelections());
  auto electrons      = electronContainer();
  auto cont_tags      = std::make_unique<xAOD::ElectronContainer>();
  auto cont_tagsAux   = std::make_unique<xAOD::AuxContainerBase>();
  auto cont_probes    = std::make_unique<xAOD::ElectronContainer>();
  auto cont_probesAux = std::make_unique<xAOD::AuxContainerBase>();
  cont_tags->setStore(cont_tagsAux.get());
  cont_probes->setStore(cont_probesAux.get());
  for ( const auto idx : m_tagIndices ) {
    auto atag = new xAOD::Electron();
    cont_tags->push_back(atag);
    *atag = *(electrons->at(idx));
  }
  for ( const auto idx : m_probeIndices ) {
    auto aprobe = new xAOD::Electron();
    cont_probes->push_back(aprobe);
    *aprobe = *(electrons->at(idx));
  }
  if ( evtStore()->record(cont_tags.release(),"TNPTagElectrons").isFailure() ) {
    ANA_MSG_ERROR("Couldn't record TNPTagElectrons");
    return EL::StatusCode::FAILURE;
  }
  if ( evtStore()->record(cont_tagsAux.release(),"TNPTagElectronsAux.").isFailure() ) {
    ANA_MSG_ERROR("Couldn't record TNPTagElectronsAux.");
    return EL::StatusCode::FAILURE;
  }
  if ( evtStore()->record(cont_probes.release(),"TNPProbeElectrons").isFailure() ) {
    ANA_MSG_ERROR("Couldn't record TNPProbeElectrons");
    return EL::StatusCode::FAILURE;
  }
  if ( evtStore()->record(cont_probesAux.release(),"TNPProbeElectronsAux.").isFailure() ) {
    ANA_MSG_ERROR("Couldn't record TNPProbeElectronsAux.");
    return EL::StatusCode::FAILURE;
  }

  auto muons       = muonContainer();
  auto cont_mus    = std::make_unique<xAOD::MuonContainer>();
  auto cont_musAux = std::make_unique<xAOD::AuxContainerBase>();
  cont_mus->setStore(cont_musAux.get());
  for ( const auto idx : m_muonIndices ) {
    auto amu = new xAOD::Muon();
    cont_mus->push_back(amu);
    *amu = *(muons->at(idx));
  }
  if ( evtStore()->record(cont_mus.release(),"TNPMuons").isFailure() ) {
    ANA_MSG_ERROR("Couldn't record TNPMuons");
    return EL::StatusCode::FAILURE;
  }
  if ( evtStore()->record(cont_musAux.release(),"TNPMuonsAux.").isFailure() ) {
    ANA_MSG_ERROR("Couldn't record TNPMuonsAux.");
    return EL::StatusCode::FAILURE;
  }

  m_containersMade = true;
  return EL::StatusCode::SUCCESS;
}
