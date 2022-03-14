#include "BiasingUtility.hh"

#include "BiasingMessenger.hh"

#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"

#include <string>
#include <vector>

BiasingMessenger *BiasingMessenger::fInstance = nullptr;

BiasingMessenger *BiasingMessenger::GetInstance()
{
    if (!fInstance)
    {
        fInstance = new BiasingMessenger();
    }
    return fInstance;
}

BiasingMessenger::BiasingMessenger() : G4UImessenger()
{
    fBiasMethodCmd = new G4UIcmdWithAnInteger("/biasing/BiasMethod", this);
    fBiasMethodCmd->SetGuidance("Flag to bias the inelastic interactions of pions and kaons to give also a muon through artificial decay");
    fBiasMethodCmd->AvailableForStates(G4State_PreInit);

    fBiasDecayZMaxCmd = new G4UIcmdWithADouble("/biasing/DecayZMax", this);
    fBiasDecayZMaxCmd->SetGuidance("maximum z-coordinate until which particles are forced to decay (mm)");
    fBiasDecayZMaxCmd->SetDefaultValue(0.0);
    fBiasDecayZMaxCmd->AvailableForStates(G4State_PreInit);
}

void BiasingMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    BiasingUtility *biasUtil = BiasingUtility::GetInstance();

    if (command == fBiasMethodCmd)
    {
        biasUtil->SetBiasMethod(fBiasMethodCmd->GetNewIntValue(newValue));    
    }
    else if (command == fBiasDecayZMaxCmd){
        biasUtil->SetDecayZMax(fBiasDecayZMaxCmd->GetNewDoubleValue(newValue));
    }
}

BiasingMessenger::~BiasingMessenger(){
    if(fBiasMethodCmd) delete fBiasMethodCmd;
    if(fBiasDecayZMaxCmd) delete fBiasDecayZMaxCmd;
}