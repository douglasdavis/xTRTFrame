#ifndef xTRTUseTNP_TNPAlg_h
#define xTRTUseTNP_TNPAlg_h

// xTRTFrame
#include <xTRTFrame/TNPAlgorithm.h>

namespace xTRT {

  class TNPAlg : public xTRT::TNPAlgorithm {

  private:

    bool m_saveHits;        //!
    bool m_type0only;       //!

    TTree* m_tree_el_tags;   //!
    TTree* m_tree_el_probes; //!
    TTree* m_tree_mu;        //!

    float       m_avgMu;        //!
    float       m_weight;       //!
    float       m_trkOcc;       //!
    float       m_pT;           //!
    float       m_p;            //!
    float       m_eta;          //!
    float       m_phi;          //!
    float       m_theta;        //!
    float       m_eProbHT;      //!
    float       m_eProbToT;     //!
    float       m_eProbComb;    //!
    int         m_nTRThits;     //!
    int         m_nTRThitsMan;  //!
    int         m_nTRTouts;     //!
    int         m_nTRToutsMan;  //!
    int         m_nHThits;      //!
    int         m_nHThitsMan;   //!
    int         m_nArhits;      //!
    int         m_nArhitsMan;   //!
    int         m_nXehits;      //!
    int         m_nXehitsMan;   //!
    float       m_dEdxNoHT;     //!
    int         m_nHitsdEdx;    //!
    float       m_sumL;         //!
    float       m_sumToT;       //!
    float       m_fAr;          //!
    float       m_fHTMB;        //!

    std::vector<int>   m_HTMB;        //!
    std::vector<int>   m_gasType;     //!
    std::vector<int>   m_bec;         //!
    std::vector<int>   m_layer;       //!
    std::vector<int>   m_strawlayer;  //!
    std::vector<int>   m_strawnumber; //!
    std::vector<float> m_drifttime;   //!
    std::vector<float> m_tot;         //!
    std::vector<float> m_T0;          //!
    std::vector<int>   m_type;        //!
    std::vector<float> m_localTheta;  //!
    std::vector<float> m_localPhi;    //!
    std::vector<float> m_HitZ;        //!
    std::vector<float> m_HitR;        //!
    std::vector<float> m_rTrkWire;    //!
    std::vector<float> m_L;           //!

  public:

    TNPAlg();
    virtual ~TNPAlg();

    virtual EL::StatusCode histInitialize() override;
    virtual EL::StatusCode initialize()     override;
    virtual EL::StatusCode execute()        override;

    void analyzeTrack(const xAOD::TrackParticle* track, const bool is_el, const bool is_tag);
    void clearVectors();
    bool fillHitBasedVariables(const xAOD::TrackParticle* track,
                               const xAOD::TrackStateValidation* msos,
                               const xAOD::TrackMeasurementValidation* driftCircle,
                               const bool type0only);

    ClassDefOverride(xTRT::TNPAlg, 1);

  };

}

#endif
