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
/// \file hadronic/Hadr02/include/StackingAction.hh
/// \brief Definition of the StackingAction class
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

#ifndef StackingAction_h
#define StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"
#include "G4String.hh"


class StackingMessenger;
class G4Track;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class StackingAction : public G4UserStackingAction
{
public:

  StackingAction();
  virtual ~StackingAction();
   
  inline void SetKillAll(G4bool value)  {fKillAll = value;};
  inline void SetKillEM (G4bool value)  {fKillEM  = value;};
  inline void SetKillAfterMuonEmitted(G4bool value) {fKillAfterMuonEmitted = value;}
  inline void SetEnergyThreshold(G4double value) {fEnergyThreshold = value;}
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
  virtual void PrepareNewEvent();
  virtual void NewStage();

    
private:
  
  StackingMessenger*  fStackMessenger;

  G4bool              fKillAll;
  G4bool              fKillEM;
  G4bool              fKillAfterMuonEmitted;
  G4double            fEnergyThreshold;
  G4bool              fMuonExistsInEvent;

  std::map<G4int, G4ClassificationOfNewTrack> fTrackClassificationMap;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

