//#include "uti.h"
#include"TString.h"
const int NmaxFonll = 1197; //fonll data points number
Float_t central[NmaxFonll],pt[NmaxFonll];
const int NEff = 300;
Double_t effS[NEff], effB[NEff],effSig[NEff],effBac[NEff];
const int NmaxVar = 11;
std::vector<TString> cuts;
std::vector<Double_t> cutval[NmaxVar];
TString varval[NmaxVar];

Int_t Nsigma = 2;
//Float_t ptmin,ptmax,raa;
float raa;
/*TString inputSname = "/afs/cern.ch/work/c/cdozen/UP_BFINDER/CMSSW_7_5_8_patch3/src/Bfinder/Bfinder/Bntuple/test.root";
TString inputBname = "/data/wangj/Data2015/Bntuple/pp/ntB_EvtBase_20160420_BfinderData_pp_20160419_bPt0jpsiPt0tkPt0p5.root";
TString mycuts = "Bgen==23333&&abs(PVz)<15&&TMath::Abs(By)<2.4&&TMath::Abs(Bmumumass-3.096916)<0.15&&Bmass>5&&Bmass<6&& ((abs(Bmu1eta)<1.2 && Bmu1pt>3.5) || (abs(Bmu1eta)>1.2 && abs(Bmu1eta)<2.1 && Bmu1pt>(5.77-1.8*abs(Bmu1eta))) || (abs(Bmu1eta)>2.1 && abs(Bmu1eta)<2.4 && Bmu1pt>1.8)) && ((abs(Bmu2eta)<1.2 && Bmu2pt>3.5) || (abs(Bmu2eta)>1.2 && abs(Bmu2eta)<2.1 && Bmu2pt>(5.77-1.8*abs(Bmu2eta))) || (abs(Bmu2eta)>2.1 && abs(Bmu2eta)<2.4 && Bmu2pt>1.8)) && Bmu1TMOneStationTight && Bmu2TMOneStationTight && Bmu1InPixelLayer > 0 && (Bmu1InPixelLayer+Bmu1InStripLayer) > 5 && Bmu2InPixelLayer > 0 && (Bmu2InPixelLayer+Bmu2InStripLayer) > 5 && Bmu1dxyPV< 0.3 && Bmu2dxyPV< 0.3 && Bmu1dzPV<20 && Bmu2dzPV<20 && Bmu1isGlobalMuon && Bmu2isGlobalMuon && Btrk1highPurity && abs(Btrk1Eta)<2.4 && Btrk1Pt>0.5 && Bchi2cl>0.005 && ((Bpt>5 && Bpt<10 && Btrk1Pt>0.75 && Bchi2cl>0.032 && (BsvpvDistance/BsvpvDisErr)>0 && cos(Bdtheta)>0.99 && abs(Btrk1Eta)<2.4)|| (Bpt>10 && Bpt<20 && Btrk1Pt>0.88 && Bchi2cl>0.005 && (BsvpvDistance/BsvpvDisErr)>0 && cos(Bdtheta)>0.97 && abs(Btrk1Eta)<2.39) || (Bpt>20 && Bpt<30 && Btrk1Pt>0.84 && Bchi2cl>0.014 && (BsvpvDistance/BsvpvDisErr)>0 && cos(Bdtheta)>0.60 && abs(Btrk1Eta)<2.38) || (Bpt>30 && Bpt<80 && Btrk1Pt>1.06 && Bchi2cl>0.015 && (BsvpvDistance/BsvpvDisErr)>0 && cos(Bdtheta)>0.99 && abs(Btrk1Eta)<2.37))";
TString mycutb = "abs(PVz)<15&&TMath::Abs(By)<2.4&&TMath::Abs(Bmumumass-3.096916)<0.15&&Bmass>5&&Bmass<6&& ((abs(Bmu1eta)<1.2 && Bmu1pt>3.5) || (abs(Bmu1eta)>1.2 && abs(Bmu1eta)<2.1 && Bmu1pt>(5.77-1.8*abs(Bmu1eta))) || (abs(Bmu1eta)>2.1 && abs(Bmu1eta)<2.4 && Bmu1pt>1.8)) && ((abs(Bmu2eta)<1.2 && Bmu2pt>3.5) || (abs(Bmu2eta)>1.2 && abs(Bmu2eta)<2.1 && Bmu2pt>(5.77-1.8*abs(Bmu2eta))) || (abs(Bmu2eta)>2.1 && abs(Bmu2eta)<2.4 && Bmu2pt>1.8)) && Bmu1TMOneStationTight && Bmu2TMOneStationTight && Bmu1InPixelLayer > 0 && (Bmu1InPixelLayer+Bmu1InStripLayer) > 5 && Bmu2InPixelLayer > 0 && (Bmu2InPixelLayer+Bmu2InStripLayer) > 5 && Bmu1dxyPV< 0.3 && Bmu2dxyPV< 0.3 && Bmu1dzPV<20 && Bmu2dzPV<20 && Bmu1isGlobalMuon && Bmu2isGlobalMuon && Btrk1highPurity && abs(Btrk1Eta)<2.4 && Btrk1Pt>0.5 && Bchi2cl>0.005 && ((Bpt>5 && Bpt<10 && Btrk1Pt>0.75 && Bchi2cl>0.032 && (BsvpvDistance/BsvpvDisErr)>0 && cos(Bdtheta)>0.99 && abs(Btrk1Eta)<2.4)|| (Bpt>10 && Bpt<20 && Btrk1Pt>0.88 && Bchi2cl>0.005 && (BsvpvDistance/BsvpvDisErr)>0 && cos(Bdtheta)>0.97 && abs(Btrk1Eta)<2.39) || (Bpt>20 && Bpt<30 && Btrk1Pt>0.84 && Bchi2cl>0.014 && (BsvpvDistance/BsvpvDisErr)>0 && cos(Bdtheta)>0.60 && abs(Btrk1Eta)<2.38) || (Bpt>30 && Bpt<80 && Btrk1Pt>1.06 && Bchi2cl>0.015 && (BsvpvDistance/BsvpvDisErr)>0 && cos(Bdtheta)>0.99 && abs(Btrk1Eta)<2.37))";
TString mycutg = "TMath::Abs(Gy)<2.4&&TMath::Abs(GpdgId)==531&&GisSignal>0";*/
