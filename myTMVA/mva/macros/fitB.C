#include "../uti.h"
#include "../parameters.h"
#include "../../prefilter.h"
#include "TF1.h"

double setparam0=100.;
double setparam1=5.28;
double setparam2=0.05;
double setparam3=0.03;
double fixparam1=5.279;

double minhisto=5.0;
double maxhisto=6.0;
double nbinsmasshisto=50;
double binwidthmass=(maxhisto-minhisto)/nbinsmasshisto;
//TString npfile;
TString weight;
TString weightgen;
TString seldata;
TString selmc;
TString selmceff;
TString selmcgen;
TString collisionsystem;
TString lowptS;
TString lowptB;
float hiBinMin,hiBinMax,centMin,centMax;

double yield;
double back;
double signif;

float bdt;
int nbdt;

void fitter(int nBDT=0, float ptMin=7, float ptMax=10, int pbpb=0)
{
    nbdt = nBDT;
    bdt = (float)(low+nbdt*ep);
    isPbPb = (bool)pbpb;
    ptmin = ptMin;
    ptmax = ptMax;
    if(isPbPb)
    {
	inputSname = inputSname_PP;
	inputBname = inputBname_PP;
	if(ptmin == 7)
	{
	    inputSname = lowptS;
	    inputBname = lowptB;
	}
	mycut = mycut_PP;
	mycuts = mycuts_PP;
	mycutb = mycutb_PP;
	mycutg = mycutg_PP;
	colsyst = "PbPb";
	luminosity = lum_PP;
	npfile = NPout_PP;	
    }
    else
    {
	inputSname = inputSname_pp;
	inputBname = inputBname_pp;
	mycut = mycut_pp;
	mycuts = mycuts_pp;
	mycutb = mycutb_pp;
	mycutg = mycutg_pp;
	colsyst = "pp";
	luminosity = lum_pp;
	npfile = NPout_pp;	
    }

  int isMC = 0;
  TString trgselection="1";
  float centmin = 0;
  float centmax = 100;
  TString inputdata = inputBname;
  TString inputmc = inputSname;
  collisionsystem=colsyst;
  hiBinMin = centmin*2;
  hiBinMax = centmax*2;
  centMin = centmin;
  centMax = centmax;
    
  if(!isPbPb)
    {
      seldata = Form("%s&&%s&&BDT>%f",trgselection.Data(),mycut.Data(),bdt);
      selmceff = Form("%s",mycuts.Data());
      selmcgen = Form("%s",mycutg.Data());
    }
  else
    {
      seldata = Form("%s&&%s&&hiBin>%f&&hiBin<%f&&BDT>%f",trgselection.Data(),mycut.Data(),hiBinMin,hiBinMax,bdt);
      selmceff = Form("%s&&hiBin>%f&&hiBin<%f",mycuts.Data(),hiBinMin,hiBinMax);
      selmcgen = Form("%s&&hiBin>%f&&hiBin<%f",mycutg.Data(),hiBinMin,hiBinMax);
    }

  selmc = Form("%s&&BDT>%f",mycuts.Data(),bdt);

  void clean0 (TH1D* h);
  void getNPFnPar(TString npfname, float par[]);
  TF1* fit (TTree* nt, TTree* ntMC, double ptMin, double ptMax, int isMC,bool, TF1* &total,float centmin, float centmax, float NPpar[]);
  float NPpar[2];
  getNPFnPar(npfile, NPpar);
  std::cout<<"NP parameter 0: "<<NPpar[0]<<std::endl;
  std::cout<<"NP parameter 1: "<<NPpar[1]<<std::endl;

  weightgen="1";
  weight="1";
  
  std::cout<<"we are using weight="<<weight<<std::endl;
  
  TString bdtDATA = Form("../MVAfiles/BDT_%s_%.0f_%.0f_DATA.root",colsyst.Data(),ptmin,ptmax);
  TString bdtMC = Form("../MVAfiles/BDT_%s_%.0f_%.0f_MC.root",colsyst.Data(),ptmin,ptmax);

  TFile* inf = new TFile(inputdata.Data());
  TFile* infMC = new TFile(inputmc.Data());
  TFile* bdtf = new TFile(bdtDATA.Data());
  TFile* bdtfMC = new TFile(bdtMC.Data());

  TTree* bdtTree = (TTree*)bdtf->Get("bdtTree");
  TTree* bdtTreeMC = (TTree*)bdtfMC->Get("bdtTree");

  TTree* nt = (TTree*)inf->Get("ntKp");
  nt->AddFriend("ntHlt");
  nt->AddFriend("ntHi");
  nt->AddFriend("ntSkim");
  nt->AddFriend(bdtTree);

  TTree* ntGen = (TTree*)infMC->Get("ntGen");
  ntGen->AddFriend("ntHlt");
  ntGen->AddFriend("ntHi");
  ntGen->AddFriend(bdtTreeMC);

  TTree* ntMC = (TTree*)infMC->Get("ntKp");
  ntMC->AddFriend("ntHlt");
  ntMC->AddFriend("ntHi");
  ntMC->AddFriend("ntSkim");
  ntMC->AddFriend(ntGen);
  ntMC->AddFriend(bdtTreeMC);
 
  TF1 *totalmass;
  TF1* f = fit(nt,ntMC,ptmin,ptmax,isMC,isPbPb, totalmass,centmin, centmax, NPpar);
  std::cout << "/////////////////// SIGNAL: " << yield << std::endl;
  std::cout << "/////////////////// BACKGROUND: " << back << std::endl;
  std::cout << "/////////////////// SIGNIFICANCE: " << signif << std::endl;
}

