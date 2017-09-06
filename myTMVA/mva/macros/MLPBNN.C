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
   Float_t         Btrk1Pt[1000];   //[Bsize]
   Float_t         Bchi2cl[1000];   //[Bsize]
   Float_t         BsvpvDistance[1000];   //[Bsize]
   Float_t         BsvpvDisErr[1000];   //[Bsize]
   Float_t         Bdtheta[1000];   //[Bsize]

   t->SetBranchAddress("Bsize", &Bsize);
   t->SetBranchAddress("Btrk1Pt", Btrk1Pt);
   t->SetBranchAddress("Bchi2cl", Bchi2cl);
   t->SetBranchAddress("BsvpvDistance", BsvpvDistance);
   t->SetBranchAddress("BsvpvDisErr", BsvpvDisErr);
   t->SetBranchAddress("Bdtheta", Bdtheta);
   
  
   std::vector<std::string> theInputVars;
   string a1="Btrk1Pt";
   string a2="Bchi2cl";
   string a3="BsvpvDistance/BsvpvDisErr";
   string a4="cos(Bdtheta)";
   theInputVars.push_back(a1);
   theInputVars.push_back(a2);
   theInputVars.push_back(a3);
   theInputVars.push_back(a4); 

   vector<double> inputValues;
//cout<<"kf1500"<<endl;
   ReadMLPBNN mva(theInputVars);
//cout<<"kf1600"<<endl;
  
   TFile *outf = new TFile(ofname,"recreate");
   TTree *mvaTree = new TTree(Form("%s_Tree",mvatype.Data()),mvatype.Data());

   double MVA[1000];
   mvaTree->Branch("Bsize",&Bsize,"Bsize/I");
   mvaTree->Branch(mvatype.Data(),MVA,Form("%s[Bsize]/D",mvatype.Data()));
   
   for (int i=0;i<t->GetEntries();i++){
      t->GetEntry(i);
      if (i%1000==0) cout <<i<<" / "<<t->GetEntries()<<endl;
      for (int j=0;j<Bsize;j++) {
         //cout <<i<<" "<<i<<" "<<Bsize<<endl;
         inputValues.clear();
         inputValues.push_back(Btrk1Pt[j]);
         inputValues.push_back(Bchi2cl[j]);
         inputValues.push_back(BsvpvDistance[j]/BsvpvDisErr[j]);
         inputValues.push_back(cos(Bdtheta[j]));
         MVA[j]=mva.GetMvaValue(inputValues);      
      }
      mvaTree->Fill();
   }
   outf->cd();
   outf->Write();
   outf->Close();
 

}
//void test(char *infname="/data/HeavyFlavourRun2/MC2015/Bntuple/PbPb/Bntuple20160628_Bpt7To10_Pythia8_BuToJpsiK_pthatweight.root",
void MLPBNN(int pbpb, float ptMin , float ptMax, string mvatype)
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
