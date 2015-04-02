//=============================================                                 
// Author: Chris McGinn                                                         
//                                                                              
// WJet Initial Skim Class (MC)                                                    
//                                                                              
//=============================================  
#ifndef cfmWJetIniSkim_h
#define cfmWJetIniSkim_h

#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include <iostream>
#include "/net/hisrv0001/home/cfmcginn/emDiJet/CMSSW_5_3_12_patch3/tempHIFA/HiForestAnalysis/commonSetup.h"
#include "TMath.h"
#include "sType.h"

enum AlgoType_PbPb{
  Vs3Calo,     //0
  Vs4Calo,     //1
  T3,          //2
  T4           //3
};

Bool_t isMonteCarlo(sampleType sType = kHIDATA){
  if(sType == kHIMC || sType == kPPMC || sType == kPAMC) return true;
  else return false;
}


Bool_t isHI(sampleType sType){
  if(sType == kHIDATA || sType == kHIMC) return true;
  else return false;
}


Bool_t isPP(sampleType sType){
  if(sType == kPPDATA || sType == kPPMC) return true;
  else return false;
}


TString getSampleName ( sampleType colli) {
  if (colli == kHIDATA) return "pbpbDATA";
  if (colli == kHIMC) return "pbpbMC";
  if (colli == kPPDATA) return "ppDATA";
  if (colli == kPPMC) return "ppMC";
  if (colli == kPADATA) return "ppbDATA";
  if (colli == kPAMC) return "ppbMC";
  return "NULL";
}
TString getSampleName ( int colli) {
  if (colli == kHIDATA) return "pbpbDATA";
  if (colli == kHIMC) return "pbpbMC";
  if (colli == kPPDATA) return "ppDATA";
  if (colli == kPPMC) return "ppMC";
  if (colli == kPADATA) return "ppbDATA";
  if (colli == kPAMC) return "ppbMC";
  return "NULL";
}

TTree* trackTreeIni_p;
TTree* pfCandTreeIni_p;
TTree* jetTreeIni_p;
TTree* genTreeIni_p;

//Track Tree Variables

Int_t nTrk_;
Float_t trkPt_[maxTracks];
Float_t trkPhi_[maxTracks];
Float_t trkEta_[maxTracks];

//PFCand Tree Variables

Int_t nPF_;
Float_t pfPt_[maxPF];
Float_t pfVsPt_[maxPF];
Float_t pfPhi_[maxPF];
Float_t pfEta_[maxPF];
Int_t pfId_[maxPF];

//Jet Tree Variables

Int_t runIni_;
Int_t evtIni_;
Int_t lumiIni_;
Int_t hiBinIni_;
Float_t pthatIni_;

Float_t hiEvtPlaneIni_;
Float_t psinIni_;

Int_t nVs3Calo_;
Float_t Vs3CaloPt_[maxJets];
Float_t Vs3CaloPhi_[maxJets];
Float_t Vs3CaloEta_[maxJets];
Float_t Vs3CaloTrkMax_[maxJets];
Float_t Vs3CaloRawPt_[maxJets];
Float_t Vs3CaloRefPt_[maxJets];
Float_t Vs3CaloRefPhi_[maxJets];
Float_t Vs3CaloRefEta_[maxJets];
Int_t Vs3CaloRefPart_[maxJets];

Int_t nVs4Calo_;
Float_t Vs4CaloPt_[maxJets];
Float_t Vs4CaloPhi_[maxJets];
Float_t Vs4CaloEta_[maxJets];
Float_t Vs4CaloTrkMax_[maxJets];
Float_t Vs4CaloRawPt_[maxJets];
Float_t Vs4CaloRefPt_[maxJets];
Float_t Vs4CaloRefPhi_[maxJets];
Float_t Vs4CaloRefEta_[maxJets];
Int_t Vs4CaloRefPart_[maxJets];

Int_t nVs5Calo_;
Float_t Vs5CaloPt_[maxJets];
Float_t Vs5CaloPhi_[maxJets];
Float_t Vs5CaloEta_[maxJets];
Float_t Vs5CaloTrkMax_[maxJets];
Float_t Vs5CaloRawPt_[maxJets];
Float_t Vs5CaloRefPt_[maxJets];
Float_t Vs5CaloRefPhi_[maxJets];
Float_t Vs5CaloRefEta_[maxJets];
Int_t Vs5CaloRefPart_[maxJets];

Int_t nT3_;
Float_t T3Pt_[maxJets];
Float_t T3Phi_[maxJets];
Float_t T3Eta_[maxJets];
Int_t T3Part_[maxJets];

