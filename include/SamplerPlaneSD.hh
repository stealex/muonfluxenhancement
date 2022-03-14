#ifndef SAMPLERSD_HH
#define SAMPLERSD_HH 1

#include "ScoringUtility.hh"

#include "G4VSensitiveDetector.hh"
#include "SamplerPlaneHit.hh"


#include <vector>

#include "TTree.h"
#include "TNtuple.h"
#include "TString.h"

class G4step;
class G4HCofThisEvent;

class SamplerPlaneSD : public G4VSensitiveDetector
{
public:
    SamplerPlaneSD(const G4String &name,
                   const G4String &hitsCollectionName);
    virtual ~SamplerPlaneSD();

    virtual void Initialize(G4HCofThisEvent *hitCollection);
    virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory *history);
    virtual void EndOfEvent(G4HCofThisEvent *hitCollection);

    void ScoreParticle(G4Track *);

private:
    SamplerPlaneHitsCollection* fHitsCollection;
    TTree *fScoringTree;
    std::vector<G4int> fStoredTracks;
    ScoringUtility *fScoringUtility;

    std::vector<Double_t> fX, fY, fZ, fPx, fPy, fPz;
    std::vector<Double_t> fInitX, fInitY, fInitZ, fInitPx, fInitPy, fInitPz;
    std::vector<Double_t> fWeight;
    std::vector<Int_t>    fEventID;
    std::vector<Int_t>    fPDGid, fMotherPDGid;
    std::vector<Int_t>    fTrackType;
    std::vector<TString>  fCreatorProcess;
    Int_t eventID;
};

#endif