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
/// \file hadronic/Hadr02/src/EventAction.cc
/// \brief Implementation of the EventAction class
//
//
/////////////////////////////////////////////////////////////////////////
//
// EventAction
//
// Created: 31.04.2006 V.Ivanchenko
//
// Modified:
// 04.06.2006 Adoptation of hadr01 (V.Ivanchenko)
//
////////////////////////////////////////////////////////////////////////
//

#include "EventAction.hh"
#include "G4Event.hh"
#include "EventActionMessenger.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4UImanager.hh"
#include "G4ios.hh"
#include <chrono>
#include <ctime>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4int EventAction::fEventID = 0;

EventAction::EventAction()
    : G4UserEventAction(),
      fEventMessenger(0),
      fSelectedEvents(),
      fNSelected(0),
      fDebugStarted(false)
{
  fEventMessenger = new EventActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

EventAction::~EventAction()
{
  delete fEventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void EventAction::BeginOfEventAction(const G4Event *evt)
{
  // New event
  G4int nEvt = evt->GetEventID();
  fEventID = nEvt;
  if (fNSelected > 0)
  {
    for (G4int i = 0; i < fNSelected; i++)
    {
      if (nEvt == fSelectedEvents[i])
      {
        G4UImanager::GetUIpointer()->ApplyCommand("/random/saveThisEvent");
        G4UImanager::GetUIpointer()->ApplyCommand("/tracking/verbose  2");
        fDebugStarted = true;
        break;
      }
    }
  }
  // Initialize user actions
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void EventAction::EndOfEventAction(const G4Event *evt)
{
  if (fDebugStarted)
  {
    G4UImanager::GetUIpointer()->ApplyCommand("/tracking/verbose  0");
    fDebugStarted = false;
  }

  bool isPrint = false;
  G4int evtId = evt->GetEventID();
  
  if ( (evtId < 10) || 
      (evtId < 100 && evtId%10 ==0) ||
      (evtId < 1000 && evtId%100 ==0) ||
      (evtId < 10000 && evtId%1000 ==0) ||
      (evtId < 100000 && evtId%10000==0) ||
      (evtId < 1000000 && evtId%100000==0) ||
      (evtId%1000000==0) ) isPrint = true;
  if (!isPrint) return;

  auto eventEndSystemTime = std::chrono::system_clock::now();
  std::time_t eventEndRealTime = std::chrono::system_clock::to_time_t(eventEndSystemTime);
  char *timeNoNewLine = std::ctime(&eventEndRealTime);
  if (timeNoNewLine[strlen(timeNoNewLine)-1] == '\n') timeNoNewLine[strlen(timeNoNewLine)-1] = '\0';
  
  G4cout << "["<< timeNoNewLine<<"] " <<"  "<<"Event " << evtId << " processed " << G4endl; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void EventAction::AddEventToDebug(G4int val)
{
  fSelectedEvents.push_back(val);
  fNSelected++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
