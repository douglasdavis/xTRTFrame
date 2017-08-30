#ifndef TRTFrame_Algo_h
#define TRTFrame_Algo_h

// C++
#include <memory>
#include <vector>
#include <map>

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

    std::unique_ptr<xTRT::Config> m_config;
    std::string m_outputName;

    std::map<std::string,TObject*> m_objStore; //!

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
    xAOD::TEvent* m_event;              //!
    xAOD::TStore* m_store;              //!

    std::shared_ptr<xTRT::ParticleIdSvc> m_pidSvc; //!

  public:

    // this is a standard constructor
    Algo();
    virtual ~Algo();

    /*! Creates a ROOT object to be stored.
     *
     * This allows easy creation by just calling the constructor of a
     * ROOT object (such as a TH1F or TProfile). The name given to the
     * object constructor will be used to manipulate the pointer to
     * the copy which is stored.
     */
    template <typename T>
    void create(const T obj);

    /*! Get access to a ROOT object which will be stored
     *
     * Using the name of the TObject staged for storage with the
     * Algo::create function, retrieve and manipulate the object, used
     * for e.g. filling a histogram.
     */
    template <typename T>
    T* grab(const std::string& name);

    /*! Send YAML file to be parsed by the configuration class
     */
    void feedConfig(const std::string fileName, bool print_conf = false);

    /*! const pointer access to the configuration class
     */
    xTRT::Config* config() const;

    /*! Sets the treeOutput name for the EL::NTupleSvc
     */
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

    EL::StatusCode setupTrackSelectionTools();
    EL::StatusCode enableGRLTool();
    EL::StatusCode enablePRWTool();
    EL::StatusCode enableTriggerTools();

    std::shared_ptr<xTRT::ParticleIdSvc> particleIdSvc() const;

    const xAOD::TrackParticleContainer* trackContainer();
    const xAOD::ElectronContainer*      electronContainer();
    const xAOD::MuonContainer*          muonContainer();

    const xAOD::TruthParticle* truthParticle(const xAOD::TrackParticle* track);

    float eventWeight() const;
    float averageMu();
    bool  passGRL() const;

    xAOD::TEvent* event();
    xAOD::TStore* store();
    const xAOD::EventInfo* eventInfo();

    xTRT::HitSummary getHitSummary(const xAOD::TrackParticle* track,
                                   const xAOD::TrackStateValidation* msos,
                                   const xAOD::TrackMeasurementValidation* driftCircle);

    bool triggerPassed(const std::string trigName) const;
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
