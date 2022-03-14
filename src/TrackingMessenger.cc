//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file hadronic/Hadr02/src/TrackingMessenger.cc
/// \brief Implementation of the TrackingMessenger class
//
//
/////////////////////////////////////////////////////////////////////////
//
// TrackingMessenger
//
// Created: 31.05.2006 V.Ivanchenko
//
// Modified:
// 04.06.2006 Adoptation of hadr01 (V.Ivanchenko)
//
////////////////////////////////////////////////////////////////////////
//

#include "TObjArray.h"
#include "TObjString.h"
#include "TrackingMessenger.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "TrackingAction.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingMessenger::TrackingMessenger(TrackingAction *Track)
    : G4UImessenger(),
      fTrackAction(Track),
      fScoreVolumeParticlesCmd(0),
      fScoreVolumeParticlesMinMomentumCmd(0)

{
    fScoreVolumeParticlesCmd = new G4UIcmdWithAString("/score/VolumeParticles", this);
    fScoreVolumeParticlesCmd->SetGuidance(" Blank separated string with pdgids of particles to score");
    fScoreVolumeParticlesCmd->SetParameterName("scoreParts", true);
    fScoreVolumeParticlesCmd->SetDefaultValue("");

    fScoreVolumeParticlesMinMomentumCmd = new G4UIcmdWithADouble("/score/VolumeParticlesMinMomentum", this);
    fScoreVolumeParticlesMinMomentumCmd->SetGuidance(" minimum momentum to score a particle");
    fScoreVolumeParticlesMinMomentumCmd->SetParameterName("scoreParts", true);
    fScoreVolumeParticlesMinMomentumCmd->SetDefaultValue(3000.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingMessenger::~TrackingMessenger()
{
    delete fScoreVolumeParticlesCmd;
    delete fScoreVolumeParticlesMinMomentumCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == fScoreVolumeParticlesCmd)
    {
        TString partsRaw = newValue.data();
        TObjArray *l = partsRaw.Tokenize(" ");
        std::vector<Int_t> pdgIds(0);
        for (Int_t i = 0; i < l->GetEntries(); i++)
        {
            Int_t part = ((TObjString *)l->At(i))->GetString().Atoi();
            pdgIds.push_back(part);
            G4cout << "    Will score partilce " << part << G4endl;
        }

        fTrackAction->SetScoreParticles(pdgIds);
    }
    if (command == fScoreVolumeParticlesMinMomentumCmd){
        G4double minMomentum = fScoreVolumeParticlesMinMomentumCmd->GetNewDoubleValue(newValue);
        fTrackAction->SetScoreParticleMinMomentum(minMomentum);
        
        G4cout << "   Minimum momentum for scoring " << minMomentum << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
