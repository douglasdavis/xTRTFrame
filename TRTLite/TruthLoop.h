#ifndef TRTLite_TruthLoop_h
#define TRTLite_TruthLoop_h

// TRTLite
#include <TRTLite/LoopAlg.h>

namespace TRTLite {

  class TruthLoop : public TRTLite::LoopAlg {

  private:

    bool m_fillLeptonsOnly;
    bool m_saveHits;

    TTree*      m_elTree; //!
    TTree*      m_muTree; //!
    TTree*      m_piTree; //!
    TTree*      m_otTree; //!
    TTree*      m_zdTree; //!
    TTree*      m_jdTree; //!

    float       m_weight;     //!
    int         m_pdgId;      //!
    float       m_trkOcc;     //!
    float       m_pT;         //!
    float       m_p;          //!
    float       m_truthMass;  //!
    float       m_eta;        //!
    float       m_phi;        //!
    float       m_theta;      //!
    float       m_eProbHT;    //!
    float       m_eProbToT;   //!
    float       m_eProbToT2;  //!
    float       m_eProbComb;  //!
    float       m_eProbComb2; //!
    int         m_NTRThits;   //!
    int         m_NHThits;    //!
    float       m_dEdxNoHT;   //!
    int         m_NhitsdEdx;  //!

    std::vector<int>   m_HTMB;        //!
    std::vector<int>   m_gasType;     //!
    std::vector<int>   m_bec;         //!
    std::vector<int>   m_layer;       //!
    std::vector<int>   m_strawlayer;  //!
    std::vector<int>   m_strawnumber; //!
    std::vector<float> m_drifttime;   //!
    std::vector<float> m_tot;         //!
    std::vector<float> m_T0;          //!
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
                               const xAOD::TrackMeasurementValidation* driftCircle);

    void setFillLeptonsOnly(bool var) { m_fillLeptonsOnly = var; }
    void setSaveHits(bool var)        { m_saveHits        = var; }

    ClassDef(TruthLoop, 1);

  };

}

#endif
