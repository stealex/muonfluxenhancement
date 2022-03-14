#include "ScoringUtility.hh"
#include "TDirectoryFile.h" //because ROOT...
#include "TVectorD.h"

#include <iostream>
ScoringUtility::ScoringUtility()
  : fScoringFileName(""), fScoringFile(0), fScoringDir(0), fScoringTree(0), fSeed(0),
    fHistogramMap(), fdEdXHistos(0), fStudyMuons(false), fSlimOutput(false), fTargetLength(-1.) {}

ScoringUtility::~ScoringUtility() {}

void ScoringUtility::CloseOutputStructre() {
  fScoringFile->cd();
  if (fTargetLength > 0.) {
    TVectorD targetLength(1);
    targetLength[0] = fTargetLength;
    targetLength.Write("targetLength");
    std::cout << "Added target length to user info" << std::endl;
  }
  fScoringDir->cd();
  fScoringTree->Write();
  for (size_t iPlane = 0; iPlane < fPlaneScoringTrees.size(); iPlane++) {
    fPlaneScoringTrees[iPlane]->Write();
  }
  fScoringFile->cd();
  for (auto const &histo : fHistogramMap) {
    histo.second->Write();
  }
  fScoringFile->Close();
}

ScoringUtility *ScoringUtility::GetInstance() {
  if (!fInstance) {
    fInstance = new ScoringUtility();
    return fInstance;
  } else {
    return fInstance;
  }
}

void ScoringUtility::CreateOutputStructure() {
  fScoringFileName = Form("scoringVolume_%d.root", fSeed);

  fScoringFile = new TFile(fScoringFileName.Data(), "RECREATE");

  for (Int_t iBin = 0; iBin < 80; iBin++) {
    TString histoName = Form("dEdXmuPlus_%03d_%03dGeV", iBin * 5, (iBin + 1) * 5);
    std::vector<Double_t> xBinEdges(0);

    // Lower edge of histo
    xBinEdges.push_back(0.0);

    // upper edge of bins up to 1E-4*initial Kinetic energy
    xBinEdges.push_back(1E-9);
    xBinEdges.push_back(1E-8);
    xBinEdges.push_back(1E-7);
    xBinEdges.push_back(1E-6);
    xBinEdges.push_back(1E-5);
    xBinEdges.push_back(1E-4);

    // 10 equal bins up to 100 MeV = 0.1 GeV
    Double_t energy100Mev = 0.1 / ((iBin + 1.) * 5.);
    Double_t stepInit = (energy100Mev - xBinEdges.back()) / 10;

    for (Int_t i = 0; i < 10; i++)
      xBinEdges.push_back(xBinEdges.back() + stepInit);

    // Bins of 100 MeV = 0.1GeV

    while (xBinEdges.back() < 1.0) {
      xBinEdges.push_back(xBinEdges.back() + energy100Mev);
    }

    fdEdXHistos.push_back(
      new TH1D(histoName.Data(), histoName.Data(), (Int_t)xBinEdges.size() - 1, xBinEdges.data()));

    fHistogramMap.insert(std::pair<TString, TH1 *>(histoName, fdEdXHistos.back()));
  }
  fScoringDir = fScoringFile->mkdir("NTuple");
  fScoringDir->cd();

  fScoringTree = new TNtuple("TARGET", "TARGET",
                             "x:y:z:Px:Py:Pz:t:PDGid:Weight:InitX:InitY:InitZ:InitPx:InitPy:InitPz:"
                             "InitT:EventID:MotherPDGid");

  // fScoringDir = fScoringFile->mkdir("AllParticles");
  // fScoringTree = new TTree("ScoredParticles", "ScoredParticles");
}