#ifndef BIASINGUTILITY_HH
#define BIASINGUTILITY_HH

#include <map>
#include <vector>

#include "G4String.hh"

class G4Track;

class BiasingUtility
{
public:
    static BiasingUtility *GetInstance();
    ~BiasingUtility();

    
    void SetBiasMethod(int iMethod) { fBiasMethod = iMethod; }
    int GetBiasMethod() { return fBiasMethod; }

    void PrintParticle(const G4Track*);

    void SetDecayZMax(double val) {fDecayZMax = val;}
    G4double GetDecayZMax() {return fDecayZMax;}

private:
    BiasingUtility();
    static BiasingUtility *fInstance;

    int fBiasMethod;
    double fBiasXSFactor;

    G4double fDecayZMax;
};

#endif