void clean0(TH1D* h)
{
  for (int i=1;i<=h->GetNbinsX();i++)
    {
      if(h->GetBinContent(i)==0) h->SetBinError(i,1);
    }
}

void getNPFnPar(TString npfname, float par[]){
	TFile* npf = new TFile(npfname.Data());
	TF1* f = (TF1*)npf->Get("f1");
	par[0] = f->GetParameter(1);
	par[1] = f->GetParameter(2);
}

TF1 *fit(TTree *nt, TTree *ntMC, double ptMin, double ptMax, int isMC,bool isPbPb,TF1* &total,float centmin, float centmax, float NPpar[])
{
   //cout<<cut.Data()<<endl;
   static int count=0;
   count++;
   TCanvas* c= new TCanvas(Form("c%d",count),"",400,400);
   TH1D* h = new TH1D(Form("h-%d",count),"",nbinsmasshisto,minhisto,maxhisto);
   TH1D* hMCSignal = new TH1D(Form("hMCSignal-%d",count),"",nbinsmasshisto,minhisto,maxhisto);

   //TString iNP="7.26667e+00*Gaus(x,5.10472e+00,2.63158e-02)/(sqrt(2*3.14159)*2.63158e-02)+4.99089e+01*Gaus(x,4.96473e+00,9.56645e-02)/(sqrt(2*3.14159)*9.56645e-02)+3.94417e-01*(3.74282e+01*Gaus(x,5.34796e+00,3.11510e-02)+1.14713e+01*Gaus(x,5.42190e+00,1.00544e-01))";

   TString iNP=Form("TMath::Erf((x-%f)/%f)+1", NPpar[0], NPpar[1]);
   TF1* f = new TF1(Form("f%d",count),"[0]*([7]*Gaus(x,[1],[2])/(sqrt(2*3.14159)*[2])+(1-[7])*Gaus(x,[1],[8])/(sqrt(2*3.14159)*[8]))+[3]+[4]*x+[5]*("+iNP+")");

   if(isMC==1) nt->Project(Form("h-%d",count),"Bmass",Form("%s*(%s&&Bpt>%f&&Bpt<%f)","1",seldata.Data(),ptMin,ptMax));   
   else nt->Project(Form("h-%d",count),"Bmass",Form("(%s&&Bpt>%f&&Bpt<%f)",seldata.Data(),ptMin,ptMax));   
   ntMC->Project(Form("hMCSignal-%d",count),"Bmass",Form("%s&&Bpt>%f&&Bpt<%f",Form("%s&&Bgen==23333",selmc.Data()),ptMin,ptMax));
   clean0(h);
  
   f->SetParLimits(4,-1000,0);
   f->SetParLimits(2,0.01,0.05);
   f->SetParLimits(8,0.01,0.05);
   f->SetParLimits(7,0,1);
   f->SetParLimits(5,0,1000);

   f->SetParameter(0,setparam0);
   f->SetParameter(1,setparam1);
   f->SetParameter(2,setparam2);
   f->SetParameter(8,setparam3);
   f->FixParameter(1,fixparam1);
   f->FixParameter(5,0);
   h->GetEntries();

   hMCSignal->Fit(Form("f%d",count),"q","",minhisto,maxhisto);
   hMCSignal->Fit(Form("f%d",count),"q","",minhisto,maxhisto);
   f->ReleaseParameter(1);
   hMCSignal->Fit(Form("f%d",count),"L q","",minhisto,maxhisto);
   hMCSignal->Fit(Form("f%d",count),"L q","",minhisto,maxhisto);
   hMCSignal->Fit(Form("f%d",count),"L q","",minhisto,maxhisto);
   hMCSignal->Fit(Form("f%d",count),"L m","",minhisto,maxhisto);

   f->FixParameter(1,f->GetParameter(1));
   f->FixParameter(2,f->GetParameter(2));
   f->FixParameter(7,f->GetParameter(7));
   f->FixParameter(8,f->GetParameter(8));
   f->ReleaseParameter(5);
   
   h->Fit(Form("f%d",count),"q","",minhisto,maxhisto);
   h->Fit(Form("f%d",count),"q","",minhisto,maxhisto);
   f->ReleaseParameter(1);
   h->Fit(Form("f%d",count),"L q","",minhisto,maxhisto);
   h->Fit(Form("f%d",count),"L q","",minhisto,maxhisto);
   h->Fit(Form("f%d",count),"L q","",minhisto,maxhisto);
   h->Fit(Form("f%d",count),"L m","",minhisto,maxhisto);
 
      f->SetLineColor(kRed);
      f->SetLineWidth(2);

   TF1 *background = new TF1(Form("background%d",count),"[0]+[1]*x");
   background->SetParameter(0,f->GetParameter(3));
   background->SetParameter(1,f->GetParameter(4));
   background->SetLineColor(4);
   background->SetLineStyle(2);
   background->SetLineWidth(3);
   background->SetRange(minhisto,maxhisto);
   
   TF1 *Bkpi = new TF1(Form("fBkpi%d",count),"[0]*("+iNP+")");
   Bkpi->SetParameter(0,f->GetParameter(5));
   Bkpi->SetRange(minhisto,maxhisto);
   Bkpi->SetFillColor(kGreen+4);
   Bkpi->SetFillStyle(3005);
   Bkpi->SetLineColor(kGreen+4);
   Bkpi->SetLineWidth(3);

   TF1 *mass = new TF1(Form("fmass%d",count),"[0]*([3]*Gaus(x,[1],[2])/(sqrt(2*3.14159)*[2])+(1-[3])*Gaus(x,[1],[4])/(sqrt(2*3.14159)*[4]))");
   mass->SetParameters(f->GetParameter(0),f->GetParameter(1),f->GetParameter(2),f->GetParameter(7),f->GetParameter(8));
   mass->SetParError(0,f->GetParError(0));
   mass->SetParError(1,f->GetParError(1));
   mass->SetParError(2,f->GetParError(2));
   mass->SetParError(7,f->GetParError(7));
   mass->SetParError(8,f->GetParError(8));
   mass->SetLineColor(kOrange-3);
   mass->SetLineStyle(2);
   mass->SetLineWidth(3);
   mass->SetFillColor(kOrange-3);
   mass->SetFillStyle(3002);

  h->SetXTitle("m_{#mu#muK} (GeV/c^{2})");
  h->SetYTitle("Entries / (5 MeV/c^{2})");
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();
  h->SetAxisRange(0,h->GetMaximum()*1.4*1.2,"Y");
  h->GetXaxis()->SetTitleOffset(1.0);
  h->GetYaxis()->SetTitleOffset(1.3);
  h->GetXaxis()->SetLabelOffset(0.008);
  h->GetYaxis()->SetLabelOffset(0.008);
  h->GetXaxis()->SetTitleSize(0.060);
  h->GetYaxis()->SetTitleSize(0.060);
  h->GetXaxis()->SetTitleFont(42);
  h->GetYaxis()->SetTitleFont(42);
  h->GetXaxis()->SetLabelFont(42);
  h->GetYaxis()->SetLabelFont(42);
  h->GetXaxis()->SetLabelSize(0.06);
  h->GetYaxis()->SetLabelSize(0.06);
  h->Draw("e");
  double axisymin = -0.05;
  h->SetAxisRange(axisymin,h->GetMaximum()*1.2,"Y");
  h->GetXaxis()->SetNdivisions(-50205);
  int ci = TColor::GetColor("#000099");
  h->SetLineColor(ci);
  h->SetStats(0);
  
      h->SetMarkerStyle(20); // 24?
      h->SetMarkerSize(0.8);

   h->Draw("e");
   Bkpi->Draw("same");
   background->Draw("same");   
   mass->SetRange(minhisto,maxhisto);
   mass->Draw("same");
   f->Draw("same");

   double ep = 0.12;
   yield = mass->Integral(minhisto,maxhisto)/binwidthmass;
   back = background->Integral(5.279-ep,5.279+ep)/binwidthmass;
   signif = yield/sqrt(yield+back);

  TLegend* leg = new TLegend(0.55,0.45,0.875,0.76,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.05);
  leg->SetTextFont(42);
  leg->SetFillStyle(0);
  leg->AddEntry(h,"Data","pl");
  leg->AddEntry(f,"Fit","l");
  leg->AddEntry(mass,"B^{+} Signal","f");
  leg->AddEntry(background,"Combinatorial","l");
  leg->AddEntry(Bkpi,"B #rightarrow J/#psi X","f");
  leg->Draw("same");

  TLatex* texCms = new TLatex(0.225,0.87, "#scale[1.25]{CMS} Preliminary");
  texCms->SetNDC();
  texCms->SetTextAlign(12);
  texCms->SetTextSize(0.04);
  texCms->SetTextFont(42);
  texCms->Draw();

  TLatex* texCol;
  if(collisionsystem=="pp"||collisionsystem=="PP") texCol= new TLatex(0.39,0.94, Form("25.8 pb^{-1} (%s #sqrt{s_{NN}} = 5.02 TeV)","pp"));
  else texCol= new TLatex(0.35,0.94, Form("345 #mub^{-1} (%s #sqrt{s_{NN}} = 5.02 TeV)","PbPb"));
  texCol->SetNDC();
  texCol->SetTextSize(0.05);
  texCol->SetLineWidth(2);
  texCol->SetTextFont(42);
  texCol->Draw();

  TLatex* tex;

  tex = new TLatex(0.53,0.85,Form("%.1f < p_{T} < %.1f GeV/c",ptMin,ptMax));
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  if(centMax>0){
  TString texper="%";
  tex = new TLatex(0.225,0.78,Form("Centrality %.0f-%.0f%s",centMin,centMax,texper.Data()));//0.2612903,0.8425793
  tex->SetNDC();
  tex->SetTextColor(1);
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  }

  tex = new TLatex(0.77,0.78,"|y_{lab}| < 2.4");
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();

  tex = new TLatex(0.30,0.54,"B^{+}");
  tex->SetNDC();
  tex->SetTextFont(42);
  tex->SetTextSize(0.06);
  tex->SetLineWidth(2);
  tex->Draw();
     
   total=f;
   
  if(isPbPb) 
      c->SaveAs(Form("plots/BDT_PbPb_%d_%.0f_%.0f.pdf",nbdt,ptmin,ptmax));
  else 
      c->SaveAs(Form("plots/BDT_pp_%d_%.0f_%.0f.pdf",nbdt,ptmin,ptmax));

  return mass;
}


