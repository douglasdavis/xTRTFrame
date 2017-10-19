#include <xTRTFrame/TNPAlgorithm.h>

EL::StatusCode xTRT::TNPAlgorithm::performSelection() {
  clear();
  m_selectionCalled = true;

  // make sure a trigger fired
  bool trig1 = triggersPassed(config()->electronTriggers());
  bool trig2 = triggersPassed(config()->dielectronTriggers());
  if ( not (trig1 or trig2) ) {
    return EL::StatusCode::SUCCESS;
  }

  // setting up EDM object skeleton
  auto electrons = electronContainer();
  if ( electrons->size() < 2 ) return EL::StatusCode::SUCCESS;
  const xAOD::Electron*      Tag       = nullptr;
  const xAOD::Electron*      Probe     = nullptr;
  const xAOD::TrackParticle* Tag_trk   = nullptr;
  const xAOD::TrackParticle* Probe_trk = nullptr;

  idx_t finder; // for searching vectors

  // do the double loop
  for ( std::size_t itag = 0; itag < electrons->size(); ++itag ) {
    for ( std::size_t iprobe = 0; iprobe < electrons->size(); ++iprobe) {

      // never the same particle
      if ( itag == iprobe ) continue;

      // get objects
      Tag       = electrons->at(itag);
      Probe     = electrons->at(iprobe);
      Tag_trk   = getTrack(Tag);
      Probe_trk = getTrack(Probe);
      if ( Tag_trk   == nullptr ) continue;
      if ( Probe_trk == nullptr ) continue;

      // don't get repeated probes or try to use a already tagged tag as a probe
      finder = std::find(m_probeIndices.begin(),m_probeIndices.end(),iprobe);
      if ( finder != m_probeIndices.end() ) continue;
      finder = std::find(m_tagIndices.begin(),m_tagIndices.end(),iprobe);
      if ( finder != m_tagIndices.end() ) continue;

      // tag must be tight LH, probe must be loose non LH
      // both must pass author
      if ( not (passTightLH(Tag) and passLoose(Probe)) ) continue;
      if ( not (passAuthor(Tag) and passAuthor(Probe)) ) continue;

      // check pT and eta cuts
      if ( (Tag->pt()*toGeV) < m_tag_pT ) continue;
      if ( (Probe->pt()*toGeV) < m_probe_pT ) continue;
      if ( Probe_trk->pt() < (m_probe_relpT*(Probe->pt())) ) continue;
      if ( std::abs(Tag->eta())   > 2.0 ) continue;
      if ( std::abs(Probe->eta()) > 2.0 ) continue;

      // check some track number of hits cuts
      if ( nTRT(Tag_trk) < m_tag_nTRT ) continue;
      if ( nTRT(Probe_trk) < m_probe_nTRT ) continue;
      if ( nPixel(Tag_trk) < m_tag_nPix ) continue;
      if ( nPixel(Probe_trk) < m_probe_nPix ) continue;
      if ( nSilicon(Tag_trk) < m_tag_nSi ) continue;
      if ( nSilicon(Probe_trk) < m_probe_nSi ) continue;

      // check OS
      if ( (Tag->charge() * Probe->charge()) > 0 ) continue;

      // check inv mass cuts
      float invMass   = (Tag->p4()+Probe->p4()).M();
      bool  inZwindow = (invMass > 80*GeV) and (invMass < 100*GeV);
      if ( not inZwindow ) continue;

      // all passes - save em
      m_probeIndices.push_back(iprobe);
      m_tagIndices.push_back(itag);
      m_invMasses.push_back(invMass);

    }
  }

  // check to make sure indices are not shared
  for ( const auto& idx : m_probeIndices ) {
    finder = std::find(m_tagIndices.begin(),m_tagIndices.end(),idx);
    if ( finder != m_tagIndices.end() ) {
      ANA_MSG_ERROR("An electron is labeled a tag and a probe! PROBLEM! Index: " << idx);
      return EL::StatusCode::FAILURE;
    }
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode xTRT::TNPAlgorithm::makeContainers() {
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(performSelection());
  if ( m_containersMade ) {
    return EL::StatusCode::SUCCESS;
  }
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
  m_containersMade = true;
  return EL::StatusCode::SUCCESS;
}
