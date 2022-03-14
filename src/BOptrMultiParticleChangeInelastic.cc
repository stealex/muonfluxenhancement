#include "BOptrMultiParticleChangeInelastic.hh"
#include "G4BiasingProcessInterface.hh"

#include "BOptrChangeInelastic.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4SystemOfUnits.hh"

BOptrMultiParticleChangeInelastic::BOptrMultiParticleChangeInelastic()
  : G4VBiasingOperator("BOptrMultiParticleInelastic") {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BOptrMultiParticleChangeInelastic::AddParticle(G4String particleName) {
  const G4ParticleDefinition *particle =
    G4ParticleTable::GetParticleTable()->FindParticle(particleName);

  if (particle == 0) {
    G4ExceptionDescription ed;
    ed << "Particle `" << particleName << "' not found !" << G4endl;
    G4Exception("BOptrMultiParticleChangeXS::AddParticle(...)", "exGB01.02", JustWarning, ed);
    return;
  }
  // Create a new cross-section biasing operator for the given particle.
  BOptrChangeInelastic *optr = new BOptrChangeInelastic(particleName);
  fParticlesToBias.push_back(particle);
  fBOptrForParticle[particle] = optr;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VBiasingOperation *BOptrMultiParticleChangeInelastic::ProposeOccurenceBiasingOperation(
  const G4Track *track, const G4BiasingProcessInterface *callingProcess) {
  if (fCurrentOperator)
    return fCurrentOperator->GetProposedOccurenceBiasingOperation(track, callingProcess);
  else
    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BOptrMultiParticleChangeInelastic::StartTracking(const G4Track *track) {
  const G4ParticleDefinition *definition = track->GetParticleDefinition();
  std::map<const G4ParticleDefinition *, BOptrChangeInelastic *>::iterator it =
    fBOptrForParticle.find(definition);
  fCurrentOperator = 0;
  if (it != fBOptrForParticle.end()) {
    fCurrentOperator = (*it).second;
  }
  // -- reset count for number of biased interactions:
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BOptrMultiParticleChangeInelastic::OperationApplied(
  const G4BiasingProcessInterface *callingProcess, G4BiasingAppliedCase biasingCase,
  G4VBiasingOperation *occurenceOperationApplied, G4double weightForOccurenceInteraction,
  G4VBiasingOperation *finalStateOperationApplied,
  const G4VParticleChange *particleChangeProduced) {
  if (fCurrentOperator)
    fCurrentOperator->ReportOperationApplied(callingProcess, biasingCase, occurenceOperationApplied,
                                             weightForOccurenceInteraction,
                                             finalStateOperationApplied, particleChangeProduced);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VBiasingOperation *BOptrMultiParticleChangeInelastic::ProposeFinalStateBiasingOperation(
  const G4Track *track, const G4BiasingProcessInterface *callingProcess) {
  if (fCurrentOperator)
    return fCurrentOperator->GetProposedFinalStateBiasingOperation(track, callingProcess);
  return 0;
}

G4VBiasingOperation *BOptrMultiParticleChangeInelastic::ProposeNonPhysicsBiasingOperation(
  const G4Track *track, const G4BiasingProcessInterface *callingProcess) {
  if (fCurrentOperator)
    return fCurrentOperator->GetProposedNonPhysicsBiasingOperation(track, callingProcess);
  return 0;
}