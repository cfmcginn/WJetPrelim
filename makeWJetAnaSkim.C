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

  for(Long64_t jentry = 0; jentry < nentries; jentry++){
    jetTreeIni_p->GetEntry(jentry);
    trackTreeIni_p->GetEntry(jentry);
    if(montecarlo) genTreeIni_p->GetEntry(jentry);

    if(jentry%1000 == 0) std::cout << jentry << std::endl;

    run_ = runIni_;
    evt_ = evtIni_;
    lumi_ = lumiIni_;

    if(montecarlo) pthat_ = pthatIni_;
    
    if(hi){
      hiBin_ = hiBinIni_;
      hiEvtPlane_ = hiEvtPlaneIni_;
      psin_ = psinIni_;
    }

    jetTreeAna_p->Fill();
    trackTreeAna_p->Fill();
    if(montecarlo) genTreeAna_p->Fill();
  }
  
  outFile->cd();
  jetTreeAna_p->Write("", TObject::kOverwrite);
  trackTreeAna_p->Write("", TObject::kOverwrite);
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
