#ifndef TRACKINGACTION_HH
#define TRACKINGACTION_HH

#include "G4UserTrackingAction.hh"
#include "G4LorentzConvertor.hh"
#include "TrackingMessenger.hh"

class G4Track;
class TNtuple;
class ScoredParticle;

class TrackingAction : public G4UserTrackingAction
{

public:
    TrackingAction();
    ~TrackingAction();

    void PreUserTrackingAction(const G4Track *);
    void PostUserTrackingAction(const G4Track *);

    inline void SetScoreParticles(std::vector<int> &val) { fScorePDGids = val; }
    inline void SetScoreParticleMinMomentum(G4double val) {fScoreParticleMinMomentum = val;}
private:
    G4LorentzVector fInitialMomentum;

    TrackingMessenger *fTrackingMessenger;
    TNtuple *fScoringTree;
    // TTree *fScoringTree;
    std::vector<int> fScorePDGids;
    G4double fScoreParticleMinMomentum;
    
    ScoredParticle *fScoredParticle;
};

#endif