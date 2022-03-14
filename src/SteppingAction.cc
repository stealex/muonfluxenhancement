
#include "ScoringUtility.hh"

#include "SteppingAction.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "TrackInformation.hh"

SteppingAction::SteppingAction() : G4UserSteppingAction()
{
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step *step)
{
    G4bool print = false;
    if (print)
    {
        G4Track *track = step->GetTrack();
        if (track->GetWeight() < 1.E-10)
        {
            TrackInformation *info = (TrackInformation *)track->GetUserInformation();
            G4cout << " TrackID: " << track->GetTrackID()
                   << " Energy: " << track->GetTotalEnergy()
                   << " Weight: " << track->GetWeight()
                   << " MPDGid: " << info->GetMotherPDGid()
                   << " Step length: " << step->GetStepLength()
                   << G4endl;
        }
    }
    return;
}