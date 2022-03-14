#include "ScoringUtility.hh"
#include "BiasingMessenger.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "PhysicsListMessenger.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "StackingAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"

#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "G4HadronicParameters.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char **argv)
{

  //detect interactive mode (if no arguments) and define UI session
  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  
  G4UIExecutive *ui = nullptr;
  if (argc == 1)
  {
    ui = new G4UIExecutive(argc, argv);
    ScoringUtility::GetInstance()->SetSeed(1);
    ScoringUtility::GetInstance()->CreateOutputStructure();
    G4cout << "[Hadr01] given seed " << ScoringUtility::GetInstance()->GetSeed() << G4endl;
    long gps_seeds[2];
    int iSeed = 1;
    gps_seeds[0] = iSeed;
    gps_seeds[1] = iSeed + 62;
    G4cout << "[Hadr01]: current couple of seeds " << gps_seeds[0]
           << ", " << gps_seeds[1] << G4endl;
    // CLHEP::HepRandom::setTheSeeds(gps_seeds);
    CLHEP::HepRandom::setTheSeed(iSeed);
    ScoringUtility::GetInstance()->CreateOutputStructure();
  }
  if (argc == 3)
  {
    G4int iSeed = std::atoi(argv[2]);
    ScoringUtility::GetInstance()->SetSeed(iSeed);
    G4cout << "[Hadr01] given seed " << ScoringUtility::GetInstance()->GetSeed() << G4endl;
    long gps_seeds[2];
    gps_seeds[0] = iSeed;
    gps_seeds[1] = iSeed + 62;
    G4cout << "[Hadr01]: current couple of seeds " << gps_seeds[0]
           << ", " << gps_seeds[1] << G4endl;
    // CLHEP::HepRandom::setTheSeeds(gps_seeds);
    CLHEP::HepRandom::setTheSeed(iSeed);
    ScoringUtility::GetInstance()->CreateOutputStructure();
  }
  //Construct the default run manager
  G4RunManager *runManager = new G4RunManager();

  // Biasing messenger first
  BiasingMessenger::GetInstance();

  //set mandatory initialization classes
  DetectorConstruction *detector = new DetectorConstruction();
  runManager->SetUserInitialization(detector);
  G4PhysListFactory factory;
  G4VModularPhysicsList *phys = nullptr;
  PhysicsListMessenger *mess = nullptr;
  G4String physName = "";
  
  // Physics List name defined via environment variable
  if ("" == physName)
  {
    char *path = std::getenv("PHYSLIST");
    if (path)
    {
      physName = G4String(path);
      G4cout << "Physics List from environment: " << physName.data() << G4endl;
    }
  }

  //reference PhysicsList via its name
  if ("" != physName && factory.IsReferencePhysList(physName))
  {
    G4cout << "Apparently it is referencePhysList" << G4endl;
    phys = factory.GetReferencePhysList(physName);

    // instantiated messenger
    mess = new PhysicsListMessenger();
  }

  // local Physics List
  if (!phys)
  {
    phys = new PhysicsList();
  }

  // define physics
  runManager->SetUserInitialization(phys);
  runManager->SetUserAction(new PrimaryGeneratorAction(detector));

  //set user action classes
  runManager->SetUserAction(new RunAction());
  runManager->SetUserAction(new EventAction());
  runManager->SetUserAction(new StackingAction());
  runManager->SetUserAction(new TrackingAction());
  runManager->SetUserAction(new SteppingAction());

  //initialize visualization
  G4VisManager *visManager = nullptr;

  //get the pointer to the User Interface manager
  G4UImanager *UImanager = G4UImanager::GetUIpointer();

  if (ui)
  {
    //interactive mode
    visManager = new G4VisExecutive;
    visManager->Initialize();
    ui->SessionStart();
    delete ui;
  }
  else
  {
    //batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }

  //job termination
  delete mess;
  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
