//=============================================                                 
// Author: Chris McGinn                                                         
//                                                                              
// WJet Analysis Skim Class (MC)                                                    
//                                                                              
//=============================================  
#ifndef cfmWJetAnaSkim_h
#define cfmWJetAnaSkim_h

#include "cfmWJetIniSkim.h"

#include <string>

TTree* trackTreeAna_p = 0;
TTree* pfCandTreeAna_p = 0;
TTree* jetTreeAna_p = 0;
TTree* genTreeAna_p = 0;

//Track Tree Variables

Float_t trkEvtPtSum_[2];
Float_t trkEvtPtMag_;
Float_t trkEvtPhi_;

Float_t trkNeuPhi_;
Float_t trkMt_;
Float_t trkCheck1Mt_;
Float_t trkCheck2Mt_;

//PF Tree Variables

Float_t muonPt_;
Float_t muonVsPt_;
Float_t muonPhi_;
Float_t muonEta_;

Float_t electronPt_;
Float_t electronVsPt_;
Float_t electronPhi_;
Float_t electronEta_;

Float_t leptPt_;
Float_t leptVsPt_;
Float_t leptPhi_;
Float_t leptEta_;

Float_t check1Pt_;
Float_t check1VsPt_;
Float_t check1Phi_;
Float_t check1Eta_;

Float_t check2Pt_;
Float_t check2VsPt_;
Float_t check2Phi_;
Float_t check2Eta_;

Float_t pfEvtPtSum_[2];
Float_t pfEvtPtMag_;
Float_t pfEvtPhi_;

Float_t pfNeuPhi_;
Float_t pfMt_;
Float_t pfCheck1Mt_;
Float_t pfCheck2Mt_;

Float_t pfEvtVsPtSum_[2];
Float_t pfEvtVsPtMag_;
Float_t pfEvtVsPhi_;

Float_t pfVsNeuPhi_;
Float_t pfVsMt_;
Float_t pfVsCheck1Mt_;
Float_t pfVsCheck2Mt_;

//Jet Tree Variables

Int_t run_;
Int_t evt_;
Int_t lumi_;
Int_t hiBin_;

Float_t pthat_;

Float_t hiEvtPlane_;
Float_t psin_;

//Gen. Tree Variables

Float_t genEvtPtSum_[2];
Float_t genEvtPtMag_;
Float_t genEvtPhi_;

Float_t genNeuPhi_;
Float_t genMt_;
Float_t genCheck1Mt_;
Float_t genCheck2Mt_;

