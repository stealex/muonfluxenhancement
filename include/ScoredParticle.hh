#ifndef SCOREDPARTICLE_HH
#define SCOREDPARTICLE_HH

#include "TObject.h"

#include "TVector3.h"
#include "TString.h"

class ScoredParticle : public TObject
{
public:
    ScoredParticle();
    ~ScoredParticle();

    TVector3 GetProdPos() { return prodPos; }
    TVector3 GetProdMom() { return prodMom; }
    Int_t GetPDGcode() { return pdgID; }
    TString GetProdProcess() { return prodProcessName; }
    TString GetEndProcess() { return endProcessName; }

    void SetProdPos(TVector3 val) {  prodPos = val; }
    void SetProdMom(TVector3 val) {  prodMom = val; }
    void SetPDGcode(Int_t val) {  pdgID = val; }
    void SetProdProcess(TString val) { prodProcessName = val; }
    void SetEndProcess(TString val) {  endProcessName = val; }

private:
    TVector3 prodPos;
    TVector3 prodMom;

    Int_t pdgID;
    TString prodProcessName;
    TString endProcessName;

    ClassDef(ScoredParticle, 1);
};

#endif
