#include "BOptrChangeInelastic.hh"
#include "BOptnSplitAtCreation.hh"
#include "G4BiasingProcessInterface.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4VProcess.hh"

#include "Randomize.hh"

#include "CLHEP/Units/SystemOfUnits.h"
#include "G4HadronicProcessStore.hh"
#include "G4InteractionLawPhysical.hh"
#include "G4ProcessManager.hh"
#include "G4UnitsTable.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4NavigationHistory.hh"
#include "G4StepPoint.hh"
#include "G4TouchableHandle.hh"
#include "G4VSolid.hh"

#include "TrackInformation.hh"
#include "BiasingUtility.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/// \class BOptrChangeInelastic
/// \Brief
/// Implementetion of G4VBiasingOperator to change cross section of a process.\n
/// Also biases the final state of the inelastic process.
/// \EndBrief
///
/// \Detailed
/// The occurance of the Kaon+ inelastic process is modified (biased)\n
/// by updating its cross-section at each step by a factor depending on\n
/// the distance to back of GTK3 non-void volumes.\n
/// It also forwards a call to BOptnForceFinalState to bias also the\n
/// final state of this
/// for details see:
/// http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForToolkitDeveloper/html/GuideToExtendFunctionality/EventBiasing/eventBiasing.html#physics-process-occurrence-biasing
///
/// \EndDetailed