void SetAnaBranches(sampleType sType = kHIDATA)
{
  Bool_t montecarlo = isMonteCarlo(sType);
  Bool_t hi = isHI(sType);

  std::cout << "Branches Set" << std::endl;

  //Track Tree Branches

  trackTreeAna_p->Branch("trkEvtPtSum", trkEvtPtSum_, "trkEvtPtSum[2]/F");
  trackTreeAna_p->Branch("trkEvtPtMag", &trkEvtPtMag_, "trkEvtPtMag/F");
  trackTreeAna_p->Branch("trkEvtPhi", &trkEvtPhi_, "trkEvtPhi/F");

  trackTreeAna_p->Branch("trkNeuPhi", &trkNeuPhi_, "trkNeuPhi/F");
  trackTreeAna_p->Branch("trkMt", &trkMt_, "trkMt/F");
  trackTreeAna_p->Branch("trkCheck1Mt", &trkCheck1Mt_, "trkCheck1Mt/F");
  trackTreeAna_p->Branch("trkCheck2Mt", &trkCheck2Mt_, "trkCheck2Mt/F");


  //PF Tree Branches

  pfCandTreeAna_p->Branch("muonPt", &muonPt_, "muonPt/F");
  pfCandTreeAna_p->Branch("muonVsPt", &muonVsPt_, "muonVsPt/F");
  pfCandTreeAna_p->Branch("muonPhi", &muonPhi_, "muonPhi/F");
  pfCandTreeAna_p->Branch("muonEta", &muonEta_, "muonEta/F");

  pfCandTreeAna_p->Branch("electronPt", &electronPt_, "electronPt/F");
  pfCandTreeAna_p->Branch("electronVsPt", &electronVsPt_, "electronVsPt/F");
  pfCandTreeAna_p->Branch("electronPhi", &electronPhi_, "electronPhi/F");
  pfCandTreeAna_p->Branch("electronEta", &electronEta_, "electronEta/F");

  pfCandTreeAna_p->Branch("leptPt", &leptPt_, "leptPt/F");
  pfCandTreeAna_p->Branch("leptVsPt", &leptVsPt_, "leptVsPt/F");
  pfCandTreeAna_p->Branch("leptPhi", &leptPhi_, "leptPhi/F");
  pfCandTreeAna_p->Branch("leptEta", &leptEta_, "leptEta/F");

  pfCandTreeAna_p->Branch("check1Pt", &check1Pt_, "check1Pt/F");
  pfCandTreeAna_p->Branch("check1VsPt", &check1VsPt_, "check1VsPt/F");
  pfCandTreeAna_p->Branch("check1Phi", &check1Phi_, "check1Phi/F");
  pfCandTreeAna_p->Branch("check1Eta", &check1Eta_, "check1Eta/F");

  pfCandTreeAna_p->Branch("check2Pt", &check2Pt_, "check2Pt/F");
  pfCandTreeAna_p->Branch("check2VsPt", &check2VsPt_, "check2VsPt/F");
  pfCandTreeAna_p->Branch("check2Phi", &check2Phi_, "check2Phi/F");
  pfCandTreeAna_p->Branch("check2Eta", &check2Eta_, "check2Eta/F");

  pfCandTreeAna_p->Branch("pfEvtPtSum", pfEvtPtSum_, "pfEvtPtSum[2]/F");
  pfCandTreeAna_p->Branch("pfEvtPtMag", &pfEvtPtMag_, "pfEvtPtMag/F");
  pfCandTreeAna_p->Branch("pfEvtPhi", &pfEvtPhi_, "pfEvtPhi/F");

  pfCandTreeAna_p->Branch("pfNeuPhi", &pfNeuPhi_, "pfNeuPhi/F");
  pfCandTreeAna_p->Branch("pfMt", &pfMt_, "pfMt/F");
  pfCandTreeAna_p->Branch("pfCheck1Mt", &pfCheck1Mt_, "pfCheck1Mt/F");
  pfCandTreeAna_p->Branch("pfCheck2Mt", &pfCheck2Mt_, "pfCheck2Mt/F");

  pfCandTreeAna_p->Branch("pfEvtVsPtSum", pfEvtVsPtSum_, "pfEvtVsPtSum[2]/F");
  pfCandTreeAna_p->Branch("pfEvtVsPtMag", &pfEvtVsPtMag_, "pfEvtVsPtMag/F");
  pfCandTreeAna_p->Branch("pfEvtVsPhi", &pfEvtVsPhi_, "pfEvtVsPhi/F");

  pfCandTreeAna_p->Branch("pfVsNeuPhi", &pfVsNeuPhi_, "pfVsNeuPhi/F");
  pfCandTreeAna_p->Branch("pfVsMt", &pfVsMt_, "pfVsMt/F");
  pfCandTreeAna_p->Branch("pfVsCheck1Mt", &pfVsCheck1Mt_, "pfVsCheck1Mt/F");
  pfCandTreeAna_p->Branch("pfVsCheck2Mt", &pfVsCheck2Mt_, "pfVsCheck2Mt/F");

  //Jet Tree Branches

  jetTreeAna_p->Branch("run", &run_, "run/I");
  jetTreeAna_p->Branch("evt", &evt_, "evt/I");
  jetTreeAna_p->Branch("lumi", &lumi_, "lumi/I");

  if(montecarlo) jetTreeAna_p->Branch("pthat", &pthat_, "pthat/F");

  if(hi){
    jetTreeAna_p->Branch("hiBin", &hiBin_, "hiBin/I");
    jetTreeAna_p->Branch("hiEvtPlane", &hiEvtPlane_, "hiEvtPlane/F");
    jetTreeAna_p->Branch("psin", &psin_, "psin/F");
  }

  //Gen. Tree Branches

  if(montecarlo){
    genTreeAna_p->Branch("genEvtPtSum", genEvtPtSum_, "genEvtPtSum[2]/F");
    genTreeAna_p->Branch("genEvtPtMag", &genEvtPtMag_, "genEvtPtMag/F");
    genTreeAna_p->Branch("genEvtPhi", &genEvtPhi_, "genEvtPhi/F");

    genTreeAna_p->Branch("genNeuPhi", &genNeuPhi_, "genNeuPhi/F");
    genTreeAna_p->Branch("genMt", &genMt_, "genMt/F");
    genTreeAna_p->Branch("genCheck1Mt", &genCheck1Mt_, "genCheck1Mt/F");
    genTreeAna_p->Branch("genCheck2Mt", &genCheck2Mt_, "genCheck2Mt/F");
  }

  return;
}