Int_t nT4_;
Float_t T4Pt_[maxJets];
Float_t T4Phi_[maxJets];
Float_t T4Eta_[maxJets];
Int_t T4Part_[maxJets];

Int_t nT5_;
Float_t T5Pt_[maxJets];
Float_t T5Phi_[maxJets];
Float_t T5Eta_[maxJets];
Int_t T5Part_[maxJets];

//Gen Tree Variables

Int_t nGen_;
Float_t genPt_[maxEntrySim];
Float_t genPhi_[maxEntrySim];
Float_t genEta_[maxEntrySim];
Int_t genId_[maxEntrySim];
Int_t genMotherId_[maxEntrySim];

void SetIniBranches(sampleType sType = kHIDATA, Bool_t isMinBias = false)
{
  Bool_t montecarlo = isMonteCarlo(sType);
  Bool_t hi = isHI(sType);

  std::cout << "Branches Set" << std::endl;

  //Track Tree Branches
  
  trackTreeIni_p->Branch("nTrk", &nTrk_, "nTrk/I");
  
  trackTreeIni_p->Branch("trkPt", trkPt_, "trkPt[nTrk]/F");
  trackTreeIni_p->Branch("trkPhi", trkPhi_, "trkPhi[nTrk]/F");
  trackTreeIni_p->Branch("trkEta", trkEta_, "trkEta[nTrk]/F");
  
  //PF Cand Tree Branches
  
  pfCandTreeIni_p->Branch("nPF", &nPF_, "nPF/I");
  pfCandTreeIni_p->Branch("pfPt", &pfPt_, "pfPt[nPF]/F");
  if(hi) pfCandTreeIni_p->Branch("pfVsPt", &pfVsPt_, "pfVsPt[nPF]/F");
  pfCandTreeIni_p->Branch("pfPhi", &pfPhi_, "pfPhi[nPF]/F");
  pfCandTreeIni_p->Branch("pfEta", &pfEta_, "pfEta[nPF]/F");
  pfCandTreeIni_p->Branch("pfId", &pfId_, "pfId[nPF]/I");


  //Jet Tree Branches
  
  jetTreeIni_p->Branch("runIni", &runIni_, "runIni/I");
  jetTreeIni_p->Branch("evtIni", &evtIni_, "evtIni/I");
  jetTreeIni_p->Branch("lumiIni", &lumiIni_, "lumiIni/I");
  
  if(hi) jetTreeIni_p->Branch("hiBinIni", &hiBinIni_, "hiBinIni/I");
  if(montecarlo) jetTreeIni_p->Branch("pthatIni", &pthatIni_, "pthatIni/F");

  if(hi){
    jetTreeIni_p->Branch("hiEvtPlaneIni", &hiEvtPlaneIni_, "hiEvtPlaneIni/F");
    jetTreeIni_p->Branch("psinIni", &psinIni_, "psinIni/F");
  }    
 
  jetTreeIni_p->Branch("nVs3Calo", &nVs3Calo_, "nVs3Calo/I");
  jetTreeIni_p->Branch("Vs3CaloPt", Vs3CaloPt_, "Vs3CaloPt[nVs3Calo]/F");
  jetTreeIni_p->Branch("Vs3CaloPhi", Vs3CaloPhi_, "Vs3CaloPhi[nVs3Calo]/F");
  jetTreeIni_p->Branch("Vs3CaloEta", Vs3CaloEta_, "Vs3CaloEta[nVs3Calo]/F");
  jetTreeIni_p->Branch("Vs3CaloTrkMax", Vs3CaloTrkMax_, "Vs3CaloTrkMax[nVs3Calo]/F");
  jetTreeIni_p->Branch("Vs3CaloRawPt", Vs3CaloRawPt_, "Vs3CaloRawPt[nVs3Calo]/F");

  if(!isMinBias){
    jetTreeIni_p->Branch("nVs4Calo", &nVs4Calo_, "nVs4Calo/I");
    jetTreeIni_p->Branch("Vs4CaloPt", Vs4CaloPt_, "Vs4CaloPt[nVs4Calo]/F");
    jetTreeIni_p->Branch("Vs4CaloPhi", Vs4CaloPhi_, "Vs4CaloPhi[nVs4Calo]/F");
    jetTreeIni_p->Branch("Vs4CaloEta", Vs4CaloEta_, "Vs4CaloEta[nVs4Calo]/F");
    jetTreeIni_p->Branch("Vs4CaloTrkMax", Vs4CaloTrkMax_, "Vs4CaloTrkMax[nVs4Calo]/F");
    jetTreeIni_p->Branch("Vs4CaloRawPt", Vs4CaloRawPt_, "Vs4CaloRawPt[nVs4Calo]/F");
    
    jetTreeIni_p->Branch("nVs5Calo", &nVs5Calo_, "nVs5Calo/I");
    jetTreeIni_p->Branch("Vs5CaloPt", Vs5CaloPt_, "Vs5CaloPt[nVs5Calo]/F");
    jetTreeIni_p->Branch("Vs5CaloPhi", Vs5CaloPhi_, "Vs5CaloPhi[nVs5Calo]/F");
    jetTreeIni_p->Branch("Vs5CaloEta", Vs5CaloEta_, "Vs5CaloEta[nVs5Calo]/F");
    jetTreeIni_p->Branch("Vs5CaloTrkMax", Vs5CaloTrkMax_, "Vs5CaloTrkMax[nVs5Calo]/F");
    jetTreeIni_p->Branch("Vs5CaloRawPt", Vs5CaloRawPt_, "Vs5CaloRawPt[nVs5Calo]/F");
  }

  if(montecarlo){
    jetTreeIni_p->Branch("Vs3CaloRefPt", Vs3CaloRefPt_, "Vs3CaloRefPt[nVs3Calo]/F");
    jetTreeIni_p->Branch("Vs3CaloRefPhi", Vs3CaloRefPhi_, "Vs3CaloRefPhi[nVs3Calo]/F");
    jetTreeIni_p->Branch("Vs3CaloRefEta", Vs3CaloRefEta_, "Vs3CaloRefEta[nVs3Calo]/F");
    jetTreeIni_p->Branch("Vs3CaloRefPart", Vs3CaloRefPart_, "Vs3CaloRefPart[nVs3Calo]/I");

    if(!isMinBias){
      jetTreeIni_p->Branch("Vs4CaloRefPt", Vs4CaloRefPt_, "Vs4CaloRefPt[nVs4Calo]/F");
      jetTreeIni_p->Branch("Vs4CaloRefPhi", Vs4CaloRefPhi_, "Vs4CaloRefPhi[nVs4Calo]/F");
      jetTreeIni_p->Branch("Vs4CaloRefEta", Vs4CaloRefEta_, "Vs4CaloRefEta[nVs4Calo]/F");
      jetTreeIni_p->Branch("Vs4CaloRefPart", Vs4CaloRefPart_, "Vs4CaloRefPart[nVs4Calo]/I");
      
      jetTreeIni_p->Branch("Vs5CaloRefPt", Vs5CaloRefPt_, "Vs5CaloRefPt[nVs5Calo]/F");
      jetTreeIni_p->Branch("Vs5CaloRefPhi", Vs5CaloRefPhi_, "Vs5CaloRefPhi[nVs5Calo]/F");
      jetTreeIni_p->Branch("Vs5CaloRefEta", Vs5CaloRefEta_, "Vs5CaloRefEta[nVs5Calo]/F");
      jetTreeIni_p->Branch("Vs5CaloRefPart", Vs5CaloRefPart_, "Vs5CaloRefPart[nVs5Calo]/I");
    }      

    jetTreeIni_p->Branch("nT3", &nT3_, "nT3/I");
    jetTreeIni_p->Branch("T3Pt", T3Pt_, "T3Pt[nT3]/F");
    jetTreeIni_p->Branch("T3Phi", T3Phi_, "T3Phi[nT3]/F");
    jetTreeIni_p->Branch("T3Eta", T3Eta_, "T3Eta[nT3]/F");
    jetTreeIni_p->Branch("T3Part", T3Part_, "T3Part[nT3]/I");

    if(!isMinBias){
      jetTreeIni_p->Branch("nT4", &nT4_, "nT4/I");
      jetTreeIni_p->Branch("T4Pt", T4Pt_, "T4Pt[nT4]/F");
      jetTreeIni_p->Branch("T4Phi", T4Phi_, "T4Phi[nT4]/F");
      jetTreeIni_p->Branch("T4Eta", T4Eta_, "T4Eta[nT4]/F");
      jetTreeIni_p->Branch("T4Part", T4Part_, "T4Part[nT4]/I");
      
      jetTreeIni_p->Branch("nT5", &nT5_, "nT5/I");
      jetTreeIni_p->Branch("T5Pt", T5Pt_, "T5Pt[nT5]/F");
      jetTreeIni_p->Branch("T5Phi", T5Phi_, "T5Phi[nT5]/F");
      jetTreeIni_p->Branch("T5Eta", T5Eta_, "T5Eta[nT5]/F");
      jetTreeIni_p->Branch("T5Part", T5Part_, "T5Part[nT5]/I");
    }
  }

  //Gen Tree Branches

  if(montecarlo){
    genTreeIni_p->Branch("nGen", &nGen_, "nGen/I");
    
    genTreeIni_p->Branch("genPt", genPt_, "genPt[nGen]/F");
    genTreeIni_p->Branch("genPhi", genPhi_, "genPhi[nGen]/F");
    genTreeIni_p->Branch("genEta", genEta_, "genEta[nGen]/F");    
    genTreeIni_p->Branch("genId", genId_, "genId[nGen]/I");
    genTreeIni_p->Branch("genMotherId", genMotherId_, "genMotherId[nGen]/I");
  }    

  return;
}