void fitB()
{
    double base_yield = 0;
    double eff_yield =0;

    for(int i=0; i<nBins; ++i)
    {
	TH1D* hpp_sig = new TH1D("hpp_sig",Form("pp_%.0f_%.0f;BDT;Eff/Signif",ptBins[i],ptBins[i+1]),N,low,high);
	TH1D* hpp_eff = new TH1D("hpp_eff","",N,low,high);

	for(int j=0; j<N; ++j)
	{
	    fitter(j,ptBins[i],ptBins[i+1],0);
	    hpp_sig->SetBinContent(j+1,signif);
		
	    if(j==0) base_yield = yield;

	    eff_yield = yield/base_yield;
	    hpp_eff->SetBinContent(j+1,eff_yield);
	}
	TCanvas* cpp = new TCanvas("cpp","",600,600);
	hpp_sig->Draw();
	hpp_eff->Draw("Same");
	cpp->SaveAs(Form("plots/pp_%.0f_%.0f.pdf",ptBins[i],ptBins[i+1]));
    }

    for(int i=0; i<nBins; ++i)
    {
	TH1D* hPP_sig = new TH1D("hPP_sig",Form("PP_%.0f_%.0f;BDT;Eff/Signif",ptBins[i],ptBins[i+1]),N,low,high);
	TH1D* hPP_eff = new TH1D("hPP_eff","",N,low,high);

	for(int j=0; j<N; ++j)
	{
	    fitter(j,ptBins[i],ptBins[i+1],1);
	    hPP_sig->SetBinContent(j+1,signif);
		
	    if(j==0) base_yield = yield;

	    eff_yield = yield/base_yield;
	    hPP_eff->SetBinContent(j+1,eff_yield);
	}
	TCanvas* cPP = new TCanvas("cPP","",600,600);
	hPP_sig->Draw();
	hPP_eff->Draw("Same");
	cPP->SaveAs(Form("plots/PbPb_%.0f_%.0f.pdf",ptBins[i],ptBins[i+1]));
    }

  
}

