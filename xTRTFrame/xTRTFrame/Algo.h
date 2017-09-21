/** @file  Algo.h
 *  @brief xTRT::Algo class header
 *  @namespace xTRT
 *  @brief main xTRTFrame namespace
 *  @class xTRT::Algo
 *  @brief Base class for running a
 *         xTRTFrame analysis algorithm
 *
 *  This class has two purposes: one is to be a skeleton for the steps
 *  needed to run a xTRTFrame algorithm. The second is the supply many
 *  helper functions and utilities for creating an algorithm for TRT
 *  work.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef xTRTFrame_Algo_h
#define xTRTFrame_Algo_h

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

// xTRTFrame
#include <xTRTFrame/Utils.h>
#include <xTRTFrame/Accessors.h>
#include <xTRTFrame/HitSummary.h>
#include <xTRTFrame/ParticleId.h>
#include <xTRTFrame/Config.h>

// ROOT
#include <TTree.h>

namespace xTRT {

  class Algo : public EL::Algorithm {

  private:
    xTRT::Config m_config;

    std::map<std::string,TObject*> m_objStore; //!

    int m_eventCounter;                 //!
    const xAOD::EventInfo* m_eventInfo; //!
    xAOD::TEvent* m_event;              //!
    xAOD::TStore* m_store;              //!

    asg::AnaToolHandle<IGoodRunsListSelectionTool> m_GRLToolHandle;          //!
    asg::AnaToolHandle<CP::IPileupReweightingTool> m_PRWToolHandle;          //!
    asg::AnaToolHandle<TrigConf::ITrigConfigTool>  m_trigConfToolHandle;     //!
    asg::AnaToolHandle<Trig::TrigDecisionTool>     m_trigDecToolHandle;      //!
    asg::AnaToolHandle<Trig::IMatchingTool>        m_trigMatchingToolHandle; //!

    asg::AnaToolHandle<InDet::IInDetTrackSelectionTool> m_trackSelToolHandle;     //!
    asg::AnaToolHandle<InDet::IInDetTrackSelectionTool> m_trackElecSelToolHandle; //!
    asg::AnaToolHandle<InDet::IInDetTrackSelectionTool> m_trackMuonSelToolHandle; //!

  protected:

    std::string m_outputName;

  public:

    Algo();
    virtual ~Algo();

    void feedConfig(const std::string fileName, bool print_conf = false);

    /// Creates a ROOT object to be stored.
    /**
     *  This allows easy creation by just calling the constructor of a
     *  ROOT object (such as a TH1F or TProfile). The name given to the
     *  object constructor will be used to manipulate the pointer to
     *  the copy which is stored.
     *
     *  @param obj the ROOT output object (THx, TProfile, etc.)
     */
    template <typename T>
    void create(const T obj);

    /// Get access to a ROOT object which will be stored
    /**
     *  Using the name of the TObject staged for storage with the
     *  Algo::create function, retrieve and manipulate the object, used
     *  for e.g. filling a histogram.
     *
     *  @param name the name of the created ROOT object to update/modify.
     */
    template <typename T>
    T* grab(const std::string& name);

    /// const pointer access to the configuration class
    const xTRT::Config* config() const;

    /// Sets the treeOutput name for the EL::NTupleSvc
    void setTreeOutputName(const std::string name);

    /// EventLoop API function
    virtual EL::StatusCode setupJob(EL::Job& job);
    /// EventLoop API function
    virtual EL::StatusCode fileExecute();
    /// EventLoop API function
    virtual EL::StatusCode histInitialize();
    /// EventLoop API function
    virtual EL::StatusCode changeInput(bool firstFile);
    /// EventLoop API function
    virtual EL::StatusCode initialize();
    /// EventLoop API function
    virtual EL::StatusCode execute();
    /// EventLoop API function
    virtual EL::StatusCode postExecute();
    /// EventLoop API function
    virtual EL::StatusCode finalize();
    /// EventLoop API function
    virtual EL::StatusCode histFinalize();

    /// creates and sets up the InDetTrackSelectionTool
    EL::StatusCode setupTrackSelectionTools();
    /// creates and sets up the GoodRunsListSelectionTool
    EL::StatusCode enableGRLTool();
    /// creates and sets up the PileupReweightingTool
    EL::StatusCode enablePRWTool();
    /// creates and sets up the ConfigTool, DecisionTool, and MatchingTool
    EL::StatusCode enableTriggerTools();

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

    /// retrieves the TruthParticle associated with the input TrackParticle
    const xAOD::TruthParticle* truthParticle(const xAOD::TrackParticle* track);

    /// return the total event weight
    float eventWeight() const;
    /// return the average number of collisions per bunch crossing
    float averageMu();
    /// return the number of primary vertices
    std::size_t NPV() const;
    /// return whether the lumi block is good
    bool  passGRL() const;

    /// get pointer to current xAOD::TEvent
    xAOD::TEvent* event();
    /// get pointer to current xAOD::TStore
    xAOD::TStore* store();
    /// get const pointer to the current event's xAOD::EventInfo
    const xAOD::EventInfo* eventInfo();

    /// get a hit summary object based on the track, surface measurement, and drift circle
    xTRT::HitSummary getHitSummary(const xAOD::TrackParticle* track,
                                   const xAOD::TrackStateValidation* msos,
                                   const xAOD::TrackMeasurementValidation* driftCircle);

    /// check whether a trigger fired
    bool triggerPassed(const std::string trigName) const;
    /// check whether a list of triggers fired
    bool triggersPassed(const std::vector<std::string>& trigNames) const;

    /// return the number of TRT hits on the track (all)
    static int nTRT(const xAOD::TrackParticle* track);
    /// return the number of TRT prec+tube (non outlier)
    static int nTRT_PrecTube(const xAOD::TrackParticle* track);
    /// return the number of TRT outliers
    static int nTRT_Outlier(const xAOD::TrackParticle* track);
    /// return the number of silicon hits (Pixel + SCT)
    static int nSilicon(const xAOD::TrackParticle* track);
    /// return the number of silicon holes (Pixel + SCT)
    static int nSiliconHoles(const xAOD::TrackParticle* track);
    /// return the number of silicon shared hits (Pixel + SCT)
    static int nSiliconShared(const xAOD::TrackParticle* track);
    /// return the (delta z0)*sin(theta) of the track
    static float deltaz0sinTheta(const xAOD::TrackParticle *track, const xAOD::Vertex* vtx);

    /// return the d0 significance of the track
    /**
     *  This function requires the EventInfo so it cannt be static
     */
    double d0signif(const xAOD::TrackParticle *track) const;

    /// grab aux data by using ConstAccessor and some object
    /**
     *  Using a ConstAccessor, look to see if the object has the
     *  auxdata and then return it.
     *
     *  @param acc the const accessor object
     *  @param xobj the xAOD object to grab the auxdata from
     *  @param adn the auxdata variable name
     */
    template <class T1, class T2>
    T1 get(const SG::AuxElement::ConstAccessor<T1>& acc, const T2* xobj, std::string adn = "") const;

    /// grab aux data from an xAOD object based on name
    /**
     *  This function will create a ConstAccessor to retrieve some aux
     *  data from the object. This is differen from the get function
     *  in that you must supply the return type, where with get the
     *  return type is deduced by the compiler from the already
     *  declared ConstAccessor.
     *
     *  @param xobj the xAOD object to retrieve the auxdata from
     *  @param adn the aux data variable name
     */
    template <typename T1, typename T2 = SG::AuxElement>
    T1 retrieve(const T2* xobj, const char* adn) const;

    /// get reference to the InDetTrackSelectionTool handle for tracks
    const asg::AnaToolHandle<InDet::IInDetTrackSelectionTool>& trackSelToolHandle()     const;
    /// get reference to the InDetTrackSelectionTool handle for electrons
    const asg::AnaToolHandle<InDet::IInDetTrackSelectionTool>& trackElecSelToolHandle() const;
    /// get reference to the InDetTrackSelectionTool handle for muon
    const asg::AnaToolHandle<InDet::IInDetTrackSelectionTool>& trackMuonSelToolHandle() const;

    ClassDef(xTRT::Algo, 1);

  };

}

#include "Algo.icc"

#endif