BOptrChangeInelastic::BOptrChangeInelastic(G4String particleName, G4String name)
    : G4VBiasingOperator(name), fSetup(true), fChangeCrossSectionOperations()
{
  /// \MemberDescr
  /// \param particleName: G4String containing the name of the particle to bias.
  /// \param name: G4String containing the name to be assigned to this operator
  ///
  /// Constructor.\n
  /// Retireves the particle definition of particle with name particleName\n
  /// and creates a final state biasing operation for this particle type\n
  /// If the definition is not found, a warning is issued and no biasing occurs.
  /// \EndMemberDescr

  fParticleToBias = G4ParticleTable::GetParticleTable()->FindParticle(particleName);
  if (fParticleToBias == 0)
  {
    G4ExceptionDescription ed;
    ed << "Particle `" << particleName << "' not found !" << G4endl;
    G4Exception("BOptrChangeInelastic(...)", "exGB01.01", JustWarning, ed);
  }
  fSplitAtCreationOperation = new BOptnSplitAtCreation("splitAtCreation");
  G4cout << "Created split-at-creation operation for " << fParticleToBias->GetParticleName().data()
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BOptrChangeInelastic::~BOptrChangeInelastic()
{
  /// \MemberDescr
  /// Destructor\n
  /// Deletes all of the biasing operations created
  /// \EndMemberDescr
  for (auto op : fChangeCrossSectionOperations)
  {
    delete op.second;
  }

  delete fSplitAtCreationOperation;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BOptrChangeInelastic::StartRun()
{
  /// \MemberDescr
  ///
  /// Setup stage.\n
  /// Start by collecting processes under biasing, create needed biasing
  /// operations and associate these operations to the processes
  /// \EndMemberDescr

  if (fSetup)
  {

    const G4ProcessManager *processManager = fParticleToBias->GetProcessManager();
    const G4BiasingProcessSharedData *sharedData =
        G4BiasingProcessInterface::GetSharedData(processManager);
    if (sharedData) // -- sharedData tested, as is can happen a user attaches an operator to a
                    // -- volume but without defined BiasingProcessInterface processes.
    {
      for (size_t i = 0; i < (sharedData->GetPhysicsBiasingProcessInterfaces()).size(); i++)
      {
        const G4BiasingProcessInterface *wrapperProcess =
            (sharedData->GetPhysicsBiasingProcessInterfaces())[i];

        G4String operationName;
        if (BiasingUtility::GetInstance()->GetBiasMethod())
        {
          operationName = "XSchange-" + wrapperProcess->GetWrappedProcess()->GetProcessName();
          fChangeCrossSectionOperations[wrapperProcess] =
              new G4BOptnChangeCrossSection(operationName);
          G4cout << "Created crossSection operator for " << wrapperProcess->GetProcessName().data()
                 << ", of particle " << fParticleToBias->GetParticleName().data() << G4endl;
        }
      }
    }
    fSetup = false;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VBiasingOperation *BOptrChangeInelastic::ProposeOccurenceBiasingOperation(
    const G4Track *track, const G4BiasingProcessInterface *callingProcess)
{
  G4int trackPDGid = track->GetParticleDefinition()->GetPDGEncoding();
  TrackInformation *trackInfo = (TrackInformation *)track->GetUserInformation();

  if (trackInfo->GetTrackType() == 0)
    return 0;
  if (BiasingUtility::GetInstance()->GetBiasMethod() &&
      (trackInfo->GetTrackType() == 0 ||
       trackInfo->GetIsAllowedInelastic())) // analog track or photon
    return 0;

  G4ProcessType procType = callingProcess->GetWrappedProcess()->GetProcessType();
  G4String procName = callingProcess->GetWrappedProcess()->GetProcessName();
  if (BiasingUtility::GetInstance()->GetBiasMethod() && procType != fHadronic &&
      procType != fDecay && trackPDGid != 22)
    return 0;

  if (trackPDGid == 22)
  {
    G4VPhysicalVolume *vol = track->GetVolume();
    if (vol)
    {
      if (vol->GetLogicalVolume()->GetMaterial()->GetName().contains("G4_Galactic"))
        return 0;
    }
  }

  G4BOptnChangeCrossSection *operation = fChangeCrossSectionOperations[callingProcess];
  if (!operation)
    return 0;

  G4double analogInteractionLength =
      callingProcess->GetWrappedProcess()->GetCurrentInteractionLength() / CLHEP::mm;
  //   // ---------------------------------------------------------------------
  //   // -- select and setup the biasing operation for current callingProcess:
  //   // ---------------------------------------------------------------------
  //   // -- Check if the analog cross-section well defined : for example, the conversion
  //   // -- process for a gamma below e+e- creation threshold has an DBL_MAX interaction
  //   // -- length. Nothing is done in this case (ie, let analog process to deal with the case)
  if (analogInteractionLength > DBL_MAX / 10.)
    return 0;
  if (analogInteractionLength < 0.)
    return 0;

  // Retrieve analog cross-section
  G4double analogXS = 1. / (analogInteractionLength / CLHEP::mm);
  G4double XStransformation = 1.0;

  // G4cout << "Doing This" << G4endl;
  // clone that must create muons and calling process is either Decay or Gamma->mumu
  if (procType == fHadronic || procName.compareTo("conv") == 0 ||
      procName.compareTo("photonNuclear") == 0)
  {
    // for the clones we always want them not to do hadronic, conversion or photonNuclear
    XStransformation = 0.;
  }
  else
  {
    G4StepPoint *pointPre = track->GetStep()->GetPreStepPoint();

    G4ThreeVector worldPosition = pointPre->GetPosition();
    G4ThreeVector worldMomDir = pointPre->GetMomentumDirection();
    G4double dz = BiasingUtility::GetInstance()->GetDecayZMax() / CLHEP::mm -
                  worldPosition.getZ() / CLHEP::mm;
    G4double dx = worldMomDir.getX() / worldMomDir.getZ() * dz;
    G4double dy = worldMomDir.getY() / worldMomDir.getZ() * dz;

    G4double distanceLeft = sqrt(dx * dx + dy * dy + dz * dz);
    XStransformation = 1. / (1.0 - std::exp(-analogXS * distanceLeft));
  }
  operation->SetBiasedCrossSection(XStransformation * analogXS);
  operation->Sample();
  return operation;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BOptrChangeInelastic::OperationApplied(const G4BiasingProcessInterface * /*callingProcess*/,
                                            G4BiasingAppliedCase,
                                            G4VBiasingOperation * /*occurenceOperationApplied*/,
                                            G4double, G4VBiasingOperation *,
                                            const G4VParticleChange *)
{
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VBiasingOperation *BOptrChangeInelastic::ProposeFinalStateBiasingOperation(
    const G4Track */*track*/, const G4BiasingProcessInterface */*callingProcess*/)
{
  return 0;
}

G4VBiasingOperation *BOptrChangeInelastic::ProposeNonPhysicsBiasingOperation(
    const G4Track */*track*/, const G4BiasingProcessInterface */*callingProcess*/)
{
  BOptnSplitAtCreation *operation = fSplitAtCreationOperation;
  if (operation)
  {
    return operation;
  }
  return 0;
}