void GetIniBranches(sampleType sType = kHIDATA, Bool_t isMinBias = false)
{
  Bool_t montecarlo = isMonteCarlo(sType);
  Bool_t hi = isHI(sType);

  std::cout << "Get Branches" << std::endl;

  //Track Tree Branches

  trackTreeIni_p->SetBranchAddress("nTrk", &nTrk_);
  trackTreeIni_p->SetBranchAddress("trkPt", trkPt_);
  trackTreeIni_p->SetBranchAddress("trkPhi", trkPhi_);
  trackTreeIni_p->SetBranchAddress("trkEta", trkEta_);
  
  pfCandTreeIni_p->SetBranchAddress("nPF", &nPF_);
  pfCandTreeIni_p->SetBranchAddress("pfPt", pfPt_);
  if(hi) pfCandTreeIni_p->SetBranchAddress("pfVsPt", pfVsPt_);
  pfCandTreeIni_p->SetBranchAddress("pfPhi", pfPhi_);
  pfCandTreeIni_p->SetBranchAddress("pfEta", pfEta_);    
  pfCandTreeIni_p->SetBranchAddress("pfId", pfId_);

  //Jet Tree Branches

  jetTreeIni_p->SetBranchAddress("runIni", &runIni_);
  jetTreeIni_p->SetBranchAddress("evtIni", &evtIni_);
  jetTreeIni_p->SetBranchAddress("lumiIni", &lumiIni_);

  if(hi) jetTreeIni_p->SetBranchAddress("hiBinIni", &hiBinIni_);

  if(montecarlo) jetTreeIni_p->SetBranchAddress("pthatIni", &pthatIni_);

  if(hi){
    jetTreeIni_p->SetBranchAddress("hiEvtPlaneIni", &hiEvtPlaneIni_);
    jetTreeIni_p->SetBranchAddress("psinIni", &psinIni_);
  }  


  jetTreeIni_p->SetBranchAddress("nVs3Calo", &nVs3Calo_);
  jetTreeIni_p->SetBranchAddress("Vs3CaloPt", Vs3CaloPt_);
  jetTreeIni_p->SetBranchAddress("Vs3CaloPhi", Vs3CaloPhi_);
  jetTreeIni_p->SetBranchAddress("Vs3CaloEta", Vs3CaloEta_);
  jetTreeIni_p->SetBranchAddress("Vs3CaloTrkMax", Vs3CaloTrkMax_);

  if(!isMinBias){
    jetTreeIni_p->SetBranchAddress("nVs4Calo", &nVs4Calo_);
    jetTreeIni_p->SetBranchAddress("Vs4CaloPt", Vs4CaloPt_);
    jetTreeIni_p->SetBranchAddress("Vs4CaloPhi", Vs4CaloPhi_);
    jetTreeIni_p->SetBranchAddress("Vs4CaloEta", Vs4CaloEta_);
    jetTreeIni_p->SetBranchAddress("Vs4CaloTrkMax", Vs4CaloTrkMax_);
    
    jetTreeIni_p->SetBranchAddress("nVs5Calo", &nVs5Calo_);
    jetTreeIni_p->SetBranchAddress("Vs5CaloPt", Vs5CaloPt_);
    jetTreeIni_p->SetBranchAddress("Vs5CaloPhi", Vs5CaloPhi_);
    jetTreeIni_p->SetBranchAddress("Vs5CaloEta", Vs5CaloEta_);
    jetTreeIni_p->SetBranchAddress("Vs5CaloTrkMax", Vs5CaloTrkMax_);
  }

  if(montecarlo){
    jetTreeIni_p->SetBranchAddress("Vs3CaloRefPt", Vs3CaloRefPt_);
    jetTreeIni_p->SetBranchAddress("Vs3CaloRefPhi", Vs3CaloRefPhi_);
    jetTreeIni_p->SetBranchAddress("Vs3CaloRefEta", Vs3CaloRefEta_);
    jetTreeIni_p->SetBranchAddress("Vs3CaloRefPart", Vs3CaloRefPart_);

    if(!isMinBias){
      jetTreeIni_p->SetBranchAddress("Vs4CaloRefPt", Vs4CaloRefPt_);
      jetTreeIni_p->SetBranchAddress("Vs4CaloRefPhi", Vs4CaloRefPhi_);
      jetTreeIni_p->SetBranchAddress("Vs4CaloRefEta", Vs4CaloRefEta_);
      jetTreeIni_p->SetBranchAddress("Vs4CaloRefPart", Vs4CaloRefPart_);
      
      jetTreeIni_p->SetBranchAddress("Vs5CaloRefPt", Vs5CaloRefPt_);
      jetTreeIni_p->SetBranchAddress("Vs5CaloRefPhi", Vs5CaloRefPhi_);
      jetTreeIni_p->SetBranchAddress("Vs5CaloRefEta", Vs5CaloRefEta_);
      jetTreeIni_p->SetBranchAddress("Vs5CaloRefPart", Vs5CaloRefPart_);
    }      

    jetTreeIni_p->SetBranchAddress("nT3", &nT3_);
    jetTreeIni_p->SetBranchAddress("T3Pt", T3Pt_);
    jetTreeIni_p->SetBranchAddress("T3Phi", T3Phi_);
    jetTreeIni_p->SetBranchAddress("T3Eta", T3Eta_);
    jetTreeIni_p->SetBranchAddress("T3Part", T3Part_);

    if(!isMinBias){
      jetTreeIni_p->SetBranchAddress("nT4", &nT4_);
      jetTreeIni_p->SetBranchAddress("T4Pt", T4Pt_);
      jetTreeIni_p->SetBranchAddress("T4Phi", T4Phi_);
      jetTreeIni_p->SetBranchAddress("T4Eta", T4Eta_);
      jetTreeIni_p->SetBranchAddress("T4Part", T4Part_);
      
      jetTreeIni_p->SetBranchAddress("nT5", &nT5_);
      jetTreeIni_p->SetBranchAddress("T5Pt", T5Pt_);
      jetTreeIni_p->SetBranchAddress("T5Phi", T5Phi_);
      jetTreeIni_p->SetBranchAddress("T5Eta", T5Eta_);
      jetTreeIni_p->SetBranchAddress("T5Part", T5Part_);
    }
  }

  //Gen Tree Branches

  if(montecarlo){
    genTreeIni_p->SetBranchAddress("nGen", &nGen_);
    genTreeIni_p->SetBranchAddress("genPt", genPt_);
    genTreeIni_p->SetBranchAddress("genPhi", genPhi_);
    genTreeIni_p->SetBranchAddress("genEta", genEta_);
    genTreeIni_p->SetBranchAddress("genId", genId_);
    genTreeIni_p->SetBranchAddress("genMotherId", genMotherId_);
  }

  return;
}


