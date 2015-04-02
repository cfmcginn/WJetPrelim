//=============================================
// Author McGinn
// 
// WJet Initial Skim Class (MC)
//
// !!NOTE: Written for jets sorted by pt, tracks unsorted!!
//
//=============================================

#include "/net/hisrv0001/home/cfmcginn/emDiJet/CMSSW_5_3_12_patch3/tempHIFA/HiForestAnalysis/hiForest.h"
#include "cfmWJetIniSkim.h"
#include "stdlib.h"
#include <fstream>

#include "TComplex.h"

#include <vector>
#include "TLorentzVector.h"

const Float_t jtThreshPtCut = 30.0;
const Float_t jtEtaCut = 2.0;
const Int_t nEvtPerFile = 10000;

const Float_t leptEtaCut = 2.4;
const Float_t leptPtCut = 15.0;

collisionType getCType(sampleType sType);

void setJtBranches(TTree* inJtTree, Bool_t montecarlo = false, Bool_t isGen = false)
{
  inJtTree->SetBranchStatus("*", 0);
  inJtTree->SetBranchStatus("nref", 1);
  inJtTree->SetBranchStatus("jtpt", 1);
  inJtTree->SetBranchStatus("rawpt", 1);
  inJtTree->SetBranchStatus("jteta", 1);
  inJtTree->SetBranchStatus("jtphi", 1);
  inJtTree->SetBranchStatus("trackMax", 1);
  if(montecarlo){
    inJtTree->SetBranchStatus("pthat", 1);
    inJtTree->SetBranchStatus("refparton_flavor", 1);
    inJtTree->SetBranchStatus("refpt", 1);
    inJtTree->SetBranchStatus("refeta", 1);
    inJtTree->SetBranchStatus("refphi", 1);
    if(isGen){
      inJtTree->SetBranchStatus("ngen", 1);
      inJtTree->SetBranchStatus("genpt", 1);
      inJtTree->SetBranchStatus("geneta", 1);
      inJtTree->SetBranchStatus("genphi", 1);
      inJtTree->SetBranchStatus("genmatchindex", 1);
    }
  }
  return;
}


