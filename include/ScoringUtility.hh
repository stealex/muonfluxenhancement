#ifndef __SCORINGUTILITYHH__
#define __SCORINGUTILITYHH__

#include <map>
#include "TFile.h"
#include "TNtuple.h"
#include "TDirectory.h"
#include "TString.h"
#include "TTree.h"
#include "TH1.h"


class ScoringUtility{
    public:
        static ScoringUtility* GetInstance();
        ~ScoringUtility();
        void CreateOutputStructure();
        void CloseOutputStructre();

        void SetSeed(Int_t val) {fSeed = val;}
        Int_t GetSeed() {return fSeed;}
        void AddPlaneScoringTree(TTree *val) {fPlaneScoringTrees.push_back(val);}

        TNtuple* GetScoringTree() {return fScoringTree;}
        // TTree* GetScoringTree() {return fScoringTree;}
        TDirectory *GetScoringDir() {return fScoringDir;}

        void SetScoringPlaneZPos(Double_t val) {fScoringPlaneZPos = val;}
        Double_t GetScoringPlaneZPos() {return fScoringPlaneZPos;}

        void SetStudyMuons(Bool_t val) {fStudyMuons = val;}
        Bool_t GetStudyMuons() {return fStudyMuons;}

        void FillHisto(TString histoName, Double_t val){ fHistogramMap.at(histoName)->Fill(val);}
        void FilldEdXHisto(Int_t iBin, Double_t val){fdEdXHistos.at(iBin)->Fill(val);}

        void SetSlimOutput(Bool_t val) {fSlimOutput = val;}
        Bool_t GetSlimOutput() {return fSlimOutput;}

        void SetTargetLength(Double_t val) {fTargetLength = val;}
    private:
        ScoringUtility();

    private:
        TString fScoringFileName;
        
        TFile *fScoringFile;
        TDirectory *fScoringDir;
        // TTree *fScoringTree;
        TNtuple *fScoringTree;
        std::vector<TTree*> fPlaneScoringTrees;
        Int_t fSeed;
        Double_t fScoringPlaneZPos;
        std::map<TString, TH1*> fHistogramMap;
        std::vector<TH1*> fdEdXHistos;
        Bool_t fStudyMuons;
        static ScoringUtility *fInstance;
        Bool_t fSlimOutput;
        Double_t fTargetLength;
};

#endif