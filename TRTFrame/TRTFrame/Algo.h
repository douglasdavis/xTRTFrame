#ifndef TRTFrame_Algo_h
#define TRTFrame_Algo_h

// C++
#include <memory>
#include <vector>

// ATLAS
#include <EventLoop/Algorithm.h>
#include <EventLoop/Worker.h>
#include <AsgTools/AnaToolHandle.h>
#include <AsgAnalysisInterfaces/IGoodRunsListSelectionTool.h>
#include <AsgAnalysisInterfaces/IPileupReweightingTool.h>
#include <TrigConfxAOD/xAODConfigTool.h>
#include <TrigDecisionTool/TrigDecisionTool.h>
#include <TriggerMatchingTool/MatchingTool.h>
#include <PathResolver/PathResolver.h>
#include <InDetTrackSelectionTool/InDetTrackSelectionTool.h>

#include <xAODTracking/TrackParticle.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODCore/ShallowCopy.h>
#include <xAODMuon/MuonContainer.h>
#include <xAODMuon/MuonAuxContainer.h>
#include <xAODEgamma/EgammaxAODHelpers.h>
#include <xAODEgamma/ElectronContainer.h>
#include <xAODEgamma/ElectronAuxContainer.h>
#include <xAODTruth/TruthParticleAuxContainer.h>
#include <xAODTruth/TruthVertex.h>
#include <xAODTruth/xAODTruthHelpers.h>
#include <xAODTracking/VertexContainer.h>
#include <xAODTracking/TrackMeasurementValidationContainer.h>
#include <xAODTracking/TrackStateValidationContainer.h>
#include <xAODTracking/TrackParticlexAODHelpers.h>

// TRTFrame
#include <TRTFrame/Accessors.h>
#include <TRTFrame/HitSummary.h>
#include <TRTFrame/ParticleId.h>
#include <TRTFrame/Config.h>

// ROOT
#include <TH1F.h>
#include <TTree.h>

namespace xTRT {

  class Algo : public EL::Algorithm {

  protected:

    std::unique_ptr<xTRT::Config> m_config;

    bool m_usePRWTool;
    bool m_useTrigTools;

    std::vector<std::string> m_JPsi_electronTriggers;
    std::vector<std::string> m_JPsi_muonTriggers;

    std::vector<std::string> m_PRWConfFiles;     //!
    std::vector<std::string> m_PRWLumiCalcFiles; //!

    asg::AnaToolHandle<IGoodRunsListSelectionTool> m_GRLToolHandle{""};          //!
    asg::AnaToolHandle<CP::IPileupReweightingTool> m_PRWToolHandle{""};          //!
    asg::AnaToolHandle<TrigConf::ITrigConfigTool>  m_trigConfToolHandle{""};     //!
    asg::AnaToolHandle<Trig::TrigDecisionTool>     m_trigDecToolHandle{""};      //!
    asg::AnaToolHandle<Trig::IMatchingTool>        m_trigMatchingToolHandle{""}; //!

    asg::AnaToolHandle<InDet::IInDetTrackSelectionTool> m_trackSelToolHandle{""};     //!
    asg::AnaToolHandle<InDet::IInDetTrackSelectionTool> m_trackElecSelToolHandle{""}; //!
    asg::AnaToolHandle<InDet::IInDetTrackSelectionTool> m_trackMuonSelToolHandle{""}; //!

    int m_eventCounter; //!

    const xAOD::EventInfo* m_eventInfo; //!
    xAOD::TEvent* m_event; //!
    xAOD::TStore* m_store; //!

    std::shared_ptr<xTRT::ParticleIdSvc> m_pidSvc; //!

  protected:

    std::string m_outputName;

  public:

    // this is a standard constructor
    Algo();
    virtual ~Algo();

    // functions to configure the algorithm
    void feedConfig(const std::string fileName) {
      m_config = std::make_unique<xTRT::Config>();
      m_config->parse(fileName);
    }
    xTRT::Config* config() { return m_config.get(); }

    void addPRWConfFile(const std::string& file)      { m_PRWConfFiles.push_back(file);     }
    void addPRWLumiCalcFile(const std::string& file)  { m_PRWLumiCalcFiles.push_back(file); }
    void usePRWTool(bool logic = true)                { m_usePRWTool   = logic; }
    void useTrigTools(bool logic = true)              { m_useTrigTools = logic; }
    void setTreeOutputName(const std::string name)    { m_outputName   = name;  }

    // these are the functions inherited from Algorithm
    virtual EL::StatusCode setupJob(EL::Job& job);
    virtual EL::StatusCode fileExecute();
    virtual EL::StatusCode histInitialize();
    virtual EL::StatusCode changeInput(bool firstFile);
    virtual EL::StatusCode initialize();
    virtual EL::StatusCode execute();
    virtual EL::StatusCode postExecute();
    virtual EL::StatusCode finalize();
    virtual EL::StatusCode histFinalize();

    EL::StatusCode setupTrackSelectionTools();
    EL::StatusCode enableGRLTool();
    EL::StatusCode enablePRWTool();
    EL::StatusCode enableTriggerTools();

    std::shared_ptr<xTRT::ParticleIdSvc> particleIdSvc() const { return m_pidSvc; }

    const xAOD::TrackParticleContainer* trackContainer();
    const xAOD::ElectronContainer*      electronContainer();
    const xAOD::MuonContainer*          muonContainer();

    const xAOD::TruthParticle* truthParticle(const xAOD::TrackParticle* track);

    float eventWeight() const;
    float averageMu();
    bool  passGRL() const;

    xAOD::TEvent* event() { return m_event; }
    xAOD::TStore* store() { return m_store; }
    const xAOD::EventInfo* eventInfo();

    xTRT::HitSummary getHitSummary(const xAOD::TrackParticle* track,
                                   const xAOD::TrackStateValidation* msos,
                                   const xAOD::TrackMeasurementValidation* driftCircle);

    // selection functions
    bool triggerPassed(const std::string trigName) const;
    bool triggersPassed(const std::vector<std::string>& trigNames) const;

    uint8_t nSilicon(const xAOD::TrackParticle* track) const;
    uint8_t nSiliconHoles(const xAOD::TrackParticle* track) const;
    uint8_t nSiliconShared(const xAOD::TrackParticle* track) const;
    float   deltaz0sinTheta(const xAOD::TrackParticle *track, const xAOD::Vertex* vtx) const;
    double  d0signif(const xAOD::TrackParticle *track) const;

    template <class T1, class T2>
    T1 get(const SG::AuxElement::ConstAccessor<T1>& acc, const T2* cont, std::string vn = "") const {
      if ( acc.isAvailable(*cont) ) {
        return acc(*cont);
      }
      ANA_MSG_WARNING("AuxElement: " << vn << " is not available! Returning 0!");
      return 0;
    }

    ClassDef(xTRT::Algo, 1);

  };

}

inline const xAOD::EventInfo* xTRT::Algo::eventInfo() {
  const xAOD::EventInfo* evtinfo = nullptr;
  if ( evtStore()->retrieve(evtinfo,"EventInfo").isFailure() ) {
    ANA_MSG_ERROR("Cannot retrieve EventInfo for some reason");
  }
  return evtinfo;
}

#endif
