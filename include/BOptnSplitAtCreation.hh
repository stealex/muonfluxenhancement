#ifndef BOPTNSPLITATCREATION_HH
#define BOPTNSPLITATCREATION_HH

#include "G4ParticleChange.hh"
#include "G4VParticleChange.hh"
#include "G4ParticleChangeForNothing.hh"
#include "G4VBiasingOperation.hh"
class G4LogicalVolume;

class BOptnSplitAtCreation : public G4VBiasingOperation {
public:
  BOptnSplitAtCreation(G4String name);
  ~BOptnSplitAtCreation();

public:
  // ----------------------------------------------
  // -- Methods from G4VBiasingOperation interface:
  // ----------------------------------------------
  // -- Unused:
  virtual const G4VBiasingInteractionLaw *
  ProvideOccurenceBiasingInteractionLaw(const G4BiasingProcessInterface *, G4ForceCondition &) {
    return 0;
  }
  virtual G4VParticleChange *ApplyFinalStateBiasing(const G4BiasingProcessInterface *,
                                                    const G4Track *, const G4Step *, G4bool &) {
    return 0;
  }

  // -- Used methods ("non-physics biasing methods"):
  // ------------------------------------------------
  // -- Method to return the distance or the condition under which
  // -- requesting the biasing. The "condition" flag will be indeed
  // -- used to apply the operation on the geometry boundary.
  virtual G4double DistanceToApplyOperation(const G4Track *, G4double, G4ForceCondition *condition);
  // -- Method the generate the final state, ie, either the final states
  // -- corresponding to the splitting or killing cases:
  virtual G4VParticleChange *GenerateBiasingFinalState(const G4Track *, const G4Step *);

  void SplitMesonAtCreation(const G4Track *track);
  void PlayRussianRoulette(const G4Track *track);
  void SplitHighWeightMuon(const G4Track *track);

private:
  G4VParticleChange fParticleChange;
  G4ParticleChangeForNothing fParticleChangeForNothing;
};

#endif