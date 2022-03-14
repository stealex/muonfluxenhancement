#include "TrackingAction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"

#include "G4Track.hh"

#include "ScoringUtility.hh"
#include "TrackInformation.hh"

#include "TNtuple.h"
#include "TTree.h"
#include "ScoredParticle.hh"

#include "BiasingUtility.hh"

TrackingAction::TrackingAction() : G4UserTrackingAction(),
                                   fScoringTree(nullptr), fScorePDGids(0)
{
    fTrackingMessenger = new TrackingMessenger(this);
    fScoredParticle = new ScoredParticle();
}

TrackingAction::~TrackingAction()
{
    delete fTrackingMessenger;
}

void TrackingAction::PreUserTrackingAction(const G4Track *aTrack)
{
    TrackInformation *trackInfo =
        (TrackInformation *)(aTrack->GetUserInformation());
    if (!trackInfo)
    {
        trackInfo = new TrackInformation(aTrack);
        trackInfo->SetIsAllowedInelastic(true);
        trackInfo->SetMotherPDGid(0);
        trackInfo->SetTrackType(0); // analog by default
        aTrack->SetUserInformation(trackInfo);
    }

    fInitialMomentum = G4LorentzVector(aTrack->GetMomentum(), aTrack->GetTotalEnergy());
    if (!fScoringTree)
    {
        fScoringTree = ScoringUtility::GetInstance()->GetScoringTree();
        // fScoringTree->Branch("ScoredParticle", "ScoredParticle", &fScoredParticle);
    }
}

void TrackingAction::PostUserTrackingAction(const G4Track *aTrack)
{
    TrackInformation *info =
        (TrackInformation *)(aTrack->GetUserInformation());
    G4TrackVector *secondaries = fpTrackingManager->GimmeSecondaries();
    if (secondaries)
    {
        std::size_t nSeco = secondaries->size();
        if (nSeco > 0)
        {
            G4int pdgID = aTrack->GetParticleDefinition()->GetPDGEncoding();
            // G4cout << "Secondaries weights: ";
            for (size_t i = 0; i < nSeco; i++)
            {
                TrackInformation *secondaryInfo = (TrackInformation *)(*secondaries)[i]->GetUserInformation();
                if (!secondaryInfo)
                {
                    TrackInformation *infoNew = new TrackInformation();
                    infoNew->SetIsAllowedInelastic(true);
                    infoNew->SetMotherPDGid(pdgID);
                    infoNew->SetTrackType(info->GetTrackType());
                    infoNew->SetHasClone(false);
                    (*secondaries)[i]->SetUserInformation(infoNew);
                }
            }
        }
    }

    G4ThreeVector prodPos(aTrack->GetVertexPosition());
    G4ThreeVector endPos(aTrack->GetPosition());
    G4ThreeVector endMom(aTrack->GetMomentum());

    if (std::find(fScorePDGids.begin(), fScorePDGids.end(),
                  aTrack->GetParticleDefinition()->GetPDGEncoding()) ==
        fScorePDGids.end())
        return;
    if (fInitialMomentum.vect().mag() < fScoreParticleMinMomentum)
        return;

    fScoredParticle->SetProdPos(TVector3(prodPos.x(), prodPos.y(), prodPos.z()));
    fScoredParticle->SetProdMom(TVector3(endPos.x(), endPos.y(), endPos.z()));
    fScoredParticle->SetPDGcode(aTrack->GetParticleDefinition()->GetPDGEncoding());
    if (aTrack->GetCreatorProcess())
    {
        fScoredParticle->SetProdProcess(aTrack->GetCreatorProcess()->GetProcessName().data());
    }
    Float_t outputVar[18];

    outputVar[0] = endPos.x();
    outputVar[1] = endPos.y();
    outputVar[2] = endPos.z();
    outputVar[3] = endMom.x();
    outputVar[4] = endMom.y();
    outputVar[5] = endMom.z();
    outputVar[6] = 0.0; // t
    outputVar[7] = 1. * aTrack->GetParticleDefinition()->GetPDGEncoding();
    outputVar[8] = aTrack->GetWeight();
    outputVar[9] = prodPos.x();
    outputVar[10] = prodPos.y();
    outputVar[11] = prodPos.z();
    outputVar[12] = fInitialMomentum.vect().x();
    outputVar[13] = fInitialMomentum.vect().y();
    outputVar[14] = fInitialMomentum.vect().z();
    outputVar[15] = 1. * info->GetTrackType(); // InitT
    outputVar[16] = 1. * G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    outputVar[17] = 1. * info->GetMotherPDGid();

    fScoringTree->Fill(outputVar);
}