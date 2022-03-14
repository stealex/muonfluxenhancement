#include "BiasingUtility.hh"

#include "ScoringUtility.hh"
#include "TMath.h"
#include "TDirectoryFile.h"
#include "SamplerPlaneSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "Rtypes.h"
#include "G4RunManager.hh"
#include "TrackInformation.hh"

SamplerPlaneSD::SamplerPlaneSD(const G4String &name,
                               const G4String &hitstCollectionName) : G4VSensitiveDetector(name),
                                                                      fHitsCollection(NULL),
                                                                      fStoredTracks(0),
                                                                      fX(), fY(), fZ(),
                                                                      fPx(), fPy(), fPz(),
                                                                      fInitX(), fInitY(), fInitZ(),
                                                                      fInitPx(), fInitPy(), fInitPz(),
                                                                      fWeight(),
                                                                      fEventID(),
                                                                      fPDGid(), fMotherPDGid(),
                                                                      fTrackType(), fCreatorProcess()
{
    collectionName.insert(hitstCollectionName);

    fScoringUtility = ScoringUtility::GetInstance();
    fScoringUtility->GetScoringDir()->cd();
    // if (fScoringUtility->GetSlimOutput())
    // {
    //     fScoringTree = new TNtuple(name.data(), name.data(),
    //                                "x:y:z:Px:Py:Pz:PDGid:Weight:TrackType");
    // }
    // else
    // {
    //     fScoringTree = new TNtuple(name.data(), name.data(),
    //                                "x:y:z:Px:Py:Pz:t:PDGid:Weight:InitX:InitY:InitZ:InitPx:InitPy:InitPz:InitT:EventID:MotherPDGid:TrackType");
    // }
    fScoringTree = new TTree(name.data(), name.data());

    fScoringTree->Branch("x", &fX);
    fScoringTree->Branch("y", &fY);
    fScoringTree->Branch("z", &fZ);

    fScoringTree->Branch("Px", &fPx);
    fScoringTree->Branch("Py", &fPy);
    fScoringTree->Branch("Pz", &fPz);

    fScoringTree->Branch("Weight", &fWeight);
    fScoringTree->Branch("EventID", &fEventID);

    fScoringTree->Branch("PDGid", &fPDGid);

    fScoringTree->Branch("TrackType", &fTrackType);

    if (!fScoringUtility->GetSlimOutput())
    {
        fScoringTree->Branch("InitX", &fInitX);
        fScoringTree->Branch("InitY", &fInitY);
        fScoringTree->Branch("InitZ", &fInitZ);

        fScoringTree->Branch("InitPx", &fInitPx);
        fScoringTree->Branch("InitPy", &fInitPy);
        fScoringTree->Branch("InitPz", &fInitPz);

        fScoringTree->Branch("MotherPDGid", &fMotherPDGid);
        fScoringTree->Branch("CreatorProcess", &fCreatorProcess);
    }
    ScoringUtility::GetInstance()->AddPlaneScoringTree(fScoringTree);
    eventID = 0;
}

SamplerPlaneSD::~SamplerPlaneSD()
{
    fStoredTracks.clear();

    fX.clear();
    fY.clear();
    fZ.clear();
    fPx.clear();
    fPy.clear();
    fPz.clear();
    fInitX.clear();
    fInitY.clear();
    fInitZ.clear();
    fInitPx.clear();
    fInitPy.clear();
    fInitPz.clear();
    fWeight.clear();
    fEventID.clear();
    fPDGid.clear();
    fMotherPDGid.clear();
    fTrackType.clear();
    fCreatorProcess.clear();
}

void SamplerPlaneSD::Initialize(G4HCofThisEvent *hce)
{
    fHitsCollection = new SamplerPlaneHitsCollection(SensitiveDetectorName, collectionName[0]);

    G4int hcID =
        G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool SamplerPlaneSD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
    G4Track *track = step->GetTrack();
    
    ScoreParticle(track);

    return true;
}

void SamplerPlaneSD::ScoreParticle(G4Track *track)
{
    fStoredTracks.push_back(track->GetTrackID());
    G4int motherPDGid = ((TrackInformation *)track->GetUserInformation())->GetMotherPDGid();
    G4int pdgID = track->GetParticleDefinition()->GetPDGEncoding();
    G4double mass = track->GetParticleDefinition()->GetPDGMass();
    G4ThreeVector position = track->GetPosition();
    G4ThreeVector momentum = track->GetMomentum();
    G4ThreeVector initialPosition = track->GetVertexPosition();
    G4ThreeVector initialMomDir = track->GetVertexMomentumDirection();
    G4double initialEnergy = track->GetVertexKineticEnergy() + mass;
    G4double initialMomentumMag = sqrt(initialEnergy * initialEnergy - mass * mass);
    G4ThreeVector initialMomentum = initialMomentumMag * initialMomDir;
    G4double weight = track->GetWeight();
    Int_t TrackType = ((TrackInformation *)track->GetUserInformation())->GetTrackType();

    fX.push_back(position.x()/ CLHEP::mm);
    fY.push_back(position.y()/ CLHEP::mm);
    fZ.push_back(position.z()/ CLHEP::mm);

    fPx.push_back(momentum.x()/ CLHEP::MeV);
    fPy.push_back(momentum.y()/ CLHEP::MeV);
    fPz.push_back(momentum.z()/ CLHEP::MeV);

    fInitX.push_back(initialPosition.x()/CLHEP::mm);
    fInitY.push_back(initialPosition.y()/CLHEP::mm);
    fInitZ.push_back(initialPosition.z()/CLHEP::mm);

    fInitPx.push_back(initialMomentum.x()/CLHEP::MeV);
    fInitPy.push_back(initialMomentum.y()/CLHEP::MeV);
    fInitPz.push_back(initialMomentum.z()/CLHEP::MeV);

    fWeight.push_back(weight);
    fEventID.push_back(eventID);

    fPDGid.push_back(pdgID);
    fMotherPDGid.push_back(motherPDGid);

    fTrackType.push_back(TrackType);
    if (track->GetCreatorProcess()){
        fCreatorProcess.push_back(track->GetCreatorProcess()->GetProcessName().data());
    }
    else fCreatorProcess.push_back("Unkown");
}

void SamplerPlaneSD::EndOfEvent(G4HCofThisEvent * /**/)
{
    fStoredTracks.clear();
    
    fScoringUtility->GetScoringDir()->cd();
    fScoringTree->Fill();
    
    fX.clear();
    fY.clear();
    fZ.clear();
    fPx.clear();
    fPy.clear();
    fPz.clear();
    fInitX.clear();
    fInitY.clear();
    fInitZ.clear();
    fInitPx.clear();
    fInitPy.clear();
    fInitPz.clear();
    fWeight.clear();
    fEventID.clear();
    fPDGid.clear();
    fMotherPDGid.clear();
    fTrackType.clear();
    fCreatorProcess.clear();
}