void GetAnaBranches(sampleType sType = kHIDATA)
{
  Bool_t montecarlo = isMonteCarlo(sType);
  Bool_t hi = isHI(sType);
  
  std::cout << "Get Branches" << std::endl;
  
  //Track Tree Branches
  
  trackTreeAna_p->SetBranchAddress("trkEvtPtSum", trkEvtPtSum_);
  trackTreeAna_p->SetBranchAddress("trkEvtPtMag", &trkEvtPtMag_);
  trackTreeAna_p->SetBranchAddress("trkEvtPhi", &trkEvtPhi_);

  trackTreeAna_p->SetBranchAddress("trkNeuPhi", &trkNeuPhi_);
  trackTreeAna_p->SetBranchAddress("trkMt", &trkMt_);
  trackTreeAna_p->SetBranchAddress("trkCheck1Mt", &trkCheck1Mt_);
  trackTreeAna_p->SetBranchAddress("trkCheck2Mt", &trkCheck2Mt_);

  //PF Tree Branches

  pfCandTreeAna_p->SetBranchAddress("muonPt", &muonPt_);
  pfCandTreeAna_p->SetBranchAddress("muonVsPt", &muonVsPt_);
  pfCandTreeAna_p->SetBranchAddress("muonPhi", &muonPhi_);
  pfCandTreeAna_p->SetBranchAddress("muonEta", &muonEta_);

  pfCandTreeAna_p->SetBranchAddress("electronPt", &electronPt_);
  pfCandTreeAna_p->SetBranchAddress("electronVsPt", &electronVsPt_);
  pfCandTreeAna_p->SetBranchAddress("electronPhi", &electronPhi_);
  pfCandTreeAna_p->SetBranchAddress("electronEta", &electronEta_);

  pfCandTreeAna_p->SetBranchAddress("leptPt", &leptPt_);
  pfCandTreeAna_p->SetBranchAddress("leptVsPt", &leptVsPt_);
  pfCandTreeAna_p->SetBranchAddress("leptPhi", &leptPhi_);
  pfCandTreeAna_p->SetBranchAddress("leptEta", &leptEta_);

  pfCandTreeAna_p->SetBranchAddress("check1Pt", &check1Pt_);
  pfCandTreeAna_p->SetBranchAddress("check1VsPt", &check1VsPt_);
  pfCandTreeAna_p->SetBranchAddress("check1Phi", &check1Phi_);
  pfCandTreeAna_p->SetBranchAddress("check1Eta", &check1Eta_);

  pfCandTreeAna_p->SetBranchAddress("check2Pt", &check2Pt_);
  pfCandTreeAna_p->SetBranchAddress("check2VsPt", &check2VsPt_);
  pfCandTreeAna_p->SetBranchAddress("check2Phi", &check2Phi_);
  pfCandTreeAna_p->SetBranchAddress("check2Eta", &check2Eta_);

  pfCandTreeAna_p->SetBranchAddress("pfEvtPtSum", pfEvtPtSum_);
  pfCandTreeAna_p->SetBranchAddress("pfEvtPtMag", &pfEvtPtMag_);
  pfCandTreeAna_p->SetBranchAddress("pfEvtPhi", &pfEvtPhi_);  

  pfCandTreeAna_p->SetBranchAddress("pfNeuPhi", &pfNeuPhi_);  
  pfCandTreeAna_p->SetBranchAddress("pfMt", &pfMt_);  
  pfCandTreeAna_p->SetBranchAddress("pfCheck1Mt", &pfCheck1Mt_);  
  pfCandTreeAna_p->SetBranchAddress("pfCheck2Mt", &pfCheck2Mt_);  

  pfCandTreeAna_p->SetBranchAddress("pfEvtVsPtSum", pfEvtVsPtSum_);
  pfCandTreeAna_p->SetBranchAddress("pfEvtVsPtMag", &pfEvtVsPtMag_);
  pfCandTreeAna_p->SetBranchAddress("pfEvtVsPhi", &pfEvtVsPhi_);  

  pfCandTreeAna_p->SetBranchAddress("pfVsNeuPhi", &pfVsNeuPhi_);  
  pfCandTreeAna_p->SetBranchAddress("pfVsMt", &pfVsMt_);  
  pfCandTreeAna_p->SetBranchAddress("pfVsCheck1Mt", &pfVsCheck1Mt_);  
  pfCandTreeAna_p->SetBranchAddress("pfVsCheck2Mt", &pfVsCheck2Mt_);  

  //Jet Tree Branches
  
  jetTreeAna_p->SetBranchAddress("run", &run_);
  jetTreeAna_p->SetBranchAddress("evt", &evt_);
  jetTreeAna_p->SetBranchAddress("lumi", &lumi_);
  
  if(montecarlo) jetTreeAna_p->SetBranchAddress("pthat", &pthat_);
  
  if(hi){
    jetTreeAna_p->SetBranchAddress("hiBin", &hiBin_);
    jetTreeAna_p->SetBranchAddress("hiEvtPlane", &hiEvtPlane_);
    jetTreeAna_p->SetBranchAddress("psin", &psin_);
  }

  //Gen Tree Variables
    
  if(montecarlo){
    genTreeAna_p->SetBranchAddress("genEvtPtSum", genEvtPtSum_);
    genTreeAna_p->SetBranchAddress("genEvtPtMag", &genEvtPtMag_);
    genTreeAna_p->SetBranchAddress("genEvtPhi", &genEvtPhi_);

    genTreeAna_p->SetBranchAddress("genNeuPhi", &genNeuPhi_);
    genTreeAna_p->SetBranchAddress("genMt", &genMt_);
    genTreeAna_p->SetBranchAddress("genCheck1Mt", &genCheck1Mt_);
    genTreeAna_p->SetBranchAddress("genCheck2Mt", &genCheck2Mt_);
  }

  return;
}


