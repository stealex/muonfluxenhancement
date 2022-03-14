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
/// \file hadronic/Hadr02/include/DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
//
/////////////////////////////////////////////////////////////////////////
//
// DetectorConstruction
//
// Created: 31.01.2003 V.Ivanchenko
//
// Modified:
// 04.06.2006 Adoptation of hadr01 (V.Ivanchenko)
//
////////////////////////////////////////////////////////////////////////
// 

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include <vector>
#include "PrimaryGeneratorAction.hh"
#include "BOptrMultiParticleChangeInelastic.hh"
#include "G4UserLimits.hh"

class CheckVolumeSD;
class TargetSD;
class G4LogicalVolume;
class DetectorMessenger;
class PrimaryGeneratorAction;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:

  DetectorConstruction();
  virtual ~DetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  G4VPhysicalVolume *BuildSimplifiedTAX();
  
  void SetNiceColors();
public:
  void SetGeomterySetup(G4int val);
  void SetBuildSamplerPlane(G4bool val) ;

  inline void SetScoreParticles(std::vector<G4int> &val) {fScorePDGids = val;}
  inline void SetPrimaryGeneratror(PrimaryGeneratorAction *val) {fPrimaryGenerator = val;}

  G4double GetBeamMeanX()  {return fBeamMeanX;}
  G4double GetBeamMeanY()  {return fBeamMeanY;}
  G4double GetBeamMeanXp() {return fBeamMeanXp;}
  G4double GetBeamMeanYp() {return fBeamMeanYp;}
  G4double GetBeamSigmaX() {return fBeamSigmaX;}
  G4double GetBeamSigmaY() {return fBeamSigmaY;}
  G4double GetBeamSigmaXp(){return fBeamSigmaXp;}
  G4double GetBeamSigmaYp(){return fBeamSigmaYp;}
  G4double GetBeamZpos()   {return fBeamZpos;}
  G4double GetShootRandom(){return fShootRandom;}

  void AddBiasMuonFlux();
private:

  DetectorConstruction & operator=(const DetectorConstruction &right);
  DetectorConstruction(const DetectorConstruction&);

  G4LogicalVolume* fLogicWorld;

  G4int fGeometrySetup;
  G4bool fBuildSamplerPlane;
  DetectorMessenger* fDetectorMessenger;
  std::vector<G4int> fSamplerZpos;
  std::vector<G4int>  fScorePDGids;
  PrimaryGeneratorAction *fPrimaryGenerator;
  
  G4double fBeamMeanX;
  G4double fBeamMeanY;
  G4double fBeamMeanXp;
  G4double fBeamMeanYp;
  G4double fBeamSigmaX;
  G4double fBeamSigmaY;
  G4double fBeamSigmaXp;
  G4double fBeamSigmaYp;
  G4double fBeamZpos;
  G4bool fShootRandom;

  BOptrMultiParticleChangeInelastic *fMultiChangeInelastic;
  G4UserLimits *fStepLimit;

};


#endif

