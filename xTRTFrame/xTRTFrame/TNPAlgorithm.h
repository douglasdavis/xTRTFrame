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

    float m_tag_pT;      //!
    float m_probe_pT;    //!
    float m_probe_relpT; //!
    int   m_probe_nTRT;  //!
    int   m_tag_nTRT;    //!
    int   m_probe_nPix;  //!
    int   m_tag_nPix;    //!
    int   m_probe_nSi;   //!
    int   m_tag_nSi;     //!

    bool m_selectionCalled; //!
    bool m_containersMade;  //!

    std::vector<float>       m_invMasses;    //!
    std::vector<std::size_t> m_probeIndices; //!
    std::vector<std::size_t> m_tagIndices;   //!
    typedef std::vector<std::size_t>::iterator idx_t;

    EL::StatusCode performSelection();
    EL::StatusCode makeContainers();
    void           clear();

    static bool passAuthor(const xAOD::Electron* el);
    static bool passTightLH(const xAOD::Electron* el);
    static bool passLoose(const xAOD::Electron* el);

  public:

    TNPAlgorithm();
    virtual ~TNPAlgorithm();

    /// retrieve the container of probe electrons
    const xAOD::ElectronContainer* probeElectrons();
    /// retrieve the container of tag electrons
    const xAOD::ElectronContainer* tagElectrons();

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
    /// te the list of tag indices
    const std::vector<std::size_t>& tagIndices()   const;
    /// get the invariant masses of all good TNP pairs
    const std::vector<float>&       invMasses()    const;

    ClassDefOverride(xTRT::TNPAlgorithm, 1);

  };

}

#include "TNPAlgorithm.icc"

#endif
