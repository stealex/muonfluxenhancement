#include "BiasingUtility.hh"

#include "G4Track.hh"
#include "TrackInformation.hh"
#include <iostream>

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4VProcess.hh"

#include "G4DecayTable.hh"
#include "G4VDecayChannel.hh"

BiasingUtility *BiasingUtility::fInstance = nullptr;

BiasingUtility *BiasingUtility::GetInstance()
{
  if (!fInstance)
  {
    fInstance = new BiasingUtility();
  }
  return fInstance;
}
BiasingUtility::BiasingUtility()
    : fBiasMethod(0) {}

BiasingUtility::~BiasingUtility()
{
}


void BiasingUtility::PrintParticle(const G4Track *track)
{
  TrackInformation *info = (TrackInformation *)track->GetUserInformation();
  G4ParticleDefinition *def = (G4ParticleDefinition *)track->GetParticleDefinition();
  G4cout << "PDGid = " << def->GetPDGEncoding() << ", trackType = " << info->GetTrackType()
         << ", isAllInel = " << info->GetIsAllowedInelastic() << ", weight = " << track->GetWeight()
         << G4endl;
}