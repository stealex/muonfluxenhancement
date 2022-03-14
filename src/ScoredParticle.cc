#include "ScoredParticle.hh"

ClassImp(ScoredParticle)

ScoredParticle::ScoredParticle(){
    prodPos = {-999., -999., -999.};
    prodMom = {-999., -999., -999.};
    pdgID = 0;
    prodProcessName = "";
    endProcessName = "";
}

ScoredParticle::~ScoredParticle(){}