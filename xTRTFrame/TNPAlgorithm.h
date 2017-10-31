/** @file  TNPAlgorithm.h
 *  @brief xTRT::TNPAlgorithm class header
 *  @class xTRT::TNPAlgorithm
 *  @brief A class to base a Tag and Probe algorithm on
 *
 *  This class builds on the xTRT::Algorithm class to make it easy to
 *  perform a tag and probe selection.
 *
 *  @author Douglas Davis < ddavis@cern.ch >
 */

#ifndef xTRTFrame_TNPAlgorithm_h
#define xTRTFrame_TNPAlgorithm_h

#include <xTRTFrame/Algorithm.h>

namespace xTRT {

  class TNPAlgorithm : public xTRT::Algorithm {

  private:
    ///////// cuts ////////////////////////
    float m_tag_maxP;    //!
    float m_tag_pT;      //!
    int   m_tag_nTRT;    //!
    int   m_tag_nPix;    //!
    int   m_tag_nSi;     //!

    float m_probe_maxP;  //!
    float m_probe_pT;    //!
    float m_probe_relpT; //!
    int   m_probe_nTRT;  //!
    int   m_probe_nPix;  //!
    int   m_probe_nSi;   //!

    float m_tag_iso_ptcone20;     //!
    float m_tag_iso_topoetcone20; //!

    float m_muon_maxP;  //!
    float m_muon_pT;    //!
    int   m_muon_nTRT;  //!
    int   m_muon_nPix;  //!
    int   m_muon_nSi;   //!
    int   m_muon_nPrec; //!

    float m_muon_iso_ptvarcone30;  //!
    float m_muon_iso_topoetcone20; //!
    ///////////////////////////////////////

  private:
    bool m_selectionCalled; //!
    bool m_containersMade;  //!

    std::vector<float>       m_invMassesEl;  //!
    std::vector<float>       m_invMassesMu;  //!
    std::vector<std::size_t> m_probeIndices; //!
    std::vector<std::size_t> m_tagIndices;   //!
    std::vector<std::size_t> m_muonIndices;  //!
    typedef std::vector<std::size_t>::iterator idx_t;


  private:
    EL::StatusCode performZeeSelection();
    EL::StatusCode performZmumuSelection();
    EL::StatusCode performSelections();
    EL::StatusCode makeContainers();
    void           clear();

    bool passZeeTNP(const xAOD::Electron* Tag, const xAOD::Electron* Probe);
    bool passZmumuTNP(const xAOD::Muon* mu1, const xAOD::Muon* mu2);

    ////// selection helper functions
    static bool passAuthor(const xAOD::Electron* electron);
    static bool passTightLH(const xAOD::Electron* electron);
    static bool passLoose(const xAOD::Electron* electron);
    static bool passQuality(const xAOD::Muon* muon, const int npl_cut = 2);

  public:

    TNPAlgorithm();
    virtual ~TNPAlgorithm();

    /// delete copy constructor
    TNPAlgorithm(const TNPAlgorithm&) = delete;
    /// delete assignment operator
    TNPAlgorithm& operator=(const TNPAlgorithm&) = delete;
    /// delete move
    TNPAlgorithm(TNPAlgorithm&&) = delete;
    /// delete move assignment
    TNPAlgorithm& operator=(TNPAlgorithm&&) = delete;

    /// retrieve the container of probe electrons
    const xAOD::ElectronContainer* probeElectrons();
    /// retrieve the container of tag electrons
    const xAOD::ElectronContainer* tagElectrons();
    /// retrieve the container of good muons
    const xAOD::MuonContainer*     goodMuons();

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

    /// get the list of probe indices
    const std::vector<std::size_t>& probeIndices() const;
    /// get the list of tag indices
    const std::vector<std::size_t>& tagIndices()   const;
    /// get the list of muon indices
    const std::vector<std::size_t>& muonIndices()  const;
    /// get the invariant masses of all good el TNP pairs
    const std::vector<float>&       invMassesEl()  const;
    /// get the invariant masses of all good mu TNP pairs
    const std::vector<float>&       invMassesMu()  const;

    ClassDefOverride(xTRT::TNPAlgorithm, 1);

  };

}

#include "TNPAlgorithm.icc"

#endif
