#ifndef SAMPLERPLANE_HH
#define SAMPLERPLANE_HH

#include "G4Box.hh"
#include "G4LogicalVolume.hh"

#include "TTree.h"
#include "TNtuple.h"

class SamplerPlane{
    public:
        SamplerPlane(G4LogicalVolume *worldLV, G4double zPos);
        virtual ~SamplerPlane() {};

    private:
        G4int fZposition;
};

#endif