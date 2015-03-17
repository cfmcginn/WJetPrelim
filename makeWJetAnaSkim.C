//=============================================
// Author: Chris McGinn / Doga Gulhan
// 
// WJet Analysis Skim Class (MC)
//
//=============================================

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "cfmWJetAnaSkim.h"
#include "stdlib.h"
#include <fstream>
#include "TComplex.h"
#include "etaPhiFunc.h"

const Float_t leptPtCut = 15.0;
const Float_t leptEtaCut = 2.1;

int makeWJetAnaSkim(std::string fList = "", sampleType sType = kHIDATA, Int_t num = 0)
{
  //Define MC or Data
  Bool_t montecarlo = isMonteCarlo(sType);
  Bool_t hi = isHI(sType);

  std::cout << sType << std::endl;
  std::cout << montecarlo << std::endl;

  std::string buffer;
  std::vector<std::string> listOfFiles;
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

  std::cout << "FileJob: " << listOfFiles[num] << std::endl;

  TFile* iniSkim_p = new TFile(listOfFiles[num].data(), "READ");

  GetWJetIniSkim(iniSkim_p, sType);

  std::cout << "IniSkim Loaded" << std::endl;
  
  std::string outName = listOfFiles[num];
  const std::string cutString = "/";
  const std::string iniString = "Ini";
  std::size_t strIndex = 0;

  std::cout << "Cull string" << std::endl;

  while(true){
    strIndex = outName.find(cutString);

    if(strIndex == std::string::npos) break;

    outName.replace(0, strIndex + 1, "");
  }

  std::cout << "Replace string" << std::endl;

  strIndex = outName.find(iniString);
  if(!(strIndex == std::string::npos)){
    outName.replace(strIndex, iniString.length(), "Ana"); 
  }

  std::cout << "Output name: " << outName.c_str() << std::endl;
  TFile *outFile = new TFile(outName.c_str(), "RECREATE");
  InitWJetAnaSkim(sType);

  Long64_t nentries = jetTreeIni_p->GetEntries();

  std::cout << nentries << std::endl;

  Float_t prevLeptPt = 0;
  Float_t prevLeptVsPt = 0;
  Float_t prevLeptPhi = 0;
  Float_t prevLeptEta = 0;

  for(Long64_t jentry = 0; jentry < nentries; jentry++){
    trackTreeIni_p->GetEntry(jentry);
    pfCandTreeIni_p->GetEntry(jentry);
    jetTreeIni_p->GetEntry(jentry);
    if(montecarlo) genTreeIni_p->GetEntry(jentry);

    if(jentry%1000 == 0) std::cout << jentry << std::endl;

    InitAnaVar();

    run_ = runIni_;
    evt_ = evtIni_;
    lumi_ = lumiIni_;

    if(montecarlo) pthat_ = pthatIni_;
    
    if(hi){
      hiBin_ = hiBinIni_;
      hiEvtPlane_ = hiEvtPlaneIni_;
      psin_ = psinIni_;
    }


    for(Int_t iter = 0; iter < nPF_; iter++){
      pfEvtPtSum_[0] += pfPt_[iter]*cos(pfPhi_[iter]);
      pfEvtPtSum_[1] += pfPt_[iter]*sin(pfPhi_[iter]);

      pfEvtVsPtSum_[0] += pfVsPt_[iter]*cos(pfPhi_[iter]);
      pfEvtVsPtSum_[1] += pfVsPt_[iter]*sin(pfPhi_[iter]);

      if(pfPt_[iter] > leptPtCut && TMath::Abs(pfEta_[iter]) < leptEtaCut){
	if(TMath::Abs(pfId_[iter]) == 3 && pfPt_[iter] > muonPt_){
	  muonPt_ = pfPt_[iter];
	  muonVsPt_ = pfVsPt_[iter];
	  muonPhi_ = pfPhi_[iter];
	  muonEta_ = pfEta_[iter];
	}

	if(TMath::Abs(pfId_[iter]) == 2 && pfPt_[iter] > electronPt_){
	  electronPt_ = pfPt_[iter];
	  electronVsPt_ = pfVsPt_[iter];
	  electronPhi_ = pfPhi_[iter];
	  electronEta_ = pfEta_[iter];
	}
      }
    }

    if(electronPt_ > muonPt_){
      leptPt_ = electronPt_;
      leptVsPt_ = electronVsPt_;
      leptPhi_ = electronPhi_;
      leptEta_ = electronEta_;
    }
    else if(muonPt_ > electronPt_){
      leptPt_ = muonPt_;
      leptVsPt_ = muonVsPt_;
      leptPhi_ = muonPhi_;
      leptEta_ = muonEta_;
    }

    for(Int_t iter = 0; iter < nPF_; iter++){
      if(TMath::Abs(pfId_[iter]) != 2 && TMath::Abs(pfId_[iter]) != 3 && pfPt_[iter] > check1Pt_){
	if(getDR(leptEta_, leptPhi_, pfEta_[iter], pfPhi_[iter]) > 0.5){
	  check1Pt_ = pfPt_[iter];
	  check1VsPt_ = pfVsPt_[iter];
	  check1Phi_ = pfPhi_[iter];
	  check1Eta_ = pfEta_[iter];
	}
      }
    }

    check2Pt_ = prevLeptPt;
    check2VsPt_ = prevLeptVsPt;
    check2Phi_ = prevLeptPhi;
    check2Eta_ = prevLeptEta;

    prevLeptPt = leptPt_;
    prevLeptVsPt = leptVsPt_;
    prevLeptPhi = leptPhi_;
    prevLeptEta = leptEta_;

    for(Int_t iter = 0; iter < nTrk_; iter++){
      trkEvtPtSum_[0] += trkPt_[iter]*cos(trkPhi_[iter]);
      trkEvtPtSum_[1] += trkPt_[iter]*sin(trkPhi_[iter]);
    }

    if(leptPt_ > 0){    
      pfEvtPtMag_ = TMath::Sqrt(pfEvtPtSum_[0]*pfEvtPtSum_[0] + pfEvtPtSum_[1]*pfEvtPtSum_[1]);
      pfEvtPhi_ = TMath::ATan2(pfEvtPtSum_[1], pfEvtPtSum_[0]);
      pfNeuPhi_ = TMath::ATan2(-pfEvtPtSum_[1], -pfEvtPtSum_[0]);
      pfMt_ = TMath::Sqrt(2*leptPt_*pfEvtPtMag_*(1 - cos(getDPHI(leptPhi_, pfNeuPhi_))));
      pfWPtSum_[0] = leptPt_*cos(leptPhi_) - pfEvtPtSum_[0];
      pfWPtSum_[1] = leptPt_*sin(leptPhi_) - pfEvtPtSum_[1];
      pfWPtMag_ = TMath::Sqrt(pfWPtSum_[0]*pfWPtSum_[0] + pfWPtSum_[1]*pfWPtSum_[1]);
      pfWPhi_ = TMath::ATan2(pfWPtSum_[1], pfWPtSum_[0]);

      pfEvtVsPtMag_ = TMath::Sqrt(pfEvtVsPtSum_[0]*pfEvtVsPtSum_[0] + pfEvtVsPtSum_[1]*pfEvtVsPtSum_[1]);
      pfEvtVsPhi_ = TMath::ATan2(pfEvtVsPtSum_[1], pfEvtVsPtSum_[0]);
      pfVsNeuPhi_ = TMath::ATan2(-pfEvtVsPtSum_[1], -pfEvtVsPtSum_[0]);
      pfVsMt_ = TMath::Sqrt(2*leptPt_*pfEvtVsPtMag_*(1 - cos(getDPHI(leptPhi_, pfVsNeuPhi_))));
      pfVsWPtSum_[0] = leptPt_*cos(leptPhi_) - pfVsEvtPtSum_[0];
      pfVsWPtSum_[1] = leptPt_*sin(leptPhi_) - pfVsEvtPtSum_[1];
      pfVsWPtMag_ = TMath::Sqrt(pfVsWPtSum_[0]*pfVsWPtSum_[0] + pfVsWPtSum_[1]*pfVsWPtSum_[1]);
      pfVsWPhi_ = TMath::ATan2(pfVsWPtSum_[1], pfVsWPtSum_[0]);

      trkEvtPtMag_ = TMath::Sqrt(trkEvtPtSum_[0]*trkEvtPtSum_[0] + trkEvtPtSum_[1]*trkEvtPtSum_[1]);
      trkEvtPhi_ = TMath::ATan2(trkEvtPtSum_[1], trkEvtPtSum_[0]);
      trkNeuPhi_ = TMath::ATan2(-trkEvtPtSum_[1], -trkEvtPtSum_[0]);
      trkMt_ = TMath::Sqrt(2*leptPt_*trkEvtPtMag_*(1 - cos(getDPHI(leptPhi_, trkNeuPhi_))));
      trkWPtSum_[0] = leptPt_*cos(leptPhi_) - trkEvtPtSum_[0];
      trkWPtSum_[1] = leptPt_*sin(leptPhi_) - trkEvtPtSum_[1];
      trkWPtMag_ = TMath::Sqrt(trkWPtSum_[0]*trkWPtSum_[0] + trkWPtSum_[1]*trkWPtSum_[1]);
      trkWPhi_ = TMath::ATan2(trkWPtSum_[1], trkWPtSum_[0]);
    }
    if(check1Pt_ > 0){
      pfCheck1Mt_ = TMath::Sqrt(2*check1Pt_*pfEvtPtMag_*(1 - cos(getDPHI(check1Phi_, pfNeuPhi_))));
      pfVsCheck1Mt_ = TMath::Sqrt(2*check1Pt_*pfEvtVsPtMag_*(1 - cos(getDPHI(check1Phi_, pfVsNeuPhi_))));
      trkCheck1Mt_ = TMath::Sqrt(2*check1Pt_*trkEvtPtMag_*(1 - cos(getDPHI(check1Phi_, trkNeuPhi_))));
    }
    if(check2Pt_ > 0){
      pfCheck2Mt_ = TMath::Sqrt(2*check2Pt_*pfEvtPtMag_*(1 - cos(getDPHI(check2Phi_, pfNeuPhi_))));
      pfVsCheck2Mt_ = TMath::Sqrt(2*check2Pt_*pfEvtVsPtMag_*(1 - cos(getDPHI(check2Phi_, pfVsNeuPhi_))));
      trkCheck2Mt_ = TMath::Sqrt(2*check2Pt_*trkEvtPtMag_*(1 - cos(getDPHI(check2Phi_, trkNeuPhi_))));
    }

    Float_t dummyArr[];

    getJtVar(nVs3Calo_, Vs3CaloPt_, Vs3CaloPhi_, Vs3CaloEta_, Vs3CaloRefPt_, Vs3CaloRefPhi_, Vs3CaloRefEta_, 0, montecarlo, false);
    getJtVar(nVs4Calo_, Vs4CaloPt_, Vs4CaloPhi_, Vs4CaloEta_, Vs4CaloRefPt_, Vs4CaloRefPhi_, Vs4CaloRefEta_, 1, montecarlo, false);
    getJtVar(nT3_, T3Pt_, T3Phi_, T3Eta_, dummyArr, dummyArr, dummyArr, 2, montecarlo, true);
    getJtVar(nT4_, T4Pt_, T4Phi_, T4Eta_, dummyArr, dummyArr, dummyArr, 3, montecarlo, true);

    trackTreeAna_p->Fill();
    pfCandTreeAna_p->Fill();
    jetTreeAna_p->Fill();
    if(montecarlo) genTreeAna_p->Fill();
  }
  
  outFile->cd(); 
  trackTreeAna_p->Write("", TObject::kOverwrite);
  pfCandTreeAna_p->Write("", TObject::kOverwrite);
  jetTreeAna_p->Write("", TObject::kOverwrite);
  if(montecarlo) genTreeAna_p->Write("", TObject::kOverwrite);

  CleanupWJetAnaSkim();
  outFile->Close();
  delete outFile;

  iniSkim_p->Close();
  delete iniSkim_p;

  printf("Done.\n");
  return(0);
}


int main(int argc, char *argv[])
{
  if(argc != 4){
    std::cout << "Usage: sortForest <inputFile> <sampleType> <#>" << std::endl;
    return 1;
  }

  int rStatus = -1;

  rStatus = makeWJetAnaSkim(argv[1], sampleType(atoi(argv[2])), atoi(argv[3]));

  return rStatus;
}
