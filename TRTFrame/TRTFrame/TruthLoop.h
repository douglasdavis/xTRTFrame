#ifndef TRTFrame_TruthLoop_h
#define TRTFrame_TruthLoop_h

// TRTFrame
#include <TRTFrame/LoopAlg.h>

namespace xTRT {

  class TruthLoop : public xTRT::LoopAlg {

  private:

    bool m_fillLeptonsOnly;
    bool m_saveHits;

    TTree*      m_elTree; //!
    TTree*      m_muTree; //!
    TTree*      m_piTree; //!
    TTree*      m_otTree; //!
    TTree*      m_zdTree; //!
    TTree*      m_jdTree; //!

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

    virtual EL::StatusCode histInitialize();
    virtual EL::StatusCode initialize();
    virtual EL::StatusCode execute();

    void clearVectors();
    bool fillHitBasedVariables(const xAOD::TrackParticle* track,
                               const xAOD::TrackStateValidation* msos,
                               const xAOD::TrackMeasurementValidation* driftCircle,
                               const bool type0only);

    ClassDef(xTRT::TruthLoop, 1);

  };

}

#endif
