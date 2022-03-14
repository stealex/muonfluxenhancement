#ifndef BOptrChangeInelastic_hh
#define BOptrChangeInelastic_hh 1

#include "G4VBiasingOperator.hh"
#include "G4BOptnChangeCrossSection.hh"
#include "G4BOptnForceFreeFlight.hh"

class BOptnSplitAtCreation;
class G4ParticleDefinition;

#include <map>

class BOptrChangeInelastic : public G4VBiasingOperator {
public:
  // ------------------------------------------------------------
  // -- Constructor: takes the name of the particle type to bias:
  // ------------------------------------------------------------
  BOptrChangeInelastic(G4String particleToBias, G4String name = "ChangeInelastic");
  virtual ~BOptrChangeInelastic();
  
  // -- method called at beginning of run:
  virtual void StartRun();

// public:
//   G4VBiasingOperation *ProposeForMuonFlux(const G4Track*, const G4BiasingProcessInterface*);
  // G4VBiasingOperation *ProposeForMesonPropagation(const G4Track*, const G4BiasingProcessInterface*);

private:
  // -----------------------------
  // -- Mandatory from base class:
  // -----------------------------
  // -- This method returns the biasing operation that will bias the physics process occurence.
  virtual G4VBiasingOperation*
  ProposeOccurenceBiasingOperation(const G4Track*                            track,
                                   const G4BiasingProcessInterface* callingProcess);
  
  virtual G4VBiasingOperation*
  ProposeFinalStateBiasingOperation(const G4Track*, const G4BiasingProcessInterface*);
  // -- Methods not used:
  virtual G4VBiasingOperation*
  ProposeNonPhysicsBiasingOperation(const G4Track*, const G4BiasingProcessInterface*);

private:
  // -- ("using" to avoid compiler complaining against (false) method shadowing.)
  using G4VBiasingOperator::OperationApplied;

  // -- Optionnal base class method implementation.
  // -- This method is called to inform the operator that a proposed operation has been applied.
  // -- In the present case, it means that a physical interaction occured (interaction at
  // -- PostStepDoIt level):
  virtual void OperationApplied( const G4BiasingProcessInterface*                callingProcess,
                                 G4BiasingAppliedCase                               biasingCase,
                                 G4VBiasingOperation*                 occurenceOperationApplied,
                                 G4double                         weightForOccurenceInteraction,
                                 G4VBiasingOperation*                finalStateOperationApplied, 
                                 const G4VParticleChange*                particleChangeProduced );
  
private:
  
  G4bool fSetup; ///< Flag to entern to setup phase if a new particle is requested
  G4ParticleDefinition *fParticleToBias; ///< Definition of the biased particle.
  std::map< const G4BiasingProcessInterface*, 
            G4BOptnChangeCrossSection*       > fChangeCrossSectionOperations; ///< List of associations between processes and biasing operations:
  std::map< const G4BiasingProcessInterface*,
            G4BOptnForceFreeFlight*> fForceFreeFlightOperations;
  BOptnSplitAtCreation *fSplitAtCreationOperation;

  G4double fXSFactor;
};

#endif
