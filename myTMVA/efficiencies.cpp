//--------------------
// c++ headers
//--------------------
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>
//--------------------
//Specific headers
//--------------------
#include "TLine.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLorentzVector.h"
#include <vector>
#include "TApplication.h"
#include "TChain.h"
#include "TFile.h"
#include <vector>
#include "TROOT.h"
using namespace std;

void efficiencies()
{
    gROOT->SetBatch();
    ///define pt bin
    //int ptbins[]= {7,10,15,20,30,50};
    int ptbins[]= {5,10,15,20,60};


    for(int i=0; i<4; i++)
    {
        stringstream bdt_pp_name,GA_pp_name,SA_pp_name,LD_pp_name,MLP_pp_name;
        stringstream bdt_PbPb_name,GA_PbPb_name,SA_PbPb_name,LD_PbPb_name,MLP_PbPb_name;
        stringstream outname,outname2;

        outname<<"all_pp_for_pt_"<<ptbins[i]<<"_"<< ptbins[i+1]<<".pdf";
        outname2<<"all_PbPb_for_pt_"<<ptbins[i]<<"_"<<ptbins[i+1]<<".pdf";

        ///PP collision
       /* bdt_pp_name<<"ROOT/TMVA_BDT_pp_"<<ptbins[i]<<"_"<<ptbins[i+1]<<".root";
        GA_pp_name<<"ROOT/TMVA_CutsGA_pp_"<<ptbins[i]<<"_"<<ptbins[i+1]<<".root";
        SA_pp_name<<"ROOT/TMVA_CutsSA_pp_"<<ptbins[i]<<"_"<<ptbins[i+1]<<".root";
        LD_pp_name<<"ROOT/TMVA_LD_pp_"<<ptbins[i]<<"_"<<ptbins[i+1]<<".root";
        MLP_pp_name<<"ROOT/TMVA_MLP_pp_"<<ptbins[i]<<"_"<<ptbins[i+1]<<".root";
        *////PbPb collision
        bdt_PbPb_name<<"ROOT/TMVA_BDT_PbPb_"<<ptbins[i]<<"_"<<ptbins[i+1]<<".root";
        GA_PbPb_name<<"ROOT/TMVA_CutsGA_PbPb_"<<ptbins[i]<<"_"<<ptbins[i+1]<<".root";
        SA_PbPb_name<<"ROOT/TMVA_CutsSA_PbPb_"<<ptbins[i]<<"_"<<ptbins[i+1]<<".root";
        LD_PbPb_name<<"ROOT/TMVA_LD_PbPb_"<<ptbins[i]<<"_"<<ptbins[i+1]<<".root";
        MLP_PbPb_name<<"ROOT/TMVA_MLP_PbPb_"<<ptbins[i]<<"_"<<ptbins[i+1]<<".root";

       /* TFile *f1 = new TFile(bdt_pp_name.str().c_str());
        TFile *f2 = new TFile(GA_pp_name.str().c_str());
        TFile *f3 = new TFile(SA_pp_name.str().c_str());
        TFile *f4 = new TFile(LD_pp_name.str().c_str());
        TFile *f5 = new TFile(MLP_pp_name.str().c_str());
*/
        TFile *f6  = new TFile(bdt_PbPb_name.str().c_str());
        TFile *f7  = new TFile(GA_PbPb_name.str().c_str());
        TFile *f8  = new TFile(SA_PbPb_name.str().c_str());
        TFile *f9  = new TFile(LD_PbPb_name.str().c_str());
        TFile *f10 = new TFile(MLP_PbPb_name.str().c_str());



        /// pp histograms
/*        TH1 *h1= (TH1*)f1->Get("Method_BDT/BDT/MVA_BDT_rejBvsS");
        TH1 *h2= (TH1*)f2->Get("Method_Cuts/CutsGA/MVA_CutsGA_rejBvsS");
        TH1 *h3= (TH1*)f3->Get("Method_Cuts/CutsSA/MVA_CutsSA_rejBvsS");
        TH1 *h4= (TH1*)f4->Get("Method_LD/LD/MVA_LD_rejBvsS");
        TH1 *h5= (TH1*)f5->Get("Method_MLP/MLP/MVA_MLP_rejBvsS");
 */       ///PbPb histograms
        TH1 *h6= (TH1*)f6->Get("Method_BDT/BDT/MVA_BDT_rejBvsS");
        TH1 *h7= (TH1*)f7->Get("Method_Cuts/CutsGA/MVA_CutsGA_rejBvsS");
        TH1 *h8= (TH1*)f8->Get("Method_Cuts/CutsSA/MVA_CutsSA_rejBvsS");
        TH1 *h9= (TH1*)f9->Get("Method_LD/LD/MVA_LD_rejBvsS");
        TH1 *h10= (TH1*)f10->Get("Method_MLP/MLP/MVA_MLP_rejBvsS");

        ///set colors
  /*      h2->SetLineColor(kRed);
        h1->SetLineWidth(3);
        h2->SetLineWidth(3);
        h3->SetLineWidth(3);
        h4->SetLineWidth(3);
        h5->SetLineWidth(3);
        h1->SetLineColor(kRed);
        h2->SetLineColor(kBlack);
        h3->SetLineColor(kMagenta);
        h4->SetLineColor(kGreen);
        h5->SetLineColor(kBlue);
*/
        h6->SetLineColor(kRed);
        h7->SetLineColor(kBlack);
        h8->SetLineColor(kMagenta);
        h9->SetLineColor(kGreen);
        h10->SetLineColor(kBlue);
        h6->SetLineWidth(3);
        h7->SetLineWidth(3);
        h8->SetLineWidth(3);
        h9->SetLineWidth(3);
        h10->SetLineWidth(3);

       /* TLegend *leg =new TLegend(0.1,0.1,0.48,0.5);
        leg->SetHeader("pp");
        leg->SetHeader("PbPb");
        leg->AddEntry(h1,"Method_BDT","l");
        leg->AddEntry(h2,"Method_CutsGA","l");
        leg->AddEntry(h3,"Method_CutsSA","l");
        leg->AddEntry(h4,"Method_LD","l");
        leg->AddEntry(h5,"Method_MLP","l");


        TCanvas *c1=new TCanvas("mycanvas","11",800,600);
        c1->cd(1);
        h1->Draw("C");
        h2->Draw("Csame");
        h3->Draw("Csame");
        h4->Draw("Csame");
        h5->Draw("Csame");
        leg->Draw();
        c1->SaveAs(outname.str().c_str());
        //  c1->Clear();

  */     TLegend *legPbPb =new TLegend(0.1,0.1,0.48,0.5);
        legPbPb->SetHeader("PbPb");
        legPbPb->AddEntry(h6,"Method_BDT","l");
        legPbPb->AddEntry(h7,"Method_CutsGA","l");
        legPbPb->AddEntry(h8,"Method_CutsSA","l");
        legPbPb->AddEntry(h9,"Method_LD","l");
        legPbPb->AddEntry(h10,"Method_MLP","l");
        
        TCanvas *c2=new TCanvas("mycanvas2","22",800,600);
        c2->cd(1);
        h6->Draw("C");
        h7->Draw("Csame");
        h8->Draw("Csame");
        h9->Draw("Csame");
        h10->Draw("Csame");
        legPbPb->Draw();
        c2->SaveAs(outname2.str().c_str());
       cout<<" :)"<<endl;



    }

}

int main(int argc, char* argv[])
{
    TApplication mainApp("mainApp", &argc, argv);
    efficiencies();
    mainApp.Run();
    return 0;
}
