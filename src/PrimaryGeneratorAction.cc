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
/// \file hadronic/Hadr01/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
//
/////////////////////////////////////////////////////////////////////////
//
// PrimaryGeneratorAction
//
// Created: 31.01.03 V.Ivanchenko
//
// Modified:
// 04.06.2006 Adoptation of Hadr01 (V.Ivanchenko)
// 16.11.2006 Add option allowing to have user defined beam position (VI)
//
////////////////////////////////////////////////////////////////////////
//

#include "ScoringUtility.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction *detectorConstruction)
    : G4VUserPrimaryGeneratorAction(),
      fParticleGun(0), fDetector(0)
{
  fParticleGun = new G4ParticleGun(1);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
  fParticleGun->SetParticleMomentum(1000.);
  fDetector = detectorConstruction;
  fDetector->SetPrimaryGeneratror(this);
}

void PrimaryGeneratorAction::UpdateBeamConfiguration(){
  fBeamMeanX = fDetector->GetBeamMeanX();
  fBeamMeanY = fDetector->GetBeamMeanY();
  fBeamMeanXp = fDetector->GetBeamMeanXp();
  fBeamMeanYp = fDetector->GetBeamMeanYp();
  fBeamSigmaX = fDetector->GetBeamSigmaX();
  fBeamSigmaY = fDetector->GetBeamSigmaY();
  fBeamSigmaXp = fDetector->GetBeamSigmaXp();
  fBeamSigmaYp = fDetector->GetBeamSigmaYp();
  fBeamZpos = fDetector->GetBeamZpos();
  fShootRandom = fDetector->GetShootRandom();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  // if(fHisto->DefaultBeamPosition()) {
  //   static const G4double offset = 2*CLHEP::mm;
  //   G4double zVertex = -1.0*m;
  //   fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,zVertex));
  // }
  if (!fShootRandom)
  {
    fParticleGun->SetParticlePosition(G4ThreeVector(fBeamMeanX, fBeamMeanY, fBeamZpos));
  }
  else if (!ScoringUtility::GetInstance()->GetStudyMuons())
  {
    G4double dummyX = G4RandGauss::shoot(0, 1.);
    G4double dummyY = G4RandGauss::shoot(0, 1.);
    
    G4double x = fBeamSigmaX * dummyX;
    G4double y = fBeamSigmaY * dummyY;

    G4double xP = fBeamSigmaXp * (dummyX / mm);
    G4double yP = fBeamSigmaYp * (dummyY / mm);
    G4double momentumZ = (400. * GeV) / sqrt(1. + xP * xP + yP * yP);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(momentumZ * xP, momentumZ * yP, momentumZ).unit());
    fParticleGun->SetParticlePosition(G4ThreeVector(x + fBeamMeanX, y + fBeamMeanY, fBeamZpos));
  }
  else{
    G4double randDummy = G4RandFlat::shoot(0., 1.);
    fParticleGun->SetParticlePosition(G4ThreeVector(fBeamMeanX, fBeamMeanY, fBeamZpos));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
    G4double eMin =  10001. + 105.7;
    G4double eMax = 399999.;
    fParticleGun->SetParticleEnergy(eMin + randDummy*(eMax - eMin));
  }

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
