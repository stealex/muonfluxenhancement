#ifndef BIASINGMESSENGER_HH
#define BIASINGMESSENGER_HH

#include "G4UImessenger.hh"

class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;

class BiasingMessenger : G4UImessenger
{
public:
    static BiasingMessenger *GetInstance();
    virtual ~BiasingMessenger();

    virtual void SetNewValue(G4UIcommand *, G4String);

private:
    BiasingMessenger();

private:
    static BiasingMessenger *fInstance;

    G4UIcmdWithAnInteger *fBiasMethodCmd;
    G4UIcmdWithADouble   *fBiasXSFactorCmd;
    G4UIcmdWithAnInteger *fBiasMuonFluxSplitFactCmd;
    G4UIcmdWithAString   *fBiasMesonsPDGidCmd;
    G4UIcmdWithADouble   *fBiasDecayZMaxCmd;
};


#endif