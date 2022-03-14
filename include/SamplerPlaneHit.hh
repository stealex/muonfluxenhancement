#ifndef SAMPLERPLANEHIT_HH
#define SAMPLERPLANEHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

class SamplerPlaneHit : public G4VHit
{
    public:
        SamplerPlaneHit();
        SamplerPlaneHit(const SamplerPlaneHit&);
        virtual ~SamplerPlaneHit();

        const SamplerPlaneHit& operator=(const SamplerPlaneHit&);
        G4bool operator==(const SamplerPlaneHit&) const;

        inline void* operator new(size_t);
        inline void operator delete(void*);

        virtual void Draw();
        virtual void Print();
        
        void SetTrackID(G4int val) {fTrackID = val;}
        void SetEventID(G4int val) {fEventID = val;}
        void SetPDGid(G4int val) {fPDGid = val;}
        void SetWeight(G4double val) {fWeight = val;}
        void SetPosition(G4ThreeVector val) {fPosition = val;}
        void SetMomentum(G4ThreeVector val) {fMomentum = val;}
        void SetInitialPosition(G4ThreeVector val) {fInitialPosition = val;}
        void SetInitialMometum(G4ThreeVector val) {fInitialMometum = val;}
        
        G4int GetTrackID() {return fTrackID;}
        G4int GetEventID() {return fEventID;}
        G4int GetPDGid() {return fPDGid;}
        G4double GetWeight() {return fWeight;}
        G4ThreeVector GetPosition() {return fPosition;}
        G4ThreeVector GetMomentum() {return fMomentum;}
        G4ThreeVector GetInitialPosition() {return fInitialPosition;}
        G4ThreeVector GetInitialMometum() {return fInitialMometum;}
    private:
        G4int           fTrackID;
        G4int           fEventID;
        G4int           fPDGid;
        G4double        fWeight;
        G4ThreeVector   fPosition;
        G4ThreeVector   fMomentum;
        G4ThreeVector   fInitialPosition;
        G4ThreeVector   fInitialMometum;
};

typedef G4THitsCollection<SamplerPlaneHit> SamplerPlaneHitsCollection;
extern G4ThreadLocal G4Allocator<SamplerPlaneHit> *SamplerPlaneHitAllocator;

inline void* SamplerPlaneHit::operator new(size_t)
{
    if (!SamplerPlaneHitAllocator)
        SamplerPlaneHitAllocator = new G4Allocator<SamplerPlaneHit>;
    return (void *) SamplerPlaneHitAllocator->MallocSingle();
}

inline void SamplerPlaneHit::operator delete(void *hit)
{
    SamplerPlaneHitAllocator->FreeSingle((SamplerPlaneHit*) hit);
}

#endif