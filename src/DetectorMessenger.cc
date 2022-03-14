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
/// \file hadronic/Hadr02/src/DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class
//
//
/////////////////////////////////////////////////////////////////////////
//
// DetectorMessenger
//
// Created: 31.01.03 V.Ivanchenko
//
// Modified:
// 04.06.2006 Adoptation of hadr01 (V.Ivanchenko)
// 16.11.2006 Add beamCmd (V.Ivanchenko)
//
////////////////////////////////////////////////////////////////////////
//

#include "TObjArray.h"
#include "TObjString.h"
#include "ScoringUtility.hh"

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction *det)
    : G4UImessenger(),
      fDetector(det),
      fTestDir(0),
      fVerbCmd(0),
      fGeomSetupCmd(0),
      fBuildSamplerPlaneCmd(0),
      fScorePlaneParticlesCmd(0),
      fSlimOutputCmd(0)
{
  fTestDir = new G4UIdirectory("/testhadr/");
  fTestDir->SetGuidance(" Hadronic Extended Example.");

  fVerbCmd = new G4UIcmdWithAnInteger("/testhadr/Verbose", this);
  fVerbCmd->SetGuidance("Set verbose for ");
  fVerbCmd->SetParameterName("verb", false);
  fVerbCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fGeomSetupCmd = new G4UIcmdWithAnInteger("/testhadr/geometry", this);
  fGeomSetupCmd->SetGuidance("Select the geometry setup. Currently: 0=LEBC, 1=NA62TAX");
  fGeomSetupCmd->AvailableForStates(G4State_PreInit);
  fGeomSetupCmd->SetDefaultValue(0); //LEBC

  fTargetMaterialCmd = new G4UIcmdWithAString("/testhadr/tagetMaterial", this);
  fTargetMaterialCmd->SetGuidance("Material for metallic strip to shoot particles in");
  fTargetMaterialCmd->AvailableForStates(G4State_PreInit);
  fTargetMaterialCmd->SetDefaultValue("G4_Cu");

  fBuildSamplerPlaneCmd = new G4UIcmdWithABool("/testhadr/buildSampler", this);
  fBuildSamplerPlaneCmd->SetGuidance("Build (true) or not (false) a sampler plane after the NA62TAX");
  fBuildSamplerPlaneCmd->AvailableForStates(G4State_PreInit);
  fBuildSamplerPlaneCmd->SetDefaultValue(true); // build sampler plane by default

  fScorePlaneParticlesCmd = new G4UIcmdWithAString("/score/PlaneParticles", this);
  fScorePlaneParticlesCmd->SetGuidance(" Blank separated string with pdg ids of particles to be scored");
  fScorePlaneParticlesCmd->SetDefaultValue("13");

  fSlimOutputCmd = new G4UIcmdWithABool("/score/SlimOutput", this);
  fSlimOutputCmd->SetGuidance("If set to true, the scoring information in the scoring plane will be slimed");
  fSlimOutputCmd->AvailableForStates(G4State_PreInit);
  fSlimOutputCmd->SetDefaultValue(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fTestDir;
  delete fVerbCmd;
  delete fGeomSetupCmd;
  delete fTargetMaterialCmd;
  delete fBuildSamplerPlaneCmd;
  delete fScorePlaneParticlesCmd;
  delete fSlimOutputCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
  if (command == fGeomSetupCmd)
  {
    fDetector->SetGeomterySetup(fGeomSetupCmd->GetNewIntValue(newValue));
  }
  else if (command == fBuildSamplerPlaneCmd)
  {
    fDetector->SetBuildSamplerPlane(fBuildSamplerPlaneCmd->GetNewBoolValue(newValue));
  }
  else if (command == fScorePlaneParticlesCmd)
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

    fDetector->SetScoreParticles(pdgIds);
  }

  else if (command == fSlimOutputCmd)
  {
    ScoringUtility::GetInstance()->SetSlimOutput(fSlimOutputCmd->GetNewBoolValue(newValue));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
