bool isPbPb;
float ptmin;
float ptmax;
double luminosity;

TString inputSname;
TString inputBname;
TString mycut;
TString mycuts;
TString mycutb;
TString mycutg;
TString colsyst;
TString MVAtype;
TString npfile;

TString selgen = "TMath::Abs(Gy)<2.4&&abs(GpdgId)==531&&GisSignal>0";

TString inputSname_pp = "/afs/cern.ch/user/c/cdozen/public/forLuke/loop_pp_pthat5_bstojpsiphi_18072017.root";
TString inputBname_pp = "/data/wangj/Data2015/Bntuple/pp/ntB_EvtBase_20160420_BfinderData_pp_20160419_bPt0jpsiPt0tkPt0p5.root"; 

TString mycut_pp = "TMath::Abs(By)<2.4&&TMath::Abs(Bmumumass-3.096916)<0.15&&Bmass>5&&Bmass<6";//&&((abs(Bmu1eta)<1.2 && Bmu1pt>3.5) || (abs(Bmu1eta)>1.2 && abs(Bmu1eta)<2.1 && Bmu1pt>(5.77-1.8*abs(Bmu1eta))) || (abs(Bmu1eta)>2.1 && abs(Bmu1eta)<2.4 && Bmu1pt>1.8))&& ((abs(Bmu2eta)<1.2 && Bmu2pt>3.5) || (abs(Bmu2eta)>1.2 && abs(Bmu2eta)<2.1 && Bmu2pt>(5.77-1.8*abs(Bmu2eta))) || (abs(Bmu2eta)>2.1 && abs(Bmu2eta)<2.4 && Bmu2pt>1.8)) && Bmu1TMOneStationTight && Bmu2TMOneStationTight && Bmu1InPixelLayer>0 && (Bmu1InPixelLayer+Bmu1InStripLayer) > 5 && Bmu2InPixelLayer > 0 && (Bmu2InPixelLayer+Bmu2InStripLayer) > 5 && Bmu1dxyPV< 0.3 && Bmu2dxyPV< 0.3 && Bmu1dzPV<20 && Bmu2dzPV<20 && Bmu1isGlobalMuon && Bmu2isGlobalMuon && Btrk1highPurity&&Bmass>5&&Bmass<6&&Bmu1pt>1.5&&Bmu2pt>1.5&&Blxy>0.025&&abs(Btktkmass-1.019455)<1.55e-02";


////"Btrk1Pt>0.7&&Btrk2Pt>0.7&&Bchi2cl>1.32e-02&&(Bd0/Bd0Err)>2.&&cos(Bdtheta)>2.60e-01" //extracted these cuts cos for mva method we are using these variables to find opt. cuts 

TString mycuts_pp = Form("%s&&Bgen==23333",mycut_pp.Data());
TString mycutb_pp = Form("%s&&abs(Bmass-5.367)>0.2&&abs(Bmass-5.367)<0.3",mycut_pp.Data());
TString mycutg_pp = "abs(Gy)<2.4&&abs(GpdgId)==531&&GisSignal>0";

TString inputSname_PP ="/afs/cern.ch/user/c/cdozen/public/forLuke/loop_Bs0_PbPb_MC_25072017_pthat10.root ";
TString inputBname_PP ="/data/wangj/Data2015/Bntuple/PbPb/Bntuple_BfinderData_PbPb_20160406_bPt5jpsiPt0tkPt0p8_BpB0BsX_skimhlt.root";

TString mycut_PP = "TMath::Abs(By)<2.4&&TMath::Abs(Bmumumass-3.096916)<0.15&&Bmass>5&&Bmass<6";//&&((abs(Bmu1eta)<1.2 && Bmu1pt>3.5) || (abs(Bmu1eta)>1.2 && abs(Bmu1eta)<2.1 && Bmu1pt>(5.77-1.8*abs(Bmu1eta))) || (abs(Bmu1eta)>2.1 && abs(Bmu1eta)<2.4 && Bmu1pt>1.8))&& ((abs(Bmu2eta)<1.2 && Bmu2pt>3.5) || (abs(Bmu2eta)>1.2 && abs(Bmu2eta)<2.1 && Bmu2pt>(5.77-1.8*abs(Bmu2eta))) || (abs(Bmu2eta)>2.1 && abs(Bmu2eta)<2.4 && Bmu2pt>1.8)) && Bmu1TMOneStationTight && Bmu2TMOneStationTight && Bmu1InPixelLayer>0 && (Bmu1InPixelLayer+Bmu1InStripLayer) > 5 && Bmu2InPixelLayer > 0 && (Bmu2InPixelLayer+Bmu2InStripLayer) > 5 && Bmu1dxyPV< 0.3 && Bmu2dxyPV< 0.3 && Bmu1dzPV<20 && Bmu2dzPV<20 && Bmu1isGlobalMuon && Bmu2isGlobalMuon && Btrk1highPurity&&Bmass>5&&Bmass<6&&Bmu1pt>1.5&&Bmu2pt>1.5&&Blxy>0.025&&abs(Btktkmass-1.019455)<1.55e-02";
//    TString mycut_PP = "TMath::Abs(By)<2.4&&abs(Bmumumass-3.096916)<0.15&&Bmass>5.&&Bmass<6.&&((abs(Bmu1eta)<1.2&&Bmu1pt>3.5) || (abs(Bmu1eta)>1.2&&abs(Bmu1eta)<2.1&&Bmu1pt>(5.77-1.8*abs(Bmu1eta))) || (abs(Bmu1eta)>2.1&&abs(Bmu1eta)<2.4&&Bmu1pt>1.8))&&((abs(Bmu2eta)<1.2&&Bmu2pt>3.5) || (abs(Bmu2eta)>1.2&&abs(Bmu2eta)<2.1&&Bmu2pt>(5.77-1.8*abs(Bmu2eta))) || (abs(Bmu2eta)>2.1&&abs(Bmu2eta)<2.4&&Bmu2pt>1.8))&&Bmu1TMOneStationTight&&Bmu2TMOneStationTight&&Bmu1InPixelLayer>0&&(Bmu1InPixelLayer+Bmu1InStripLayer)>5&&Bmu2InPixelLayer>0&&(Bmu2InPixelLayer+Bmu2InStripLayer)>5&&Bmu1dxyPV<0.3&&Bmu2dxyPV<0.3&&Bmu1dzPV<20&&Bmu2dzPV<20&&Bmu1isGlobalMuon&&Bmu2isGlobalMuon&&Btrk1highPurity&&(Bd0/Bd0Err)>5.&&Bmu1pt>1.5&&Bmu2pt>1.5&&TMath::Abs(Btktkmass-1.019455)<0.05";
//    TString mycut_PP ="TMath::Abs(By)<2.4&&abs(Bmumumass-3.096916)<0.15&&Bmass>5.&&Bmass<6.&&(Bd0/Bd0Err)>5.&&Bmu1pt>1.5&&Bmu2pt>1.5&&TMath::Abs(Btktkmass-1.019455)<0.05";
TString mycuts_PP = Form("%s&&Bgen==23333",mycut_PP.Data()); 
TString mycutb_PP = Form("%s&&abs(Bmass-5.367)>0.2&&abs(Bmass-5.367)<0.3",mycut_PP.Data()); 
TString mycutg_PP = "abs(Gy)<2.4&&abs(GpdgId)==531&&GisSignal>0";

//double lum_PP=15.17;
double lum_PP=13.18;
double lum_pp=27.87;


