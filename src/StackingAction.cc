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
/// \file hadronic/Hadr02/src/StackingAction.cc
/// \brief Implementation of the StackingAction class
//
//
/////////////////////////////////////////////////////////////////////////
//
// StackingAction
//
// Created: 31.04.2006 V.Ivanchenko
//
// Modified:
//
////////////////////////////////////////////////////////////////////////
//

#include "StackingAction.hh"

#include "StackingMessenger.hh"

#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::StackingAction()
    : G4UserStackingAction(),
      fStackMessenger(0),
      fKillAll(false),
      fKillEM(false),
      fKillAfterMuonEmitted(false),
      fEnergyThreshold(-1.),
      fMuonExistsInEvent(false),
      fTrackClassificationMap()
{
  fStackMessenger = new StackingMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::~StackingAction()
{
  delete fStackMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track *track)
{
  G4ClassificationOfNewTrack status = fUrgent;
  if (track->GetTrackID() == 1)
  {
    return status;
  }
  if (fKillAll)
  {
    status = fKill;
  }
  else if (fKillEM && track->GetDefinition()->GetPDGMass() < MeV)
  {
    status = fKill;
  }

  G4double energy = track->GetKineticEnergy();
  if (energy / CLHEP::GeV < fEnergyThreshold)
  {
    status = fKill;
  }
  if (track->GetParticleDefinition()->GetParticleName().contains("neutron"))
  {
    if (track->GetKineticEnergy() < 0.1 * GeV)
      status = fKill;
  }
  return status;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StackingAction::PrepareNewEvent()
{
  fTrackClassificationMap.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StackingAction::NewStage()
{
  stackManager->ReClassify();
}