void InitWJetAnaSkim(sampleType sType = kHIDATA)
{
  std::cout << "Init WJet AnaSkim" << std::endl;

  trackTreeAna_p = new TTree("trackTreeAna", "trackTreeAna");
  pfCandTreeAna_p = new TTree("pfCandTreeAna", "pfCandTreeAna");
  jetTreeAna_p = new TTree("jetTreeAna", "jetTreeAna");
  if(isMonteCarlo(sType)) genTreeAna_p = new TTree("genTreeAna", "genTreeAna");

  SetAnaBranches(sType);

  return;
}


void CleanupWJetAnaSkim()
{
  if(trackTreeAna_p != 0) delete trackTreeAna_p;
  if(pfCandTreeAna_p != 0) delete pfCandTreeAna_p;
  if(jetTreeAna_p != 0) delete jetTreeAna_p;
  if(genTreeAna_p != 0) delete genTreeAna_p;

  return;
}


void GetWJetAnaSkim(TFile* anaFile_p, sampleType sType = kHIDATA){
  std::cout << "Get WJet AnaSkim" << std::endl;

  trackTreeAna_p = (TTree*)anaFile_p->Get("trackTreeAna");
  pfCandTreeAna_p = (TTree*)anaFile_p->Get("pfCandTreeAna");
  jetTreeAna_p = (TTree*)anaFile_p->Get("jetTreeAna");
  if(isMonteCarlo(sType)) genTreeAna_p = (TTree*)anaFile_p->Get("genTreeAna");

  GetAnaBranches(sType);

  return;
}


void InitAnaVar()
{
  //Track Tree Variables

  trkEvtPtSum_[0] = 0;
  trkEvtPtSum_[1] = 0;
  trkEvtPtMag_ = -999;
  trkEvtPhi_ = -999;

  trkNeuPhi_ = -999;
  trkMt_ = -999;
  trkCheck1Mt_ = -999;
  trkCheck2Mt_ = -999;

  //PF Tree Variables

  muonPt_ = -999;
  muonVsPt_ = -999;
  muonPhi_ = -999;
  muonEta_ = -999;

  electronPt_ = -999;
  electronVsPt_ = -999;
  electronPhi_ = -999;
  electronEta_ = -999;

  leptPt_ = -999;
  leptVsPt_ = -999;
  leptPhi_ = -999;
  leptEta_ = -999;

  check1Pt_ = -999;
  check1VsPt_ = -999;
  check1Phi_ = -999;
  check1Eta_ = -999;

  check2Pt_ = -999;
  check2VsPt_ = -999;
  check2Phi_ = -999;
  check2Eta_ = -999;

  pfEvtPtSum_[0] = 0;
  pfEvtPtSum_[1] = 0;
  pfEvtPtMag_ = -999;
  pfEvtPhi_ = -999;

  pfNeuPhi_ = -999;
  pfMt_ = -999;
  pfCheck1Mt_ = -999;
  pfCheck2Mt_ = -999;

  pfEvtVsPtSum_[0] = 0;
  pfEvtVsPtSum_[1] = 0;
  pfEvtVsPtMag_ = -999;
  pfEvtVsPhi_ = -999;

  pfVsNeuPhi_ = -999;
  pfVsMt_ = -999;
  pfVsCheck1Mt_ = -999;
  pfVsCheck2Mt_ = -999;

  //Gen Tree Variables

  genEvtPtSum_[0] = 0;
  genEvtPtSum_[1] = 0;
  genEvtPtMag_ = -999;
  genEvtPhi_ = -999;

  genNeuPhi_ = -999;
  genMt_ = -999;
  genCheck1Mt_ = -999;
  genCheck2Mt_ = -999;

  return;
}


#endif