int makeWJetIniSkim(string fList = "", sampleType sType = kHIDATA, Bool_t isMinBias = false, const char *outName = "defaultName_DIJETINISKIM.root", Int_t num = 0)
{
  //Define MC or Data
  Bool_t montecarlo = isMonteCarlo(sType);
  Bool_t hi = isHI(sType);

  std::cout << sType << std::endl;
  std::cout << montecarlo << std::endl;

  collisionType cType = getCType(sType);

  string buffer;
  std::vector<string> listOfFiles;
  int nLines = 0;
  ifstream inFile(fList.data());

  std::cout << fList << std::endl;
  std::cout << inFile.is_open() << std::endl;

  if(!inFile.is_open()){
    std::cout << "Error opening file. Exiting." <<std::endl;
    return 1;
  }
  else{
    while(true){
      inFile >> buffer;
      if(inFile.eof()) break;
      listOfFiles.push_back(buffer);
      nLines++;
    }
  }

  std::cout << "FileList Loaded" << std::endl;

  for(Int_t iter = 0; iter < (Int_t)(listOfFiles.size()); iter++){
    std::cout << listOfFiles[iter] << std::endl;
  }

  //Setup correction tables

  HiForest *c = new HiForest(listOfFiles[num].data(), "Forest", cType, montecarlo);
  std::cout << "JobFile: " << listOfFiles[num] << std::endl;

  c->skimTree->SetBranchStatus("*", 0);
  c->skimTree->SetBranchStatus("pHBHENoiseFilter", 1);
  
  c->evtTree->SetBranchStatus("*", 0);
  c->evtTree->SetBranchStatus("vz", 1);
  if(hi) c->evtTree->SetBranchStatus("hiEvtPlanes", 1);
  c->evtTree->SetBranchStatus("run", 1);
  c->evtTree->SetBranchStatus("evt", 1);
  c->evtTree->SetBranchStatus("lumi", 1);  

  c->trackTree->SetBranchStatus("*", 0);
  c->trackTree->SetBranchStatus("nTrk", 1);
  c->trackTree->SetBranchStatus("trkPt", 1);
  c->trackTree->SetBranchStatus("trkPhi", 1);
  c->trackTree->SetBranchStatus("trkEta", 1);
  c->trackTree->SetBranchStatus("highPurity", 1);
  c->trackTree->SetBranchStatus("trkDz1", 1);
  c->trackTree->SetBranchStatus("trkDzError1", 1);
  c->trackTree->SetBranchStatus("trkDxy1", 1);
  c->trackTree->SetBranchStatus("trkDxyError1", 1);
  c->trackTree->SetBranchStatus("trkPtError", 1);

  c->pfTree->SetBranchStatus("*", 0);
  c->pfTree->SetBranchStatus("nPFpart", 1);
  c->pfTree->SetBranchStatus("pfPt", 1);
  if(hi) c->pfTree->SetBranchStatus("pfVsPt", 1);
  c->pfTree->SetBranchStatus("pfPhi", 1);
  c->pfTree->SetBranchStatus("pfEta", 1);
  c->pfTree->SetBranchStatus("pfId", 1);

  if(montecarlo){
    c->genParticleTree->SetBranchStatus("*", 0);
    c->genParticleTree->SetBranchStatus("b", 1);
    c->genParticleTree->SetBranchStatus("mult", 1);
    c->genParticleTree->SetBranchStatus("chg", 1);
    c->genParticleTree->SetBranchStatus("pt", 1);
    c->genParticleTree->SetBranchStatus("phi", 1);
    c->genParticleTree->SetBranchStatus("eta", 1);
    c->genParticleTree->SetBranchStatus("sube", 1);
  }

  if(hi){
    c->skimTree->SetBranchStatus("pcollisionEventSelection", 1);

    c->evtTree->SetBranchStatus("hiBin", 1);

    setJtBranches(c->akVs3CaloJetTree, montecarlo, true);
    if(!isMinBias) setJtBranches(c->akVs4CaloJetTree, montecarlo, true);
    if(!isMinBias) setJtBranches(c->akVs5CaloJetTree, montecarlo, true);
  }
  else{
    c->skimTree->SetBranchStatus("pPAcollisionEventSelectionPA", 1);

    setJtBranches(c->ak3CaloJetTree, montecarlo, true);
    if(!isMinBias) setJtBranches(c->ak4CaloJetTree, montecarlo, true);
    if(!isMinBias) setJtBranches(c->ak5CaloJetTree, montecarlo, true);
  }

  c->LoadNoTrees();

  c->hasSkimTree = true;
  c->hasEvtTree = true;

  if(hi){
    c->hasAkVs3CaloJetTree = true;
    if(!isMinBias) c->hasAkVs4CaloJetTree = true;
    if(!isMinBias) c->hasAkVs5CaloJetTree = true;
  }
  else{
    c->hasAk3CaloJetTree = true;
    if(!isMinBias) c->hasAk4CaloJetTree = true;
    if(!isMinBias) c->hasAk5CaloJetTree = true;
  }

  c->hasTrackTree = true;
  c->hasPFTree = true;
  if(montecarlo) c->hasGenParticleTree = true;

  c->InitTree();

  Float_t meanVz = 0;

  if(sType == kHIMC){
    //mean mc .16458, mean data -.337
    meanVz = .16458 + .337;
  }
  else if(sType == kPPMC){
    //MC vz = .4205,  Data vz = .6953
    meanVz = .4205 - .6953;
  }

  Long64_t nentries;

  Int_t nEvtsOutTag = 0;

  if(!hi) nentries = c->ak3CaloJetTree->GetEntries();
  else nentries = c->akVs3CaloJetTree->GetEntries();

  std::cout << nentries << std::endl;

  TFile *outFile = new TFile(Form("%s_%d_%d.root", outName, num, 0), "RECREATE");
  InitWJetIniSkim(sType);

  Int_t totEv = 0;
  Int_t selectCut = 0;
  Int_t vzCut = 0;
  Int_t leptCut = 0;

  for(Long64_t jentry = 0; jentry < nentries; jentry++){
    c->hasTrackTree = false;
    if(montecarlo) c->hasGenParticleTree = false;

    c->GetEntry(jentry);

    totEv++;

    if(jentry%10000 == 0) std::cout << jentry << ", " << nEvtsOutTag << std::endl;

    if(!c->selectEvent()){
      selectCut++;
      continue;
    }

    if(TMath::Abs(c->evt.vz - meanVz) > 15){
      vzCut++;
      continue;
    }

    //add muon cut selection

    PFs pf = c->pf;
    Bool_t evtPass = false;

    for(Int_t pfIter = 0; pfIter < pf.nPFpart; pfIter++){
      if(TMath::Abs(pf.pfEta[pfIter]) > leptEtaCut) continue;
      if(pf.pfPt[pfIter] < leptPtCut) continue;

      if(TMath::Abs(pf.pfId[pfIter]) == 2 || TMath::Abs(pf.pfId[pfIter]) == 3){
	evtPass = true;
	break;
      }
    }

    if(!evtPass){
      leptCut++;
      continue;
    }

    //particle flow

    c->hasTrackTree = true;
    if(montecarlo) c->hasGenParticleTree = true;

    c->GetEntry(jentry);

    if(kHIMC == sType) pthatIni_ = c->akVs3Calo.pthat;
    else if(kPPMC == sType) pthatIni_ = c->ak3Calo.pthat;

    if(hi){
      hiEvtPlaneIni_ = c->evt.hiEvtPlanes[21];                                                  
      TComplex cn1((c->pf.sumpt[0])*(c->pf.vn[2][0]), c->pf.psin[2][0], true);                    
      TComplex cn2((c->pf.sumpt[14])*(c->pf.vn[2][14]), c->pf.psin[2][14], true);                
      TComplex cn = cn1+cn2;                                                                    
      psinIni_ = cn.Theta();      
    }      

    runIni_ = c->evt.run;
    evtIni_ = c->evt.evt;
    lumiIni_ = c->evt.lumi;

    if(hi) hiBinIni_ = c->evt.hiBin;

    nVs3Calo_ = 0;
    nVs4Calo_ = 0;
    nVs5Calo_ = 0;

    nT3_ = 0;
    nT4_ = 0;
    nT5_ = 0;

    Jets AlgJtCollection[3];
    
    if(hi){
      AlgJtCollection[0] = c->akVs3Calo;
      if(!isMinBias) AlgJtCollection[1] = c->akVs4Calo;
      if(!isMinBias) AlgJtCollection[2] = c->akVs5Calo;
    }
    else{
      AlgJtCollection[0] = c->ak3Calo;
      if(!isMinBias) AlgJtCollection[1] = c->ak4Calo;
      if(!isMinBias) AlgJtCollection[2] = c->ak5Calo;
    }


    for(Int_t Vs3CaloIter = 0; Vs3CaloIter < AlgJtCollection[0].nref; Vs3CaloIter++){
      if(AlgJtCollection[0].jtpt[Vs3CaloIter] < jtThreshPtCut) break;
      else if(TMath::Abs(AlgJtCollection[0].jteta[Vs3CaloIter]) > jtEtaCut) continue;
      
      Vs3CaloPt_[nVs3Calo_] = AlgJtCollection[0].jtpt[Vs3CaloIter];
      Vs3CaloPhi_[nVs3Calo_] = AlgJtCollection[0].jtphi[Vs3CaloIter];
      Vs3CaloEta_[nVs3Calo_] = AlgJtCollection[0].jteta[Vs3CaloIter];
      
      Vs3CaloTrkMax_[nVs3Calo_] = AlgJtCollection[0].trackMax[Vs3CaloIter];
      Vs3CaloRawPt_[nVs3Calo_] = AlgJtCollection[0].rawpt[Vs3CaloIter];

      if(montecarlo){
	Vs3CaloRefPt_[nVs3Calo_] = AlgJtCollection[0].refpt[Vs3CaloIter];
	Vs3CaloRefPhi_[nVs3Calo_] = AlgJtCollection[0].refphi[Vs3CaloIter];
	Vs3CaloRefEta_[nVs3Calo_] = AlgJtCollection[0].refeta[Vs3CaloIter];
	Vs3CaloRefPart_[nVs3Calo_] = AlgJtCollection[0].refparton_flavor[Vs3CaloIter];
      }
      
      nVs3Calo_++;
    }

    if(!isMinBias){
      for(Int_t Vs4CaloIter = 0; Vs4CaloIter < AlgJtCollection[1].nref; Vs4CaloIter++){
	if(AlgJtCollection[1].jtpt[Vs4CaloIter] < jtThreshPtCut) break;
	else if(TMath::Abs(AlgJtCollection[1].jteta[Vs4CaloIter]) > jtEtaCut) continue;
	
	Vs4CaloPt_[nVs4Calo_] = AlgJtCollection[1].jtpt[Vs4CaloIter];
	Vs4CaloPhi_[nVs4Calo_] = AlgJtCollection[1].jtphi[Vs4CaloIter];
	Vs4CaloEta_[nVs4Calo_] = AlgJtCollection[1].jteta[Vs4CaloIter];
	
	Vs4CaloTrkMax_[nVs4Calo_] = AlgJtCollection[1].trackMax[Vs4CaloIter];
	Vs4CaloRawPt_[nVs4Calo_] = AlgJtCollection[1].rawpt[Vs4CaloIter];
	
	if(montecarlo){
	  Vs4CaloRefPt_[nVs4Calo_] = AlgJtCollection[1].refpt[Vs4CaloIter];
	  Vs4CaloRefPhi_[nVs4Calo_] = AlgJtCollection[1].refphi[Vs4CaloIter];
	  Vs4CaloRefEta_[nVs4Calo_] = AlgJtCollection[1].refeta[Vs4CaloIter];
	  Vs4CaloRefPart_[nVs4Calo_] = AlgJtCollection[1].refparton_flavor[Vs4CaloIter];
	}
	
	nVs4Calo_++;
      }
      
      for(Int_t Vs5CaloIter = 0; Vs5CaloIter < AlgJtCollection[2].nref; Vs5CaloIter++){
	if(AlgJtCollection[2].jtpt[Vs5CaloIter] < jtThreshPtCut) break;
	else if(TMath::Abs(AlgJtCollection[2].jteta[Vs5CaloIter]) > jtEtaCut) continue;
	
	Vs5CaloPt_[nVs5Calo_] = AlgJtCollection[2].jtpt[Vs5CaloIter];
	Vs5CaloPhi_[nVs5Calo_] = AlgJtCollection[2].jtphi[Vs5CaloIter];
	Vs5CaloEta_[nVs5Calo_] = AlgJtCollection[2].jteta[Vs5CaloIter];
	
	Vs5CaloTrkMax_[nVs5Calo_] = AlgJtCollection[2].trackMax[Vs5CaloIter];
	Vs5CaloRawPt_[nVs5Calo_] = AlgJtCollection[2].rawpt[Vs5CaloIter];
	
	if(montecarlo){
	  Vs5CaloRefPt_[nVs5Calo_] = AlgJtCollection[2].refpt[Vs5CaloIter];
	  Vs5CaloRefPhi_[nVs5Calo_] = AlgJtCollection[2].refphi[Vs5CaloIter];
	  Vs5CaloRefEta_[nVs5Calo_] = AlgJtCollection[2].refeta[Vs5CaloIter];
	  Vs5CaloRefPart_[nVs5Calo_] = AlgJtCollection[2].refparton_flavor[Vs5CaloIter];
	}
	
	nVs5Calo_++;
      }
    }

    if(montecarlo){
      for(Int_t T3Iter = 0; T3Iter < AlgJtCollection[0].ngen; T3Iter++){
	if(AlgJtCollection[0].genpt[T3Iter] < jtThreshPtCut) break;
	else if(TMath::Abs(AlgJtCollection[0].geneta[T3Iter]) > jtEtaCut) continue;
	
	T3Pt_[nT3_] = AlgJtCollection[0].genpt[T3Iter];
	T3Phi_[nT3_] = AlgJtCollection[0].genphi[T3Iter];
	T3Eta_[nT3_] = AlgJtCollection[0].geneta[T3Iter];
	T3Part_[nT3_] = AlgJtCollection[0].refparton_flavor[AlgJtCollection[0].genmatchindex[T3Iter]];
	
	nT3_++;
      }

      if(!isMinBias){
	for(Int_t T4Iter = 0; T4Iter < AlgJtCollection[1].ngen; T4Iter++){
	  if(AlgJtCollection[1].genpt[T4Iter] < jtThreshPtCut) break;
	  else if(TMath::Abs(AlgJtCollection[1].geneta[T4Iter]) > jtEtaCut) continue;
	  
	  T4Pt_[nT4_] = AlgJtCollection[1].genpt[T4Iter];
	  T4Phi_[nT4_] = AlgJtCollection[1].genphi[T4Iter];
	  T4Eta_[nT4_] = AlgJtCollection[1].geneta[T4Iter];
	  T4Part_[nT4_] = AlgJtCollection[1].refparton_flavor[AlgJtCollection[1].genmatchindex[T4Iter]];
	  
	  nT4_++;
	}
	
	for(Int_t T5Iter = 0; T5Iter < AlgJtCollection[2].ngen; T5Iter++){
	  if(AlgJtCollection[2].genpt[T5Iter] < jtThreshPtCut) break;
	  else if(TMath::Abs(AlgJtCollection[2].geneta[T5Iter]) > jtEtaCut) continue;
	  
	  T5Pt_[nT5_] = AlgJtCollection[2].genpt[T5Iter];
	  T5Phi_[nT5_] = AlgJtCollection[2].genphi[T5Iter];
	  T5Eta_[nT5_] = AlgJtCollection[2].geneta[T5Iter];
	  T5Part_[nT5_] = AlgJtCollection[2].refparton_flavor[AlgJtCollection[2].genmatchindex[T5Iter]];
	  
	  nT5_++;
	}
      }
    }
   
    //Iterate over tracks

    nTrk_ = 0;

    Tracks trkCollection;
    trkCollection = c->track;
    
    for(Int_t trkEntry = 0; trkEntry < trkCollection.nTrk; trkEntry++){
      
      if(TMath::Abs(trkCollection.trkEta[trkEntry]) > 2.4) continue;
      
      //      if(trkCollection.trkPt[trkEntry] <= 0.5) continue;
	
      if(!trkCollection.highPurity[trkEntry]) continue;
	
      if(TMath::Abs(trkCollection.trkDz1[trkEntry]/trkCollection.trkDzError1[trkEntry]) > 3) continue;
	
      if(TMath::Abs(trkCollection.trkDxy1[trkEntry]/trkCollection.trkDxyError1[trkEntry]) > 3) continue;
	
      if(trkCollection.trkPtError[trkEntry]/trkCollection.trkPt[trkEntry] > 0.1) continue;
	
      trkPt_[nTrk_] = trkCollection.trkPt[trkEntry];
      trkPhi_[nTrk_] = trkCollection.trkPhi[trkEntry];
      trkEta_[nTrk_] = trkCollection.trkEta[trkEntry];
      
      //Grab proj. Pt Spectra For Tracks in each Event Subset    
	
      nTrk_++;
      if(nTrk_ > maxTracks - 1){
	printf("ERROR: Trk arrays not large enough.\n");
	return(1);
      }
    }

    nPF_ = 0;

    for(Int_t pfIter = 0; pfIter < pf.nPFpart; pfIter++){
      if(TMath::Abs(pf.pfEta[pfIter]) > 2.4) continue;

      //      if(pf.pfPt[pfIter] < 0.5) continue;

      pfPt_[nPF_] = pf.pfPt[pfIter];
      if(hi) pfVsPt_[nPF_] = pf.pfVsPt[pfIter];
      pfPhi_[nPF_] = pf.pfPhi[pfIter];
      pfEta_[nPF_] = pf.pfEta[pfIter];
      pfId_[nPF_] = pf.pfId[pfIter];

      nPF_++;
      if(nPF_ > maxPF - 1){
	printf("ERROR: PF arrays not large enough.\n");
	return(1);
      }
    }

    if(montecarlo){
      //Iterate over truth
      nGen_ = 0;
      
      GenParticles genCollection;
      genCollection = c->genparticle;
      
      for(Int_t genEntry = 0; genEntry < genCollection.mult; genEntry++){
	if(genCollection.chg[genEntry] == 0) continue;
	
	if(TMath::Abs(genCollection.eta[genEntry]) > 2.4) continue;
	  
	//	  if(genCollection.pt[genEntry] < 0.5) continue;
	
	genPt_[nGen_] = genCollection.pt[genEntry];
	genPhi_[nGen_] = genCollection.phi[genEntry];
	genEta_[nGen_] = genCollection.eta[genEntry];
	  
	nGen_++;
	if(nGen_ > maxEntrySim - 1){
	  printf("ERROR: Gen arrays not large enough.\n");
	  return(1);
	}	
      }
    }
    
    jetTreeIni_p->Fill();
    trackTreeIni_p->Fill();
    pfCandTreeIni_p->Fill();
    
    if(montecarlo) genTreeIni_p->Fill();

    Int_t nEvtsPassed = jetTreeIni_p->GetEntries();

    if(nEvtsPassed%nEvtPerFile == 0){
      outFile->cd();

      nEvtsOutTag++;

      jetTreeIni_p->Write("", TObject::kOverwrite);
      trackTreeIni_p->Write("", TObject::kOverwrite);
      pfCandTreeIni_p->Write("", TObject::kOverwrite);

      if(montecarlo) genTreeIni_p->Write("", TObject::kOverwrite);

      CleanupWJetIniSkim();
      outFile->Close();
      delete outFile;
      outFile = new TFile(Form("%s_%d_%d.root", outName, num, nEvtsOutTag), "RECREATE");
      InitWJetIniSkim(sType);
    }
  }

  std::cout << "totEv: " << totEv << std::endl;
  Int_t tempTot = totEv - selectCut;
  std::cout << "selectCut: " << tempTot << std::endl;
  tempTot = tempTot - vzCut;
  std::cout << "vzCut: " << tempTot << std::endl;
  tempTot = tempTot - leptCut;
  std::cout << "leptCut: " << tempTot << std::endl;

  outFile->cd();

  jetTreeIni_p->Write("", TObject::kOverwrite);
  trackTreeIni_p->Write("", TObject::kOverwrite);
  pfCandTreeIni_p->Write("", TObject::kOverwrite);    
  if(montecarlo) genTreeIni_p->Write("", TObject::kOverwrite);

  CleanupWJetIniSkim();
  outFile->Close();
  delete outFile;

  delete c;

  printf("Done.\n");
  return(0);
}


collisionType getCType(sampleType sType)
{
  switch (sType)
    {
    case kPPDATA:
    case kPPMC:
      return cPP;
    case kPADATA:
    case kPAMC:
      return cPPb;
    case kHIDATA:
    case kHIMC:
      return cPbPb;
    }
  return cPbPb; //probably a bad guess
}


int main(int argc, char *argv[])
{
  if(argc != 6)
    {
      std::cout << "Usage: makeWJetIniSkim <inputFile> <sType> <isMinBias> <outputFile> <#>"  << std::endl;
      std::cout << argc << std::endl;
      std::cout << argv[0] << ", "  << argv[1] << ", " << argv[2] << ", " << argv[3]<< ", " << argv[4] << ", " << argv[5] << std::endl;
      return 1;
    }

  int rStatus = -1;

  rStatus = makeWJetIniSkim(argv[1], sampleType(atoi(argv[2])), (Bool_t)(atoi(argv[3])), argv[4], atoi(argv[5]));

  return rStatus;
}
