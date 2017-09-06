#include <iostream>
#include <sstream> 
#include <string> 
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include "style.h"
using namespace std;
TLegend* myLegend(Double_t x1, Double_t y1, Double_t x2, Double_t y2);

void plotROC_StageWise(string mvatype="CutsGA,CutsSA,LD,MLP,BDT", string colsys = "pp", float ptmin = 5, float ptmax = 7, int stage = 1, int isTrain = 0){
	TH1F* h[maxstyle];

	TFile* f;

  //TODO change MLP to be read from mvatype

  f = new TFile(Form("../myTMVA/ROOT/TMVA_MLP_%s_%.0f_%.0f.root",
        colsys.c_str(), ptmin, ptmax));
  std::cout << "\n1\n";

  TCanvas* c =  new TCanvas("c","",600,600);
	//c->SetLogx();
	c->cd();
	TH1F* hempty = new TH1F(Form("Stage%d_ROC",stage),
      Form("Stage %d ROC curves",stage),100,0,1.1);
	hempty->SetMaximum(1.2);
	hempty->SetMinimum(0.0);
	hempty->SetTitle("");
  TLatex* texCut = new TLatex(0.7,0.88,Form("Stage %d",stage)); 
  texCut->SetNDC();
  texCut->SetTextAlign(12);
  texCut->SetTextSize(0.05);
  texCut->SetTextFont(42);
  std::cout << "\n2\n";
  
  TLegend* leg = myLegend(0.15,0.15,0.5,0.6);

	vector<string> mvamethod;
	std::istringstream ss(mvatype);
  std::string item;
  while (std::getline(ss, item, ',')) {
    mvamethod.push_back(item);
  }
  std::cout << "\n3\n";

  string istrain;
  if(isTrain == 1) istrain = "trainingRej";
  else istrain = "rej";
  std::cout << "\n4\n";

	hempty->Draw();
	texCut->Draw();

  std::cout << "\n41\n";
  std::cout << "size " << int(mvamethod.size()) << std::endl;
	for(int i = 0; i < int(mvamethod.size()); i ++){
    std::cout << "for " << i << std::endl;
		h[i] = (TH1F*) f->Get(Form("dataset/Method_%s/%s/MVA_%s_%sBvsS", 
          mvamethod[i].c_str(), mvamethod[i].c_str(), 
          mvamethod[i].c_str(), istrain.c_str()));
    std::cout << "42\n";
		h[i]->SetLineColor(colorprofile[i]);
    std::cout << "43\n";
		h[i]->SetLineStyle(lineprofile[i]);
    std::cout << "44\n";
		h[i]->SetLineWidth(3);
    std::cout << "45\n";
		h[i]->Draw("same");
        leg->AddEntry(h[i],Form("method %s",mvamethod[i].c_str()),"l");
    std::cout << "46\n";
	}
    std::cout << "47\n";
	leg->Draw();
    std::cout << "48\n";
	c->SaveAs(Form("plotsROC/%sBvsS_Stage%d.pdf", istrain.c_str(), stage));
  std::cout << "\n5\n";

    if(isTrain == 1) istrain = "trainingEff";
    else istrain = "eff";
	hempty->Draw();
	texCut->Draw();
	for(int i = 0; i < int(mvamethod.size()); i ++){
		h[i] = (TH1F*)f->Get(Form("dataset/Method_%s/%s/MVA_%s_%sBvsS", mvamethod[i].c_str(), mvamethod[i].c_str(), mvamethod[i].c_str(), istrain.c_str()));
		h[i]->SetLineColor(colorprofile[i]);
		h[i]->SetLineStyle(lineprofile[i]);
		h[i]->SetLineWidth(3);
		h[i]->Draw("same");
	}
	leg->SetY1(0.3); leg->SetY2(1.1);
	leg->Draw();
	c->SaveAs(Form("plotsROC/%sBvsS_Stage%d.pdf", istrain.c_str(), stage));
}

TLegend* myLegend(Double_t x1, Double_t y1, Double_t x2, Double_t y2)
{
  TLegend* leg = new TLegend(x1,y1,x2,y2);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  return leg;
}