void InitWJetIniSkim(sampleType sType = kHIDATA, Bool_t isMinBias = false)
{
  std::cout << "Init WJet IniSkim" << std::endl;

  trackTreeIni_p = new TTree("trackTreeIni", "trackTreeIni");
  pfCandTreeIni_p = new TTree("pfCandTreeIni", "pfCandTreeIni");
  jetTreeIni_p = new TTree("jetTreeIni", "jetTreeIni");
  if(isMonteCarlo(sType)) genTreeIni_p = new TTree("genTreeIni", "genTreeIni");

  SetIniBranches(sType, isMinBias);

  return;
}


void CleanupWJetIniSkim()
{
  if(trackTreeIni_p != 0) delete trackTreeIni_p;
  if(pfCandTreeIni_p != 0) delete pfCandTreeIni_p;
  if(jetTreeIni_p != 0) delete jetTreeIni_p;
  if(genTreeIni_p != 0) delete genTreeIni_p;

  return;
}


void GetWJetIniSkim(TFile* iniFile_p, sampleType sType = kHIDATA, Bool_t isMinBias = false)
{
  std::cout << "Get WJet IniSkim" << std::endl;

  trackTreeIni_p = (TTree*)iniFile_p->Get("trackTreeIni");
  pfCandTreeIni_p = (TTree*)iniFile_p->Get("pfCandTreeIni");
  jetTreeIni_p = (TTree*)iniFile_p->Get("jetTreeIni");
  if(isMonteCarlo(sType)) genTreeIni_p = (TTree*)iniFile_p->Get("genTreeIni");

  GetIniBranches(sType, isMinBias);

  return;
}

#endif
