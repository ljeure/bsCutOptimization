//#include "Bmeson_BDT.class.C"
//#include "TMVA_BDT_pp_7_10.class.C"
#include "../header.h"
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <vector>
#include <iostream>
#include "../../prefilter.h"
void makeoutput(TString infname, TString ofname, TString mvatype){

   TFile *inf = new TFile(infname);
   TTree *t = (TTree*)inf->Get("ntKp");
   
   Int_t           Bsize;
   Float_t         Btrk1Pt[1000];
   Float_t         Btrk2Pt[1000];
   Float_t         Bchi2cl[1000];
   Float_t         BsvpvDistance[1000];
   Float_t         BsvpvDisErr[1000];
   Float_t         Bd0[1000];
   Float_t         Bd0Err[1000];
   Float_t         Bdtheta[1000];
   Float_t         Bmu1pt[1000];
   Float_t         Bmu2pt[1000];
   Float_t         Bpt[1000];
   Float_t         Btktkmasst[1000];
   Float_t         Blxy[1000]; 

   t->SetBranchAddress("Bsize", &Bsize);
   t->SetBranchAddress("Btrk1Pt", Btrk1Pt);
   t->SetBranchAddress("Btrk2Pt", Btrk2Pt);
   t->SetBranchAddress("Bchi2cl", Bchi2cl);
   t->SetBranchAddress("BsvpvDistance", BsvpvDistance);
   t->SetBranchAddress("BsvpvDisErr", BsvpvDisErr);
   t->SetBranchAddress("Bd0", Bd0);
   t->SetBranchAddress("Bd0Err", Bd0Err);
   t->SetBranchAddress("Bdtheta", Bdtheta);
   t->SetBranchAddress("Bmu1pt", Bmu1pt);
   t->SetBranchAddress("Bmu2pt", Bmu2pt);
   t->SetBranchAddress("Bpt", Bpt);
   t->SetBranchAddress("Btktkmass", Btktkmass);
   t->SetBranchAddress("Blxy", Blxy);
  
   std::vector<std::string> theInputVars;
   string a0="Btrk1Pt";
   string a1="Btrk2Pt";
   string a2="Bchi2cl";
   string a3="BsvpvDistance/BsvpvDisErr";
   string a4="Bd0/Bd0Err";
   string a5="cos(Bdtheta)";
   string a6="Bmu1pt";
   string a7="Bmu2pt";
   string a8="Bpt";
   string a9="abs(Btktkmass-1.019455)";
   string a10="Blxy";
   theInputVars.push_back(a0);
   theInputVars.push_back(a1);
   theInputVars.push_back(a1);
   theInputVars.push_back(a2);
   theInputVars.push_back(a3);
   theInputVars.push_back(a4); 
   theInputVars.push_back(a5);
   theInputVars.push_back(a6);
   theInputVars.push_back(a7);
   theInputVars.push_back(a8);
   theInputVars.push_back(a9);
   theInputVars.push_back(a10); 

   vector<double> inputValues;
   ReadCutsGA mva(theInputVars);
  
   TFile *outf = new TFile(ofname,"recreate");
   TTree *mvaTree = new TTree(Form("%s_Tree",mvatype.Data()),mvatype.Data());

   double MVA[1000];
   mvaTree->Branch("Bsize",&Bsize,"Bsize/I");
   mvaTree->Branch(mvatype.Data(),MVA,Form("%s[Bsize]/D",mvatype.Data()));
   
   for (int i=0;i<t->GetEntries();i++){
      t->GetEntry(i);
      if (i%1000==0) cout <<i<<" / "<<t->GetEntries()<<endl;
      for (int j=0;j<Bsize;j++) {
         inputValues.clear();
         inputValues.push_back(Btrk1Pt[j]);
         inputValues.push_back(Btrk2Pt[j]);
         inputValues.push_back(Bchi2cl[j]);
         inputValues.push_back(BsvpvDistance[j]/BsvpvDisErr[j]);
         inputValues.push_back(Bd0[j]/Bd0Err[j]);
         inputValues.push_back(cos(Bdtheta[j]));
         inputValues.push_back(Bmu1pt[j]);
         inputValues.push_back(Bmu2pt[j]);
         inputValues.push_back(Bpt[j]);
         inputValues.push_back(abs(Btktkmass[j]-1.019455));
         inputValues.push_back(Blxy[j]);
         MVA[j]=mva.GetMvaValue(inputValues);      
      }
      mvaTree->Fill();
   }
   outf->cd();
   outf->Write();
   outf->Close();
 
}
//void test(char *infname="/data/HeavyFlavourRun2/MC2015/Bntuple/PbPb/Bntuple20160628_Bpt7To10_Pythia8_BuToJpsiK_pthatweight.root",
void CutsGA(int pbpb, float ptMin , float ptMax, string mvatype)
{
      MVAtype = (TString)mvatype;
      isPbPb = (bool)pbpb;
      ptmin = ptMin;
      ptmax = ptMax;
      if(isPbPb)
      {
          inputSname = inputSname_PP;
          inputBname = inputBname_PP;
          colsyst = "PbPb";
      }
      else{
          inputSname = inputSname_pp;
          inputBname = inputBname_pp;
          colsyst = "pp";
      }

    TString outfile1 =Form("../MVAfiles/%s_%s_%.0f_%.0f_DATA.root",MVAtype.Data(),colsyst.Data(),ptMin,ptMax) ;
    makeoutput(inputBname,outfile1,MVAtype);

    TString outfile2 =Form("../MVAfiles/%s_%s_%.0f_%.0f_MC.root",MVAtype.Data(),colsyst.Data(),ptMin,ptMax) ;
    makeoutput(inputSname,outfile2,MVAtype);
  
}
