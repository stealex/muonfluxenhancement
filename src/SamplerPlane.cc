#include "SamplerPlane.hh"

#include "TString.h"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"

SamplerPlane::SamplerPlane(G4LogicalVolume *worldVolume, G4double zPos) : 
fZposition(zPos)
{
    G4int pos = (G4int) (fZposition/CLHEP::mm);
    G4Material *material = 
      G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
    G4Box *solid = new G4Box(Form("Z%d_Box", pos), 4.0*CLHEP::m, 4.0*CLHEP::m, 0.01*CLHEP::um);
    G4LogicalVolume *logic = new G4LogicalVolume(solid, material, Form("Z%d_LV", pos));
    new G4PVPlacement(0,
                      G4ThreeVector(0.0, 0.0, zPos),
                      logic,
                      Form("Z%d_PV", pos),
                      worldVolume,
                      false,
                      0);
    
}
