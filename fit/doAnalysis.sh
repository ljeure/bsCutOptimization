#!/bin/bash

DOANALYSISPP_FIT=1
DOANALYSISPbPb_FIT=0

LUMIPP=27.87 # BS analysis
INPUTMCPP="../test/BDT_MC.root"
INPUTDATAPP="../test/BDT_Data.root"
TRGPP="1"
TRGPPMC="1"
CUTPP="BDT>-.0979"
SELGENPP= "TMath::Abs(Gy)<2.4&&abs(GpdgId)==531&&GisSignal>0"

SELGENPPACCPP="TMath::Abs(Gy)<2.4 && abs(GpdgId)==531 && (GisSignal==13 || GisSignal==14) && Gtk1pt>1.0 && TMath::Abs(Gtk1eta)<2.4"
RECOONLYPP=$CUTPP
ISMCPP=0
ISDOWEIGHTPP=0
LABELPP="pp"
OUTPUTFILEPP="ROOTfiles/hPtSpectrumBplusPP.root"
OUTPUTFILEMCSTUDYPP="ROOTfiles/MCstudiesPP.root"
NPFIT_PP="3.12764e1*Gaus(x,5.33166,3.64663e-2)*(x<5.33166)+(x>=5.33166)*3.12764e1*Gaus(x,5.33166,1.5204e-1)+2.11124e2*TMath::Erf(-(x-5.14397)/6.43194e-2) + 2.11124e2"

if [ $DOANALYSISPP_FIT -eq 1 ]; then
g++ fitB.C $(root-config --cflags --libs) -g -o fitB.exe
./fitB.exe 0 "$INPUTDATAPP"  "$INPUTMCPP"  "$TRGPP" "$CUTPP"   "$SELGENPP"   "$ISMCPP"   1   "$ISDOWEIGHTPP"   "$LABELPP"  "$OUTPUTFILEPP" "$NPFIT_PP" 0
rm fitB.exe
fi

LUMIPbPb=13.18   # from bs analysis
INPUTDATAPbPb="../Skim/Skim_ntD_EvtBase_20170323_DfinderData_PbPb_20170123_BtoD0Pi_Dpt10Dsvpv3Dalpha0p14Dchi20p05Dmass56Tkpt1Ressvpv3.root"
INPUTMCPbPb="../Skim/Skim_ntD_EvtBase_20170130_DfinderMC_PbPb_20170123_BtoD0Pi_dPt0tkPt2p5_Pythia8_nonprompt_Hydjet_MB_Dpt5tkPt2p5Decay3tktkResDecay3Skim_pthatweight.root"
#TRGPbPb="(HLT_HIDmesonHITrackingGlobal_Dpt20_v1 || HLT_HIDmesonHITrackingGlobal_Dpt40_v1 || HLT_HIDmesonHITrackingGlobal_Dpt60_v1)"
TRGPbPb="1"
TRGPbPbMC="1"
CUTPbPb="abs(PVz)<15 && pclusterCompatibilityFilter && pprimaryVertexFilter && phfCoincFilter3 && TMath::Abs(Dy)<2.4 && Dmass>5&&Dmass<6 && abs(Dtrk1Eta)<1.0 && Dtrk1Pt>0.5 && Dchi2cl>0.05 && DtktkRes_chi2cl>0.05 && Dalpha < 0.14 && (Dpt>10 && (DsvpvDistance/DsvpvDisErr)>3) && Dtrk1Pt > 2.5 && DRestrk1Pt > 2.5 && DRestrk2Pt > 2.5 && fabs(DtktkResmass-1.87) < 0.03 && DtktkRes_alpha < 0.1 && Dalpha < 0.14 && ((Dpt < 30 && Dalpha < 0.08 && DsvpvDistance/DsvpvDisErr > 5.) || (Dpt > 30))"
SELGENPbPb="TMath::Abs(Gy)<2.4 && abs(GpdgId)==521 && (GisSignal==13 || GisSignal==14)"
SELGENPbPbACCPbPb="TMath::Abs(Gy)<2.4 && abs(GpdgId)==521 && (GisSignal==13 || GisSignal==14) && Gtk1pt>2.5 && TMath::Abs(Gtk1eta)<2.4"
RECOONLYPbPb=$CUTPbPb
ISMCPbPb=0
ISDOWEIGHTPbPb=1
LABELPbPb="PbPb"
OUTPUTFILEPbPb="ROOTfiles/hPtSpectrumBplusPbPb.root"
OUTPUTFILEMCSTUDYPbPb="ROOTfiles/MCstudiesPbPb.root"
NPFIT_PbPb="3.39711e1*Gaus(x,5.35002e0,3.87952e-2)*(x<5.35002e0)+(x>=5.35002e0)*3.39711e1*Gaus(x,5.35002e0,1.14232e-1)+2.16376e2*TMath::Erf(-(x-5.14189)/8.66243e-2) + 2.16376e2"
CENTPbPbMIN=0
CENTPbPbMAX=100

if [ $DOANALYSISPbPb_FIT -eq 1 ]; then
g++ fitB.C $(root-config --cflags --libs) -g -o fitB.exe
./fitB.exe 1 "$INPUTDATAPbPb"  "$INPUTMCPbPb"  "$TRGPbPb" "$CUTPbPb"   "$SELGENPbPb"   "$ISMCPbPb"   1   "$ISDOWEIGHTPbPb"   "$LABELPbPb"  "$OUTPUTFILEPbPb" "$NPFIT_PbPb" 0 "$CENTPbPbMIN" "$CENTPbPbMAX"
rm fitB.exe
fi
