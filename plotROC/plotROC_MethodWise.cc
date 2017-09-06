#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include "style.h"
using namespace std;
TLegend* myLegend(Double_t x1, Double_t y1, Double_t x2, Double_t y2);
void plotROC_MethodWise(string mvatype="CutsGA", string colsys = "pp", float ptmin = 5, float ptmax = 7, int nvIni = 0, int nvar = 1, int isTrain = 0){
	TFile* f[maxstyle];
	TH1F* h[maxstyle];

  std::cout << "\nopens Methodwise file\n";
    
  TCanvas* c =  new TCanvas("c","",600,600);
	//c->SetLogx();
	c->cd();
	TH1F* hempty = new TH1F(Form("%s_ROC",mvatype.c_str()),Form("%s ROC curves",mvatype.c_str()),100,0,1.1);
	hempty->SetMaximum(1.2);
	hempty->SetMinimum(0.0);
	hempty->SetTitle("");
    TLatex* texCut = new TLatex(0.7,0.88,Form("%s",mvatype.c_str())); 
    texCut->SetNDC();    texCut->SetTextAlign(12);    texCut->SetTextSize(0.05);    texCut->SetTextFont(42);
    TLegend* leg = myLegend(0.15,0.15,0.5,0.6);

	string istrain;
	if(isTrain == 1) istrain = "trainingRej";
	else istrain = "rej";
	hempty->Draw();
	texCut->Draw();
	for(int i = nvIni; i < nvar; i ++){
		f[i] = new TFile(Form("../myTMVA/ROOT/TMVA_%s_%.0f_%.0f.root",colsys.c_str(), ptmin, ptmax));
		h[i] = (TH1F*)f[i]->Get(Form("dataset/Method_%s/%s/MVA_%s_%sBvsS", mvatype.c_str(), mvatype.c_str(), mvatype.c_str(), istrain.c_str()));
		h[i]->SetLineColor(colorprofile[i]);
        h[i]->SetLineStyle(lineprofile[i]);
		h[i]->SetLineWidth(3);
		h[i]->Draw("same");
        leg->AddEntry(h[i],Form("variable lv %d",i+1),"l");
	}
	leg->Draw();
	c->SaveAs(Form("plotsROC/%sBvsS_%s.pdf", istrain.c_str(), mvatype.c_str()));

	if(isTrain == 1) istrain = "trainingEff";
	else istrain = "eff";
    hempty->Draw();
	texCut->Draw();
	for(int i = nvIni; i < nvar; i ++){
		f[i] = new TFile(Form("../myTMVA/ROOT/TMVA_%s_%.0f_%.0f_varStage%d.root",colsys.c_str(), ptmin, ptmax, i+1));
		h[i] = (TH1F*)f[i]->Get(Form("dataset/Method_%s/%s/MVA_%s_%sBvsS", mvatype.c_str(), mvatype.c_str(), mvatype.c_str(), istrain.c_str()));
		h[i]->SetLineColor(colorprofile[i]);
        h[i]->SetLineStyle(lineprofile[i]);
		h[i]->SetLineWidth(3);
		h[i]->Draw("same");
	}
    leg->SetY1(0.3); leg->SetY2(1.1);
	leg->Draw();
	c->SaveAs(Form("plotsROC/%sBvsS_%s.pdf", istrain.c_str(), mvatype.c_str()));
}

TLegend* myLegend(Double_t x1, Double_t y1, Double_t x2, Double_t y2)
{
  TLegend* leg = new TLegend(x1,y1,x2,y2);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  return leg;
}
