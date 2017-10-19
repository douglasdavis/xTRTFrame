#ifndef xTRTFrame_TruthLoop_h
#define xTRTFrame_TruthLoop_h

// xTRTFrame
#include <xTRTFrame/Algorithm.h>

namespace xTRT {

  class TruthLoop : public xTRT::Algorithm {

  private:

    bool m_fillLeptonsOnly; //!
    bool m_saveHits;        //!
    bool m_type0only;       //!
    bool m_divZweightBy1k;  //!
    bool m_anaTracks;       //!
    bool m_anaElecs;        //!
    bool m_anaMuons;        //!

    TTree* m_true_Zee_tree;      //!
    TTree* m_true_Zmumu_tree;    //!
    TTree* m_true_Jpsiee_tree;   //!
    TTree* m_true_Jpsimumu_tree; //!

    float       m_avgMu;        //!
    float       m_weight;       //!
    int         m_pdgId;        //!
    float       m_trkOcc;       //!
    float       m_pT;           //!
    float       m_p;            //!
    float       m_truthMass;    //!
    float       m_eta;          //!
    float       m_phi;          //!
    float       m_theta;        //!
    float       m_eProbHT;      //!
    float       m_eProbToT;     //!
    float       m_eProbToT2;    //!
    float       m_eProbComb;    //!
    float       m_eProbComb2;   //!
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

    TruthLoop();
    virtual ~TruthLoop();

    virtual EL::StatusCode histInitialize() override;
    virtual EL::StatusCode initialize()     override;
    virtual EL::StatusCode execute()        override;

    void analyzeTrack(const xAOD::TrackParticle* track);
    void clearVectors();
    bool fillHitBasedVariables(const xAOD::TrackParticle* track,
                               const xAOD::TrackStateValidation* msos,
                               const xAOD::TrackMeasurementValidation* driftCircle,
                               const bool type0only);

    ClassDefOverride(xTRT::TruthLoop, 1);

  };

}

#endif
