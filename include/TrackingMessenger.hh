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
/// \file hadronic/Hadr02/include/TrackingMessenger.hh
/// \brief Definition of the TrackingMessenger class
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

#ifndef TrackingMessenger_h
#define TrackingMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class TrackingAction;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TrackingMessenger: public G4UImessenger
{
public:

  TrackingMessenger(TrackingAction*);
  virtual ~TrackingMessenger();
    
  virtual void SetNewValue(G4UIcommand*, G4String);
    
private:
    
  TrackingAction*     fTrackAction;
  G4UIcmdWithAString* fScoreVolumeParticlesCmd;
  G4UIcmdWithADouble* fScoreVolumeParticlesMinMomentumCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
