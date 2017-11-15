/** @file  Algorithm.h
 *  @brief xTRT::Algorithm class header
 *  @namespace xTRT
 *  @brief main xTRTFrame namespace
 *  @class xTRT::Algorithm
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

#ifndef xTRTFrame_Algorithm_h
#define xTRTFrame_Algorithm_h

// C++
#include <memory>
#include <vector>
#include <map>
#include <functional>

// ATLAS
#include <xTRTFrame/AtlasIncludes.h>

// xTRTFrame
#include <xTRTFrame/Utils.h>
#include <xTRTFrame/Accessors.h>
#include <xTRTFrame/HitSummary.h>
#include <xTRTFrame/Config.h>

// ROOT
#include <TTree.h>

namespace xTRT {

  class Algorithm : public EL::Algorithm {

  private:
    xTRT::Config m_config;

    std::map<std::string,TObject*> m_objStore; //!

    int m_eventCounter;                 //!
    const xAOD::EventInfo* m_eventInfo; //!
    xAOD::TEvent* m_event;              //!
    xAOD::TStore* m_store;              //!

  private:
    asg::AnaToolHandle<IGoodRunsListSelectionTool>
    m_GRLToolHandle{"GoodRunsListSelectionTool/GRLTool",this}; //!
    asg::AnaToolHandle<CP::IPileupReweightingTool>
    m_PRWToolHandle{"CP::PileupReweightingTool/PRWTool",this}; //!
    asg::AnaToolHandle<TrigConf::ITrigConfigTool>
    m_trigConfToolHandle{"TrigConf::xAODConfigTool/xAODConfigTool",this}; //!
    asg::AnaToolHandle<Trig::TrigDecisionTool>
    m_trigDecToolHandle{"Trig::TrigDecisionTool/TrigDecisionTool",this}; //!
    asg::AnaToolHandle<Trig::IMatchingTool>
    m_trigMatchingToolHandle{"Trig::MatchingTool/TrigMatchingTool",this}; //!
    asg::AnaToolHandle<InDet::IInDetTrackSelectionTool>
    m_trackSelToolHandle{"InDet::InDetTrackSelectionTool/IDTSTracks",this}; //!
    asg::AnaToolHandle<InDet::IInDetTrackSelectionTool>
    m_trackSelElecToolHandle{"InDet::InDetTrackSelectionTool/IDTSElecs",this}; //!
    asg::AnaToolHandle<InDet::IInDetTrackSelectionTool>
    m_trackSelMuonToolHandle{"InDet::InDetTrackSelectionTool/IDTSMuons",this}; //!

  protected:
    std::string m_outputName{"xTRTFrameOutput"};

  public:

    Algorithm();
    virtual ~Algorithm();

    /// delete copy constructor
    Algorithm(const Algorithm&) = delete;
    /// delete assignment operator
    Algorithm& operator=(const Algorithm&) = delete;
    /// delete move
    Algorithm(Algorithm&&) = delete;
    /// delete move assignment
    Algorithm& operator=(Algorithm&&) = delete;

    void feedConfig(const std::string fileName, bool print_conf = false, bool mcMode = false);

  protected:
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
     *  Algorithm::create function, retrieve and manipulate the object, used
     *  for e.g. filling a histogram.
     *
     *  @param name the name of the created ROOT object to update/modify.
     */
    template <typename T>
    T* grab(const std::string& name);

  public:
    /// const pointer access to the configuration class
    const xTRT::Config* config() const;

    /// Sets the treeOutput name for the EL::NTupleSvc
    void setTreeOutputName(const std::string name);

  protected:
    /// creates and sets up the InDetTrackSelectionTool
    EL::StatusCode setupTrackSelectionTools();
    /// creates and sets up the GoodRunsListSelectionTool
    EL::StatusCode enableGRLTool();
    /// creates and sets up the PileupReweightingTool
    EL::StatusCode enablePRWTool();
    /// creates and sets up the ConfigTool, DecisionTool, and MatchingTool
    EL::StatusCode enableTriggerTools();

  public:
    /// checks if a track passes cuts defined in the config
    static bool passTrackSelection(const xAOD::TrackParticle* track, const xTRT::Config* conf);
    /// checks if an electron passes cuts defined in the config
    static bool passElectronSelection(const xAOD::Electron* electron, const xTRT::Config* conf);
    /// checks if a muon passes cuts defined in the config
    static bool passMuonSelection(const xAOD::Muon* muon, const xTRT::Config* conf);

  protected:
    /// returns the raw track container (no selection applied)
    const xAOD::TrackParticleContainer* trackContainer();
    /// returns the raw electron container (no selection applied)
    const xAOD::ElectronContainer*      electronContainer();
    /// returns the raw electron container (no selection applied)
    const xAOD::MuonContainer*          muonContainer();

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

  public:
    /// retrieves the TruthParticle associated with the input track particle
    static const xAOD::TruthParticle* getTruth(const xAOD::TrackParticle* track);
    /// retrieve the "original" xAOD::TrackParticle pointer from the electron
    static const xAOD::TrackParticle* getTrack(const xAOD::Electron* electron);
    /// retrieve the GSF xAOD::TrackParticle pointer from the electron
    static const xAOD::TrackParticle* getGSFTrack(const xAOD::Electron* electron);
    /// retrieve the xAOD::TrackParticle pointer from the muon
    static const xAOD::TrackParticle* getTrack(const xAOD::Muon* muon);

  protected:
    /// check if the sample is MC
    bool isMC() const;
    /// check if the sample is Data (convenience function, opposite of isMC())
    bool isData() const;
    /// return the total event weight
    float eventWeight();
    /// return the average number of collisions per bunch crossing
    float averageMu();
    /// return the number of primary vertices
    std::size_t NPV() const;
    /// return whether the lumi block is good
    bool passGRL() const;

    /// get pointer to current xAOD::TEvent
    xAOD::TEvent* event();
    /// get pointer to current xAOD::TStore
    xAOD::TStore* store();
    /// get const pointer to the current event's xAOD::EventInfo
    const xAOD::EventInfo* eventInfo() const;

  public:
    /// get a hit summary object based on the track, surface measurement, and drift circle
    static xTRT::HitSummary getHitSummary(const xAOD::TrackParticle* track,
                                          const xAOD::TrackStateValidation* msos,
                                          const xAOD::TrackMeasurementValidation* driftCircle);

  protected:
    /// check for a nullptr and print a debug message
    /**
     * A lot of xAOD pointers have the potential to be null, this is
     * just a utliity function to test for a nullptr and use the ASG
     * message service to print a debug message
     *
     * @param ptr the pointer to check
     * @param message the message to print with ANA_MSG_DEBUG
     * @return true if good, false if nullptr.
     */
    template <class T>
    bool debug_nullptr(const T* ptr, const std::string& message = "") const;

    /// check for a nullptr and print a warning message
    /**
     * see xTRT::Algorithm::debugnullptr, but this prints a warning
     * message instead of a debug message.
     *
     * @param ptr the pointer to check
     * @param message the message to print with ANA_MSG_WARNING
     * @return true if good, false if nullptr
     */
    template <class T>
    bool warn_nullptr(const T* ptr, const std::string& message = "") const;

  protected:
    /// check whether a trigger fired
    bool triggerPassed(const std::string trigName) const;
    /// check whether a list of triggers fired
    bool triggersPassed(const std::vector<std::string>& trigNames) const;
    /// check if an electron matches to any of the single electron triggers in config file
    bool singleElectronTrigMatched(const xAOD::Electron* electron);
    /// check if a muon matches to any of the single muon triggers in config file
    bool singleMuonTrigMatched(const xAOD::Muon* muon);

  public:
    /// return the number of TRT hits on the track (all)
    static int nTRT(const xAOD::TrackParticle* track);
    /// return the number of TRT prec+tube (non outlier)
    static int nTRT_PrecTube(const xAOD::TrackParticle* track);
    /// return the number of TRT outliers
    static int nTRT_Outlier(const xAOD::TrackParticle* track);
    /// return the number of Pixel hits
    static int nPixel(const xAOD::TrackParticle* track);
    /// return the number of silicon hits (Pixel + SCT)
    static int nSilicon(const xAOD::TrackParticle* track);
    /// return the number of silicon holes (Pixel + SCT)
    static int nSiliconHoles(const xAOD::TrackParticle* track);
    /// return the number of silicon shared hits (Pixel + SCT)
    static int nSiliconShared(const xAOD::TrackParticle* track);

    /// get the calo iso value from an electron (xAOD::Iso::topoetcone20)
    static float caloIso(const xAOD::Electron* electron);
    /// get the calo iso value from a muon
    static float caloIso(const xAOD::Muon* muon);
    /// get the track iso value from an electron (xAOD::Iso::ptvarcone20)
    static float trackIso(const xAOD::Electron* electron);
    /// get the track iso value from a muon
    static float trackIso(const xAOD::Muon* muon);
    /// get the track pTcone20 iso value from an electron (xAOD::Iso::ptcone20)
    static float trackIso_pTcone20(const xAOD::Electron* electron);
    /// get if electron is truth matched
    static bool  truthMatched(const xAOD::Electron* electron);
    /// get if muon is truth matched
    static bool  truthMatched(const xAOD::Muon* muon);

    /// return the TRT track occupancy
    static float trackOccupancy(const xAOD::TrackParticle* track);
    /// return the (delta z0)*sin(theta) of the track
    static float deltaz0sinTheta(const xAOD::TrackParticle* track, const xAOD::Vertex* vtx);
    /// return the d0 significance of the track
    static double d0signif(const xAOD::TrackParticle* track, const xAOD::EventInfo* evtinfo);

  protected:
    /// get reference to the InDetTrackSelectionTool handle for tracks
    const asg::AnaToolHandle<InDet::IInDetTrackSelectionTool>& trackSelToolHandle()     const;
    /// get reference to the InDetTrackSelectionTool handle for electrons
    const asg::AnaToolHandle<InDet::IInDetTrackSelectionTool>& trackSelElecToolHandle() const;
    /// get reference to the InDetTrackSelectionTool handle for muons
    const asg::AnaToolHandle<InDet::IInDetTrackSelectionTool>& trackSelMuonToolHandle() const;
    /// get reference to the Trigger Matching Tool
    const asg::AnaToolHandle<Trig::IMatchingTool>& trigMatchingToolHandle() const;
    /// get reference not class const Trigger Matching Tool
    const asg::AnaToolHandle<Trig::IMatchingTool>& trigMatchingToolHandle();

  protected:
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
    static T1 get(const SG::AuxElement::ConstAccessor<T1>& acc, const T2* xobj, const std::string adn = "");

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
    static T1 retrieve(const T2* xobj, const std::string adn);

  public:
    /// EventLoop API function
    virtual EL::StatusCode setupJob(EL::Job& job) override;
    /// EventLoop API function
    virtual EL::StatusCode fileExecute() override;
    /// EventLoop API function
    virtual EL::StatusCode histInitialize() override;
    /// EventLoop API function
    virtual EL::StatusCode changeInput(bool firstFile) override;
    /// EventLoop API function
    virtual EL::StatusCode initialize() override;
    /// EventLoop API function
    virtual EL::StatusCode execute() override;
    /// EventLoop API function
    virtual EL::StatusCode postExecute() override;
    /// EventLoop API function
    virtual EL::StatusCode finalize() override;
    /// EventLoop API function
    virtual EL::StatusCode histFinalize() override;

    ClassDefOverride(xTRT::Algorithm, 1);

  };

}

#include "Algorithm.icc"

#endif
