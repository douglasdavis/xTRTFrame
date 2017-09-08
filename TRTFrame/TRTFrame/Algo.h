#ifndef TRTFrame_Algo_h
#define TRTFrame_Algo_h

// C++
#include <memory>
#include <vector>
#include <map>
#include <functional>

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
#include <TTree.h>

namespace xTRT {

  class Algo : public EL::Algorithm {

  protected:

    asg::AnaToolHandle<IGoodRunsListSelectionTool> m_GRLToolHandle;          //!
    asg::AnaToolHandle<CP::IPileupReweightingTool> m_PRWToolHandle;          //!
    asg::AnaToolHandle<TrigConf::ITrigConfigTool>  m_trigConfToolHandle;     //!
    asg::AnaToolHandle<Trig::TrigDecisionTool>     m_trigDecToolHandle;      //!
    asg::AnaToolHandle<Trig::IMatchingTool>        m_trigMatchingToolHandle; //!

    asg::AnaToolHandle<InDet::IInDetTrackSelectionTool> m_trackSelToolHandle;     //!
    asg::AnaToolHandle<InDet::IInDetTrackSelectionTool> m_trackElecSelToolHandle; //!
    asg::AnaToolHandle<InDet::IInDetTrackSelectionTool> m_trackMuonSelToolHandle; //!

    xTRT::Config m_config;
    std::string m_outputName;

    std::map<std::string,TObject*> m_objStore; //!

    int m_eventCounter; //!

    const xAOD::EventInfo* m_eventInfo; //!
    xAOD::TEvent* m_event;              //!
    xAOD::TStore* m_store;              //!

    std::shared_ptr<xTRT::ParticleIdSvc> m_pidSvc; //!

  public:

    Algo();
    virtual ~Algo();

    /// Creates a ROOT object to be stored.
    /**
     *  This allows easy creation by just calling the constructor of a
     *  ROOT object (such as a TH1F or TProfile). The name given to the
     *  object constructor will be used to manipulate the pointer to
     *  the copy which is stored.
     */
    template <typename T>
    void create(const T obj);

    /// Get access to a ROOT object which will be stored
    /**
     *  Using the name of the TObject staged for storage with the
     *  Algo::create function, retrieve and manipulate the object, used
     *  for e.g. filling a histogram.
     */
    template <typename T>
    T* grab(const std::string& name);

    void feedConfig(const std::string fileName, bool print_conf = false);

    /// const pointer access to the configuration class
    const xTRT::Config* config() const;

    /// Sets the treeOutput name for the EL::NTupleSvc
    void setTreeOutputName(const std::string name);

    virtual EL::StatusCode setupJob(EL::Job& job);
    virtual EL::StatusCode fileExecute();
    virtual EL::StatusCode histInitialize();
    virtual EL::StatusCode changeInput(bool firstFile);
    virtual EL::StatusCode initialize();
    virtual EL::StatusCode execute();
    virtual EL::StatusCode postExecute();
    virtual EL::StatusCode finalize();
    virtual EL::StatusCode histFinalize();

    /// creates and sets up the InDetTrackSelectionTool
    EL::StatusCode setupTrackSelectionTools();
    /// creates and sets up the GoodRunsListSelectionTool
    EL::StatusCode enableGRLTool();
    /// creates and sets up the PileupReweightingTool
    EL::StatusCode enablePRWTool();
    /// creates and sets up the ConfigTool, DecisionTool, and MatchingTool
    EL::StatusCode enableTriggerTools();

    std::shared_ptr<xTRT::ParticleIdSvc> particleIdSvc() const;

    /// returns the raw track container (no selection applied)
    const xAOD::TrackParticleContainer* trackContainer();
    /// returns the raw electron container (no selection applied)
    const xAOD::ElectronContainer*      electronContainer();
    /// returns the raw electron container (no selection applied)
    const xAOD::MuonContainer*          muonContainer();

    /// checks if a track passes cuts defined in the config
    static bool passTrackSelection(const xAOD::TrackParticle* track, const xTRT::Config* conf);
    /// checks if an electron passes cuts defined in the config
    static bool passElectronSelection(const xAOD::Electron* electron, const xTRT::Config* conf);
    /// checks if a muon passes cuts defined in the config
    static bool passMuonSelection(const xAOD::Muon* muon, const xTRT::Config* conf);

    /// use raw container to form deep copy containing only selected objects
    /**
     *  The class C must be a container of type T. This will create
     *  and return a deep copy of the raw container with name contName
     *  after applying the selection defined in the selector function.
     *
     *  @param raw the raw container
     *  @param selector the function which applies the selection
     *  @param contName the name of the new deep copy container
     */
    template<class C, class T>
    const C* selectedContainer(const C* raw,
                               std::function<bool(const T*,const xTRT::Config*)> selector,
                               const std::string& contName);

    /// applies selectedContainer on tracks
    const xAOD::TrackParticleContainer* selectedTracks();
    /// applies selectedContainer on electrons
    const xAOD::ElectronContainer*      selectedElectrons();
    /// applies selectedContainer on muons
    const xAOD::MuonContainer*          selectedMuons();

    /// retrieves the TruthParticle from a given TrackParticle
    const xAOD::TruthParticle* truthParticle(const xAOD::TrackParticle* track);

    /// return the total event weight
    float eventWeight() const;
    /// return the average number of collisions per bunch crossing
    float averageMu();
    /// return whether the lumi block is good
    bool  passGRL() const;

    xAOD::TEvent* event();
    xAOD::TStore* store();
    const xAOD::EventInfo* eventInfo();

    /// get a hit summary object based on the track, surface measurement, and drift circle
    xTRT::HitSummary getHitSummary(const xAOD::TrackParticle* track,
                                   const xAOD::TrackStateValidation* msos,
                                   const xAOD::TrackMeasurementValidation* driftCircle);

    /// check whether a trigger fired
    bool triggerPassed(const std::string trigName) const;
    /// check whether a list of triggers fired
    bool triggersPassed(const std::vector<std::string>& trigNames) const;

    uint8_t nSilicon(const xAOD::TrackParticle* track) const;
    uint8_t nSiliconHoles(const xAOD::TrackParticle* track) const;
    uint8_t nSiliconShared(const xAOD::TrackParticle* track) const;
    float   deltaz0sinTheta(const xAOD::TrackParticle *track, const xAOD::Vertex* vtx) const;
    double  d0signif(const xAOD::TrackParticle *track) const;

    template <class T1, class T2>
    T1 get(const SG::AuxElement::ConstAccessor<T1>& acc, const T2* cont, std::string vn = "") const;

    ClassDef(xTRT::Algo, 1);

  };

}

#include "Algo.icc"

#endif
