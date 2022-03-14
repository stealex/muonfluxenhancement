#include "BOptnSplitAtCreation.hh"
#include "TrackInformation.hh"

#include "BiasingUtility.hh"

BOptnSplitAtCreation::BOptnSplitAtCreation(G4String name)
    : G4VBiasingOperation(name), fParticleChange(), fParticleChangeForNothing() {}

BOptnSplitAtCreation::~BOptnSplitAtCreation() {}

G4double BOptnSplitAtCreation::DistanceToApplyOperation(const G4Track *track, G4double,
                                                        G4ForceCondition *condition)
{
  TrackInformation *info = (TrackInformation *)track->GetUserInformation();

  // Assume that we clone nothing
  *condition = NotForced;

  // Algorithm:
  // 1. If Track is an original (meson or gamma) and does not have a clone, clone it.
  // 2. If track is a clone (meson or gamma) and its weight drops under 1E-4, play russian roullette
  // 3. If track is a clone (muon) and  its weight is above 1E-3, split it.
  G4int pdgID = track->GetParticleDefinition()->GetPDGEncoding();
  if (info->GetTrackType() == 0)
  {
    // apply biasing only if meson or gamma and we did not already clone
    if (abs(pdgID) != 13 && !info->GetHasClone())
      *condition = Forced;
  }
  else
  {
    // here we have a clone.
    // apply split/RR if muon is outside window
    if (abs(pdgID) == 13 && (track->GetWeight() < 1.E-4 || track->GetWeight() > 1.E-3))
      *condition = Forced;
    // apply RR if meson/gamma is bellow weight threshold
    if (abs(pdgID) != 13 && track->GetWeight() < 1.E-4)
      *condition = Forced;
  }

  return DBL_MAX;
}

G4VParticleChange *BOptnSplitAtCreation::GenerateBiasingFinalState(const G4Track *track, const G4Step *)
{
  // check if this is the first step performed by the original track
  // check if track already has clone
  TrackInformation *info = (TrackInformation *)track->GetUserInformation();
  G4int pdgID = track->GetParticleDefinition()->GetPDGEncoding();
  G4double weight = track->GetWeight();

  if (info->GetTrackType() == 0 && !info->GetHasClone() && track->GetCurrentStepNumber() == 1)
  {
    SplitMesonAtCreation(track);
  }
  else if (info->GetTrackType() == 1 && abs(pdgID) != 13 && weight < 1.E-4)
  {
    // 2. If track is a clone (meson or gamma) and its weight drops under 1E-4, play russian roullette
    PlayRussianRoulette(track);
  }
  else if (info->GetTrackType() == 1 && abs(pdgID) == 13)
  {
    // 3. If track is a clone (muon) and  its weight is above 1E-3, split it.
    if (weight > 1.E-3)
      SplitHighWeightMuon(track);
    else if (weight < 1.E-4)
      PlayRussianRoulette(track);
  }
  else
  {
    fParticleChangeForNothing.Initialize(*track);
    return &fParticleChangeForNothing;
  }
  return &fParticleChange;
}

void BOptnSplitAtCreation::SplitMesonAtCreation(const G4Track *track)
{
  fParticleChange.Initialize(*track);
  fParticleChange.SetSecondaryWeightByProcess(true);
  fParticleChange.SetNumberOfSecondaries(1);

  TrackInformation *infoOriginal = (TrackInformation *)track->GetUserInformation();

  G4ThreeVector vertexPos = track->GetVertexPosition();
  G4ThreeVector vertexMomDir = track->GetVertexMomentumDirection();
  G4DynamicParticle *dyn = new G4DynamicParticle(track->GetParticleDefinition(),
                                                 vertexMomDir, track->GetVertexKineticEnergy());
  fParticleChange.AddSecondary(new G4Track(dyn, track->GetGlobalTime(), vertexPos));

  G4Track *clone = fParticleChange.GetSecondary(0);
  TrackInformation *infoClone = new TrackInformation();
  infoClone->SetMotherPDGid(infoOriginal->GetMotherPDGid());
  infoClone->SetIsAllowedInelastic(false);
  infoClone->SetHasClone(true);
  infoClone->SetTrackType(1);

  clone->SetUserInformation(infoClone);

  infoOriginal->SetHasClone(true);
}

void BOptnSplitAtCreation::SplitHighWeightMuon(const G4Track *track)
{
  fParticleChange.Initialize(*track);

  TrackInformation *infoOriginal = (TrackInformation *)track->GetUserInformation();

  G4double weight = track->GetWeight();
  G4int nClones = (int)(weight / (5.E-4));
  G4double finalWeight = weight / nClones;

  fParticleChange.ProposeParentWeight(finalWeight);
  fParticleChange.SetNumberOfSecondaries(nClones);

  G4ThreeVector vertexPos = track->GetVertexPosition();
  G4ThreeVector vertexMomDir = track->GetVertexMomentumDirection();

  for (G4int iClone = 1; iClone < nClones; iClone++)
  {
    G4DynamicParticle *dyn = new G4DynamicParticle(track->GetParticleDefinition(),
                                                   vertexMomDir, track->GetVertexKineticEnergy());

    fParticleChange.AddSecondary(new G4Track(dyn, track->GetGlobalTime(), vertexPos));

    G4Track *clone = fParticleChange.GetSecondary(iClone - 1);

    clone->SetWeight(finalWeight);

    TrackInformation *infoClone = new TrackInformation();
    infoClone->SetMotherPDGid(infoOriginal->GetMotherPDGid());
    infoClone->SetIsAllowedInelastic(false);
    infoClone->SetHasClone(true);
    infoClone->SetTrackType(1);

    clone->SetUserInformation(infoClone);
  }
  fParticleChange.SetSecondaryWeightByProcess(true);
}

#include "G4RandomTools.hh"

void BOptnSplitAtCreation::PlayRussianRoulette(const G4Track *track)
{
  fParticleChange.Initialize(*track);
  fParticleChange.SetSecondaryWeightByProcess(true);
  // fParticleChange.SetNumberOfSecondaries(0);

  G4double weight = track->GetWeight();
  G4double killingProb = 1.0 - weight / 5.E-4;

  if (G4RandFlat::shoot() < killingProb)
  {
    fParticleChange.ProposeTrackStatus(fKillTrackAndSecondaries);
  }
  else
  {
    fParticleChange.ProposeParentWeight(5.E-4);
  }
}