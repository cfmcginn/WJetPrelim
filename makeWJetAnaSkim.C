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
const Float_t trkPtCut = 3.0;

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
      pfEvtPtSum_[0] += pfPt_[iter]*TMath::Cos(pfPhi_[iter]);
      pfEvtPtSum_[1] += pfPt_[iter]*TMath::Sin(pfPhi_[iter]);

      if(hi){
	pfEvtVsPtSum_[0] += pfVsPt_[iter]*TMath::Cos(pfPhi_[iter]);
	pfEvtVsPtSum_[1] += pfVsPt_[iter]*TMath::Sin(pfPhi_[iter]);
      }

      if(pfPt_[iter] > leptPtCut && TMath::Abs(pfEta_[iter]) < leptEtaCut){
	if(TMath::Abs(pfId_[iter]) == 3 && pfPt_[iter] > muonPt_){
	  muonPt_ = pfPt_[iter];
	  if(hi) muonVsPt_ = pfVsPt_[iter];
	  muonPhi_ = pfPhi_[iter];
	  muonEta_ = pfEta_[iter];
	}

	if(TMath::Abs(pfId_[iter]) == 2 && pfPt_[iter] > electronPt_ && (TMath::Abs(pfEta_[iter]) < 0.80 || TMath::Abs(pfEta_[iter]) > 1.4)){
	  electronPt_ = pfPt_[iter];
	  if(hi) electronVsPt_ = pfVsPt_[iter];
	  electronPhi_ = pfPhi_[iter];
	  electronEta_ = pfEta_[iter];
	}
      }
    }

    if(electronPt_ > muonPt_){
      leptPt_ = electronPt_;
      if(hi) leptVsPt_ = electronVsPt_;
      leptPhi_ = electronPhi_;
      leptEta_ = electronEta_;
    }
    else if(muonPt_ > electronPt_){
      leptPt_ = muonPt_;
      if(hi) leptVsPt_ = muonVsPt_;
      leptPhi_ = muonPhi_;
      leptEta_ = muonEta_;
    }

    for(Int_t iter = 0; iter < nPF_; iter++){
      if(TMath::Abs(pfId_[iter]) != 2 && TMath::Abs(pfId_[iter]) != 3 && pfPt_[iter] > check1Pt_){
	if(getDR(leptEta_, leptPhi_, pfEta_[iter], pfPhi_[iter]) > 0.5){
	  check1Pt_ = pfPt_[iter];
	  if(hi) check1VsPt_ = pfVsPt_[iter];
	  check1Phi_ = pfPhi_[iter];
	  check1Eta_ = pfEta_[iter];
	}
      }
    }

    check2Pt_ = prevLeptPt;
    if(hi) check2VsPt_ = prevLeptVsPt;
    check2Phi_ = prevLeptPhi;
    check2Eta_ = prevLeptEta;

    prevLeptPt = leptPt_;
    if(hi) prevLeptVsPt = leptVsPt_;
    prevLeptPhi = leptPhi_;
    prevLeptEta = leptEta_;

    for(Int_t iter = 0; iter < nTrk_; iter++){
      trkEvtPtSum_[0] += trkPt_[iter]*TMath::Cos(trkPhi_[iter]);
      trkEvtPtSum_[1] += trkPt_[iter]*TMath::Sin(trkPhi_[iter]);

      if(trkPt_[iter] > trkPtCut){
	trkCutEvtPtSum_[0] += trkPt_[iter]*TMath::Cos(trkPhi_[iter]);
	trkCutEvtPtSum_[1] += trkPt_[iter]*TMath::Sin(trkPhi_[iter]);
      }
    }

    if(leptPt_ > 0){    
      pfEvtPtMag_ = TMath::Sqrt(pfEvtPtSum_[0]*pfEvtPtSum_[0] + pfEvtPtSum_[1]*pfEvtPtSum_[1]);
      pfEvtPhi_ = TMath::ATan2(pfEvtPtSum_[1], pfEvtPtSum_[0]);
      pfNeuPhi_ = TMath::ATan2(-pfEvtPtSum_[1], -pfEvtPtSum_[0]);
      pfMt_ = TMath::Sqrt(2*leptPt_*pfEvtPtMag_*(1 - TMath::Cos(getDPHI(leptPhi_, pfNeuPhi_))));
      pfWPtSum_[0] = leptPt_*TMath::Cos(leptPhi_) - pfEvtPtSum_[0];
      pfWPtSum_[1] = leptPt_*TMath::Sin(leptPhi_) - pfEvtPtSum_[1];
      pfWPtMag_ = TMath::Sqrt(pfWPtSum_[0]*pfWPtSum_[0] + pfWPtSum_[1]*pfWPtSum_[1]);
      pfWPhi_ = TMath::ATan2(pfWPtSum_[1], pfWPtSum_[0]);

      if(hi){
	pfEvtVsPtMag_ = TMath::Sqrt(pfEvtVsPtSum_[0]*pfEvtVsPtSum_[0] + pfEvtVsPtSum_[1]*pfEvtVsPtSum_[1]);
	pfEvtVsPhi_ = TMath::ATan2(pfEvtVsPtSum_[1], pfEvtVsPtSum_[0]);
	pfVsNeuPhi_ = TMath::ATan2(-pfEvtVsPtSum_[1], -pfEvtVsPtSum_[0]);
	pfVsMt_ = TMath::Sqrt(2*leptPt_*pfEvtVsPtMag_*(1 - TMath::Cos(getDPHI(leptPhi_, pfVsNeuPhi_))));
	pfVsWPtSum_[0] = leptPt_*TMath::Cos(leptPhi_) - pfEvtVsPtSum_[0];
	pfVsWPtSum_[1] = leptPt_*TMath::Sin(leptPhi_) - pfEvtVsPtSum_[1];
	pfVsWPtMag_ = TMath::Sqrt(pfVsWPtSum_[0]*pfVsWPtSum_[0] + pfVsWPtSum_[1]*pfVsWPtSum_[1]);
	pfVsWPhi_ = TMath::ATan2(pfVsWPtSum_[1], pfVsWPtSum_[0]);
      }

      trkEvtPtMag_ = TMath::Sqrt(trkEvtPtSum_[0]*trkEvtPtSum_[0] + trkEvtPtSum_[1]*trkEvtPtSum_[1]);
      trkEvtPhi_ = TMath::ATan2(trkEvtPtSum_[1], trkEvtPtSum_[0]);
      trkNeuPhi_ = TMath::ATan2(-trkEvtPtSum_[1], -trkEvtPtSum_[0]);
      trkMt_ = TMath::Sqrt(2*leptPt_*trkEvtPtMag_*(1 - TMath::Cos(getDPHI(leptPhi_, trkNeuPhi_))));
      trkWPtSum_[0] = leptPt_*TMath::Cos(leptPhi_) - trkEvtPtSum_[0];
      trkWPtSum_[1] = leptPt_*TMath::Sin(leptPhi_) - trkEvtPtSum_[1];
      trkWPtMag_ = TMath::Sqrt(trkWPtSum_[0]*trkWPtSum_[0] + trkWPtSum_[1]*trkWPtSum_[1]);
      trkWPhi_ = TMath::ATan2(trkWPtSum_[1], trkWPtSum_[0]);

      trkCutEvtPtMag_ = TMath::Sqrt(trkCutEvtPtSum_[0]*trkCutEvtPtSum_[0] + trkCutEvtPtSum_[1]*trkCutEvtPtSum_[1]);
      trkCutEvtPhi_ = TMath::ATan2(trkCutEvtPtSum_[1], trkCutEvtPtSum_[0]);
      trkCutNeuPhi_ = TMath::ATan2(-trkCutEvtPtSum_[1], -trkCutEvtPtSum_[0]);
      trkCutMt_ = TMath::Sqrt(2*leptPt_*trkCutEvtPtMag_*(1 - TMath::Cos(getDPHI(leptPhi_, trkCutNeuPhi_))));
      trkCutWPtSum_[0] = leptPt_*TMath::Cos(leptPhi_) - trkCutEvtPtSum_[0];
      trkCutWPtSum_[1] = leptPt_*TMath::Sin(leptPhi_) - trkCutEvtPtSum_[1];
      trkCutWPtMag_ = TMath::Sqrt(trkCutWPtSum_[0]*trkCutWPtSum_[0] + trkCutWPtSum_[1]*trkCutWPtSum_[1]);
      trkCutWPhi_ = TMath::ATan2(trkCutWPtSum_[1], trkCutWPtSum_[0]);
    }
    if(check1Pt_ > 0){
      pfCheck1Mt_ = TMath::Sqrt(2*check1Pt_*pfEvtPtMag_*(1 - TMath::Cos(getDPHI(check1Phi_, pfNeuPhi_))));
      if(hi) pfVsCheck1Mt_ = TMath::Sqrt(2*check1Pt_*pfEvtVsPtMag_*(1 - TMath::Cos(getDPHI(check1Phi_, pfVsNeuPhi_))));
      trkCheck1Mt_ = TMath::Sqrt(2*check1Pt_*trkEvtPtMag_*(1 - TMath::Cos(getDPHI(check1Phi_, trkNeuPhi_))));
      trkCutCheck1Mt_ = TMath::Sqrt(2*check1Pt_*trkCutEvtPtMag_*(1 - TMath::Cos(getDPHI(check1Phi_, trkCutNeuPhi_))));
    }
    if(check2Pt_ > 0){
      pfCheck2Mt_ = TMath::Sqrt(2*check2Pt_*pfEvtPtMag_*(1 - TMath::Cos(getDPHI(check2Phi_, pfNeuPhi_))));
      if(hi) pfVsCheck2Mt_ = TMath::Sqrt(2*check2Pt_*pfEvtVsPtMag_*(1 - TMath::Cos(getDPHI(check2Phi_, pfVsNeuPhi_))));
      trkCheck2Mt_ = TMath::Sqrt(2*check2Pt_*trkEvtPtMag_*(1 - TMath::Cos(getDPHI(check2Phi_, trkNeuPhi_))));
      trkCutCheck2Mt_ = TMath::Sqrt(2*check2Pt_*trkCutEvtPtMag_*(1 - TMath::Cos(getDPHI(check2Phi_, trkCutNeuPhi_))));
    }

    Float_t dummyArr[2];

    getJtVar(nVs3Calo_, Vs3CaloPt_, Vs3CaloPhi_, Vs3CaloEta_, Vs3CaloRefPt_, Vs3CaloRefPhi_, Vs3CaloRefEta_, 0, montecarlo, false);
    getJtVar(nVs4Calo_, Vs4CaloPt_, Vs4CaloPhi_, Vs4CaloEta_, Vs4CaloRefPt_, Vs4CaloRefPhi_, Vs4CaloRefEta_, 1, montecarlo, false);
    getJtVar(nVs5Calo_, Vs5CaloPt_, Vs5CaloPhi_, Vs5CaloEta_, Vs5CaloRefPt_, Vs5CaloRefPhi_, Vs5CaloRefEta_, 2, montecarlo, false);
    getJtVar(nT3_, T3Pt_, T3Phi_, T3Eta_, dummyArr, dummyArr, dummyArr, 3, montecarlo, true);
    getJtVar(nT4_, T4Pt_, T4Phi_, T4Eta_, dummyArr, dummyArr, dummyArr, 4, montecarlo, true);
    getJtVar(nT5_, T5Pt_, T5Phi_, T5Eta_, dummyArr, dummyArr, dummyArr, 5, montecarlo, true);

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
