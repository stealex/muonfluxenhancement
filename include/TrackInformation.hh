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
/// \file optical/OpNovice2/include/TrackInformation.hh
/// \brief Definition of the TrackInformation class
//
//
//

#ifndef TrackInformation_h
#define TrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class TrackInformation : public G4VUserTrackInformation 
{
public:
  TrackInformation();
  TrackInformation(const G4Track* aTrack);
  TrackInformation(const TrackInformation* aTrackInfo);
  virtual ~TrackInformation();
   
  inline void *operator new(size_t);
  inline void operator delete(void *aTrackInfo);

  TrackInformation& operator =(const TrackInformation& right);
  
  void SetSourceTrackInformation(const G4Track* aTrack);
  virtual void Print() const;

public:
  inline G4bool GetIsAllowedInelastic() const {return fIsAllowedInelastic;}
  inline void   SetIsAllowedInelastic(G4bool b) {fIsAllowedInelastic = b;}

  inline G4int GetMotherPDGid() const {return fMotherPDGid;}
  inline void SetMotherPDGid(G4int val) {fMotherPDGid = val;}

  inline G4int GetTrackType() const {return fTrackType;}
  inline void SetTrackType(G4int val) {fTrackType = val;}

  inline G4bool GetHasClone() const {return fHasClone;}
  inline void SetHasClone(G4bool val) {fHasClone = val;}

private:
  G4bool    fIsAllowedInelastic;
  G4int     fMotherPDGid;
  G4int     fTrackType; // 0 = analog; 1 = clone that must decay; 2 = clone that must survive
  G4bool    fHasClone;
};

extern G4ThreadLocal
 G4Allocator<TrackInformation> * aTrackInformationAllocator;

inline void* TrackInformation::operator new(size_t)
{
  if(!aTrackInformationAllocator)
    aTrackInformationAllocator = new G4Allocator<TrackInformation>;
  return (void*)aTrackInformationAllocator->MallocSingle();
}

inline void TrackInformation::operator delete(void *aTrackInfo)
{ aTrackInformationAllocator->FreeSingle((TrackInformation*)aTrackInfo);}

#endif

