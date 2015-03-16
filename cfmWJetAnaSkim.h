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

TTree* pfCandTreeAna_p = 0;
TTree* trackTreeAna_p = 0;
TTree* jetTreeAna_p = 0;
TTree* genTreeAna_p = 0;

//Jet Tree Variables

Int_t run_;
Int_t evt_;
Int_t lumi_;
Int_t hiBin_;

Float_t pthat_;

Float_t hiEvtPlane_;
Float_t psin_;

void SetAnaBranches(sampleType sType = kHIDATA)
{
  Bool_t montecarlo = isMonteCarlo(sType);
  Bool_t hi = isHI(sType);

  std::cout << "Branches Set" << std::endl;

  //Track Tree Branches

  //PF Tree Branches

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

  }

  return;
}


void GetAnaBranches(sampleType sType = kHIDATA)
{
  Bool_t montecarlo = isMonteCarlo(sType);
  Bool_t hi = isHI(sType);
  
  std::cout << "Get Branches" << std::endl;
  
  //Track Tree Branches
  
  //PF Tree Branches
  
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

  }

  return;
}


void InitWJetAnaSkim(sampleType sType = kHIDATA)
{
  std::cout << "Init WJet AnaSkim" << std::endl;

  trackTreeAna_p = new TTree("trackTreeAna", "trackTreeAna");
  jetTreeAna_p = new TTree("jetTreeAna", "jetTreeAna");
  if(isMonteCarlo(sType)) genTreeAna_p = new TTree("genTreeAna", "genTreeAna");

  SetAnaBranches(sType);
}


void CleanupWJetAnaSkim()
{
  if(trackTreeAna_p != 0) delete trackTreeAna_p;
  if(jetTreeAna_p != 0) delete jetTreeAna_p;
  if(genTreeAna_p != 0) delete genTreeAna_p;
}


void GetWJetAnaSkim(TFile* anaFile_p, sampleType sType = kHIDATA){
  std::cout << "Get WJet AnaSkim" << std::endl;

  trackTreeAna_p = (TTree*)anaFile_p->Get("trackTreeAna");
  jetTreeAna_p = (TTree*)anaFile_p->Get("jetTreeAna");
  if(isMonteCarlo(sType)) genTreeAna_p = (TTree*)anaFile_p->Get("genTreeAna");

  GetAnaBranches(sType);
}

#endif
