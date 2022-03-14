#include "SamplerPlaneHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"

#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<SamplerPlaneHit>* SamplerPlaneHitAllocator=0;

SamplerPlaneHit::SamplerPlaneHit() : G4VHit(),
fTrackID(0),
fEventID(0),
fPDGid(0),
fWeight(0.),
fPosition(G4ThreeVector(-999.)),
fMomentum(G4ThreeVector(-999.)),
fInitialPosition(G4ThreeVector(-999.)),
fInitialMometum(G4ThreeVector(-999.))
{}

SamplerPlaneHit::~SamplerPlaneHit(){}

SamplerPlaneHit::SamplerPlaneHit(const SamplerPlaneHit &right):
G4VHit(){
    fTrackID = right.fTrackID;
    fEventID = right.fEventID;
    fPDGid = right.fPDGid;
    fWeight = right.fWeight;
    fPosition = right.fPosition;
    fMomentum = right.fMomentum;
    fInitialPosition = right.fInitialPosition;
    fInitialMometum = right.fInitialMometum;
}

const SamplerPlaneHit& SamplerPlaneHit::operator=(const SamplerPlaneHit &right){
    fTrackID = right.fTrackID;
    fEventID = right.fEventID;
    fPDGid = right.fPDGid;
    fWeight = right.fWeight;
    fPosition = right.fPosition;
    fMomentum = right.fMomentum;
    fInitialPosition = right.fInitialPosition;
    fInitialMometum = right.fInitialMometum;

    return *this;
}

G4bool SamplerPlaneHit::operator==(const SamplerPlaneHit &right) const
{
    return (this==&right) ? true : false;
}

void SamplerPlaneHit::Draw(){
    G4VVisManager *pVVisManager = G4VVisManager::GetConcreteInstance();
    if (pVVisManager){
        G4Circle circle(fPosition);
        circle.SetScreenSize(4.);
        circle.SetFillStyle(G4Circle::filled);
        G4Colour colour(1., 0., 0.);
        G4VisAttributes attribs(colour);
        pVVisManager->Draw(circle);
    }
}

void SamplerPlaneHit::Print(){
    G4cout << "Hit" << G4endl;
}