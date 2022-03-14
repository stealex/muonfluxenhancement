#include "BiasingUtility.hh"
#include "SamplerPlane.hh"
#include "SamplerPlaneSD.hh"

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"

#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"

#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4GDMLParser.hh"

#include "BOptrMultiParticleChangeInelastic.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),  fLogicWorld(0), fGeometrySetup(0), fBuildSamplerPlane(false),
    fDetectorMessenger(0), fSamplerZpos(0), fPrimaryGenerator(0), fMultiChangeInelastic(nullptr),
    fStepLimit(nullptr)

{
  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction() {
  delete fDetectorMessenger;
  fSamplerZpos.clear();
  if (fMultiChangeInelastic)
    delete fMultiChangeInelastic;

  if (fStepLimit)
    delete fStepLimit;
}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4VPhysicalVolume *world = 0;

  if (fGeometrySetup == 0)
    world = BuildSimplifiedTAX();
  // // colors

  G4VisAttributes zero = G4VisAttributes::GetInvisible();
  fLogicWorld->SetVisAttributes(zero);

  return world;
}

void DetectorConstruction::ConstructSDandField() {
  for (std::size_t iSampler = 0; iSampler < fSamplerZpos.size(); iSampler++) {
    G4int zPos = (G4int)(fSamplerZpos[iSampler] / CLHEP::mm);
    SamplerPlaneSD *samplerSD = new SamplerPlaneSD(Form("Z%d", zPos), Form("Z%dCollection", zPos));
    G4SDManager::GetSDMpointer()->AddNewDetector(samplerSD);
    SetSensitiveDetector(Form("Z%d_LV", zPos), samplerSD, false);
  }

  switch (BiasingUtility::GetInstance()->GetBiasMethod()) {
  case 1:
    AddBiasMuonFlux();
    break;
  default:
    break;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetGeomterySetup(G4int val) {
  if (val == 0) {
    fGeometrySetup = val;
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
  }
}

void DetectorConstruction::SetBuildSamplerPlane(G4bool val) {
  fBuildSamplerPlane = val;
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

G4VPhysicalVolume *DetectorConstruction::BuildSimplifiedTAX() {
  G4cout << "Building simplified version of NA62 TAX" << G4endl;
  G4Box *solidW = new G4Box("World", 5.0 * m, 5.0 * m, 10.0 * m);
  fLogicWorld = new G4LogicalVolume(solidW, G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic"), "World");
  G4VPhysicalVolume *world =
    new G4PVPlacement(0, G4ThreeVector(), fLogicWorld, "World", 0, false, 0);

  G4double halfX = 0.39 * CLHEP::m;
  G4double halfY = 0.6 * CLHEP::m;
  G4double halfZ = (0.415 / 2.) * CLHEP::m;

  G4int nCopperBlocks = 2;
  G4int nIronBlocks = 6;
  G4double safety = 0.1 * mm;

  G4Material *blockMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  G4int nPlaced = 0;
  for (G4int iBlock = 0; iBlock < nCopperBlocks; iBlock++) {
    G4Box *solid = new G4Box("TAX_Block_" + std::to_string(nPlaced), halfX, halfY, halfZ);
    G4LogicalVolume *logic =
      new G4LogicalVolume(solid, blockMaterial, "TAX_Block_lv_" + std::to_string(nPlaced));
    new G4PVPlacement(0, G4ThreeVector(0., 0., halfZ + nPlaced * 2 * halfZ + safety), logic,
                      "TAX_Block_pv_" + std::to_string(nPlaced), fLogicWorld, false, 0);
    nPlaced++;
  }

  blockMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
  for (G4int iBlock = 0; iBlock < nIronBlocks; iBlock++) {
    G4Box *solid = new G4Box("TAX_Block_" + std::to_string(nPlaced), halfX, halfY, halfZ);
    G4LogicalVolume *logic =
      new G4LogicalVolume(solid, blockMaterial, "TAX_Block_lv_" + std::to_string(nPlaced));
    new G4PVPlacement(0, G4ThreeVector(0., 0., halfZ + nPlaced * 2 * halfZ + safety), logic,
                      "TAX_Block_pv_" + std::to_string(nPlaced), fLogicWorld, false, 0);
    nPlaced++;
  }

  fBeamMeanX = 0.;
  fBeamMeanY = 0.;
  fBeamMeanXp = 0.;
  fBeamMeanYp = 0.;
  fBeamSigmaX = 0.;
  fBeamSigmaY = 0.;
  fBeamSigmaXp = 0.;
  fBeamSigmaYp = 0.;
  fBeamZpos = 0.0;
  fShootRandom = false;

  fPrimaryGenerator->UpdateBeamConfiguration();
  G4cout << "Building sampler plane" << G4endl;
  G4double samplerPosition = (nIronBlocks + nCopperBlocks) * (2. * halfZ + safety);
  BiasingUtility::GetInstance()->SetDecayZMax(samplerPosition);
  new SamplerPlane(fLogicWorld, samplerPosition);
  fSamplerZpos.push_back(samplerPosition);

  return world;
}



void DetectorConstruction::SetNiceColors() {
  G4VisAttributes copperColor = G4VisAttributes(G4Color(0.722, 0.451, 0.2, 0.3));
  G4VisAttributes ironColor = G4VisAttributes(G4Color(0.631, 0.616, 0.58, 0.3));
  G4VisAttributes wolframColor = G4VisAttributes(G4Color::White());
  G4VisAttributes airColor = G4VisAttributes(G4Color(0.678, 0.847, 0.902, 0.03));

  std::size_t nVolumes = G4LogicalVolumeStore::GetInstance()->size();
  for (std::size_t iVol = 0; iVol < nVolumes; iVol++) {
    G4LogicalVolume *vol = G4LogicalVolumeStore::GetInstance()->at(iVol);
    if (vol->GetName().contains("layer")) {
      G4cout << "Volume " << vol->GetName().data() << " Material "
             << vol->GetMaterial()->GetName().data() << G4endl;
    }
    if (vol->GetMaterial()->GetName().contains("G4_Cu"))
      vol->SetVisAttributes(copperColor);
    if (vol->GetMaterial()->GetName().contains("G4_Fe"))
      vol->SetVisAttributes(ironColor);
    if (vol->GetMaterial()->GetName().contains("G4_Air"))
      vol->SetVisAttributes(airColor);
    if (vol->GetMaterial()->GetName().contains("G4_W"))
      vol->SetVisAttributes(wolframColor);
  }
}


void DetectorConstruction::AddBiasMuonFlux() {
  // Create instance of operator and add  particles.
  fMultiChangeInelastic = new BOptrMultiParticleChangeInelastic();
  fMultiChangeInelastic->AddParticle("pi+");
  fMultiChangeInelastic->AddParticle("pi-");
  fMultiChangeInelastic->AddParticle("kaon+");
  fMultiChangeInelastic->AddParticle("kaon-");
  fMultiChangeInelastic->AddParticle("kaon0L");
  fMultiChangeInelastic->AddParticle("gamma");
  fMultiChangeInelastic->AddParticle("mu+");
  fMultiChangeInelastic->AddParticle("mu-");
  
  // Loop over volumes and select the ones that with material != G4_Galactic.
  G4int nVols = G4LogicalVolumeStore::GetInstance()->size();
  fStepLimit = new G4UserLimits(10. * mm); // 100 um.

  G4cout << "----------------------------------------------------" << G4endl;
  G4cout << " Attaching biasing operator " << fMultiChangeInelastic->GetName()
         << " to logical volume(s): " << G4endl;
  for (G4int i = 0; i < nVols; i++) {
    G4LogicalVolume *volume = (*G4LogicalVolumeStore::GetInstance())[i];
    // G4Material *material = volume->GetMaterial();
    // if (material->GetName().compareTo("G4_Galactic") == 0)
    //   continue;
    fMultiChangeInelastic->AttachTo(volume);

    volume->SetUserLimits(fStepLimit);
    G4cout << "--- " << volume->GetName() << G4endl;
  }
  G4cout << "----------------------------------------------------" << G4endl;
}
