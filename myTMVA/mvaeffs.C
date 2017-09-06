// root 6 version of mvaeffs
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;

//#include "tmvaglob.C"

#include "RQ_OBJECT.h"

#include "TH1.h"
#include "TROOT.h"
#include "TList.h"
#include "TIterator.h"
#include "TStyle.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TH2.h"
#include "TFormula.h"
#include "TFile.h"
#include "TApplication.h"
#include "TKey.h"
#include "TClass.h"
#include "TGaxis.h"

#include "TGWindow.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGNumberEntry.h"
#include "prefilter.h"


#include "TImage.h"
#include "TSystem.h"
// this macro plots the signal and background efficiencies
// as a function of the MVA cut.

namespace TMVAGlob {

   // --------- S t y l e ---------------------------
   static Bool_t UsePaperStyle = 0;
   // -----------------------------------------------

   enum TypeOfPlot { kId = 0,
                     kNorm,
                     kDecorrelated,
                     kPCA,
                     kGaussDecorr,
                     kNumOfMethods };

   static Int_t c_Canvas         = TColor::GetColor( "#f0f0f0" );
   static Int_t c_FrameFill      = TColor::GetColor( "#fffffd" );
   static Int_t c_TitleBox       = TColor::GetColor( "#5D6B7D" );
   static Int_t c_TitleBorder    = TColor::GetColor( "#7D8B9D" );
   static Int_t c_TitleText      = TColor::GetColor( "#FFFFFF" );
   static Int_t c_SignalLine     = TColor::GetColor( "#0000ee" );
   static Int_t c_SignalFill     = TColor::GetColor( "#7d99d1" );
   static Int_t c_BackgroundLine = TColor::GetColor( "#ff0000" );
   static Int_t c_BackgroundFill = TColor::GetColor( "#ff0000" );
   static Int_t c_NovelBlue      = TColor::GetColor( "#2244a5" );

   // set the style
   void SetSignalAndBackgroundStyle( TH1* sig, TH1* bkg, TH1* all = 0 ) 
   {
      //signal
      // const Int_t FillColor__S = 38 + 150; // change of Color Scheme in ROOT-5.16.
      // convince yourself with gROOT->GetListOfColors()->Print()
      Int_t FillColor__S = c_SignalFill;
      Int_t FillStyle__S = 1001;
      Int_t LineColor__S = c_SignalLine;
      Int_t LineWidth__S = 2;

      // background
      //Int_t icolor = UsePaperStyle ? 2 + 100 : 2;
      Int_t FillColor__B = c_BackgroundFill;
      Int_t FillStyle__B = 3554;
      Int_t LineColor__B = c_BackgroundLine;
      Int_t LineWidth__B = 2;

      if (sig != NULL) {
         sig->SetLineColor( LineColor__S );
         sig->SetLineWidth( LineWidth__S );
         sig->SetFillStyle( FillStyle__S );
         sig->SetFillColor( FillColor__S );
      }
 
      if (bkg != NULL) {
         bkg->SetLineColor( LineColor__B );
         bkg->SetLineWidth( LineWidth__B );
         bkg->SetFillStyle( FillStyle__B );
         bkg->SetFillColor( FillColor__B );
      }

      if (all != NULL) {
         all->SetLineColor( LineColor__S );
         all->SetLineWidth( LineWidth__S );
         all->SetFillStyle( FillStyle__S );
         all->SetFillColor( FillColor__S );
      }
   }

   void SetMultiClassStyle( TObjArray* hists ) 
   {
      //signal
      // const Int_t FillColor__S = 38 + 150; // change of Color Scheme in ROOT-5.16.
      // convince yourself with gROOT->GetListOfColors()->Print()
      //Int_t FillColor__S = c_SignalFill;
      //Int_t FillStyle__S = 1001;
      //Int_t LineColor__S = c_SignalLine;
      //Int_t LineWidth__S = 2;

      // background
      //Int_t icolor = UsePaperStyle ? 2 + 100 : 2;
      //Int_t FillColor__B = c_BackgroundFill;
      //Int_t FillStyle__B = 3554;
      //Int_t LineColor__B = c_BackgroundLine;
      //Int_t LineWidth__B = 2;

      Int_t FillColors[10] = {38,2,3,6,7,8,9,11};
      Int_t LineColors[10] = {4,2,3,6,7,8,9,11};
      Int_t FillStyles[5] = {1001,3554,3003,3545,0};

      for(Int_t i=0; i<hists->GetEntriesFast(); ++i){
         ((TH1*)(*hists)[i])->SetFillColor(FillColors[i%10]);
         ((TH1*)(*hists)[i])->SetFillStyle(FillStyles[i%5]);
         ((TH1*)(*hists)[i])->SetLineColor(LineColors[i%10]);
         ((TH1*)(*hists)[i])->SetLineWidth(2);
      }
   }

   // set frame styles
   void SetFrameStyle( TH1* frame, Float_t scale = 1.0 )
   {
      frame->SetLabelOffset( 0.012, "X" );// label offset on x axis
      frame->SetLabelOffset( 0.012, "Y" );// label offset on x axis
      frame->GetXaxis()->SetTitleOffset( 1.25 );
      frame->GetYaxis()->SetTitleOffset( 1.22 );
      frame->GetXaxis()->SetTitleSize( 0.045*scale );
      frame->GetYaxis()->SetTitleSize( 0.045*scale );
      Float_t labelSize = 0.04*scale;
      frame->GetXaxis()->SetLabelSize( labelSize );
      frame->GetYaxis()->SetLabelSize( labelSize );

      // global style settings
      gPad->SetTicks();
      gPad->SetLeftMargin  ( 0.108*scale );
      gPad->SetRightMargin ( 0.050*scale );
      gPad->SetBottomMargin( 0.120*scale  );
   }

   void SetTMVAStyle() {
      
      TStyle *TMVAStyle = gROOT->GetStyle("TMVA");
      if(TMVAStyle!=0) {
         gROOT->SetStyle("TMVA");
         return;
      }
			
      TMVAStyle = new TStyle(*gROOT->GetStyle("Plain")); // our style is based on Plain
      TMVAStyle->SetName("TMVA");
      TMVAStyle->SetTitle("TMVA style based on \"Plain\" with modifications defined in tmvaglob.C");
      gROOT->GetListOfStyles()->Add(TMVAStyle);
      gROOT->SetStyle("TMVA");
			
      TMVAStyle->SetLineStyleString( 5, "[52 12]" );
      TMVAStyle->SetLineStyleString( 6, "[22 12]" );
      TMVAStyle->SetLineStyleString( 7, "[22 10 7 10]" );

      // the pretty color palette of old
      TMVAStyle->SetPalette((UsePaperStyle ? 18 : 1),0);

      // use plain black on white colors
      TMVAStyle->SetFrameBorderMode(0);
      TMVAStyle->SetCanvasBorderMode(0);
      TMVAStyle->SetPadBorderMode(0);
      TMVAStyle->SetPadColor(0);
      TMVAStyle->SetFillStyle(0);

      TMVAStyle->SetLegendBorderSize(0);

      // title properties
      // TMVAStyle->SetTitleW(.4);
      // TMVAStyle->SetTitleH(.10);
      // MVAStyle->SetTitleX(.5);
      // TMVAStyle->SetTitleY(.9);
      TMVAStyle->SetTitleFillColor( c_TitleBox );
      TMVAStyle->SetTitleTextColor( c_TitleText );
      TMVAStyle->SetTitleBorderSize( 1 );
      TMVAStyle->SetLineColor( c_TitleBorder );
      if (!UsePaperStyle) {
         TMVAStyle->SetFrameFillColor( c_FrameFill );
         TMVAStyle->SetCanvasColor( c_Canvas );
      }

      // set the paper & margin sizes
      TMVAStyle->SetPaperSize(20,26);
      TMVAStyle->SetPadTopMargin(0.10);
      TMVAStyle->SetPadRightMargin(0.05);
      TMVAStyle->SetPadBottomMargin(0.11);
      TMVAStyle->SetPadLeftMargin(0.12);

      // use bold lines and markers
      TMVAStyle->SetMarkerStyle(21);
      TMVAStyle->SetMarkerSize(0.3);
      TMVAStyle->SetHistLineWidth(2);
      TMVAStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

      // do not display any of the standard histogram decorations
      TMVAStyle->SetOptTitle(1);
      TMVAStyle->SetTitleH(0.052);

      TMVAStyle->SetOptStat(0);
      TMVAStyle->SetOptFit(0);

      // put tick marks on top and RHS of plots
      TMVAStyle->SetPadTickX(1);
      TMVAStyle->SetPadTickY(1);

   }

   void DestroyCanvases()
   {

      TList* loc = (TList*)gROOT->GetListOfCanvases();
      TListIter itc(loc);
      TObject *o(0);
      while ((o = itc())) delete o;
   }

   // set style and remove existing canvas'
   void Initialize( Bool_t useTMVAStyle = kTRUE )
   {
      // destroy canvas'
      DestroyCanvases();

      // set style
      if (!useTMVAStyle) {
         gROOT->SetStyle("Plain");
         gStyle->SetOptStat(0);
         return;
      }

      SetTMVAStyle();
   }

   // checks if file with name "fin" is already open, and if not opens one
   TFile* OpenFile( const TString& fin )
   {
      TFile* file = gDirectory->GetFile();
      if (file==0 || fin != file->GetName()) {
         if (file != 0) {
            gROOT->cd();
            file->Close();
         }
         cout << "--- Opening root file " << fin << " in read mode" << endl;
         file = TFile::Open( fin, "READ" );
      }
      else {
         file = gDirectory->GetFile();
      }

      file->cd();
      return file;
   }

   // used to create output file for canvas
   void imgconv( TCanvas* c, const TString & fname )
   {
      // return;
      if (NULL == c) {
         cout << "*** Error in TMVAGlob::imgconv: canvas is NULL" << endl;
      }
      else {
         // create directory if not existing
         TString f = fname;
         TString dir = f.Remove( f.Last( '/' ), f.Length() - f.Last( '/' ) );
         gSystem->mkdir( dir );

         TString pngName = fname + ".png";
         TString gifName = fname + ".gif";
         TString epsName = fname + ".eps";
         c->cd();

         // create eps (other option: c->Print( epsName ))
         if (UsePaperStyle) {
            c->Print(epsName);
         } 
         else {
            cout << "--- --------------------------------------------------------------------" << endl;
            cout << "--- If you want to save the image as eps, gif or png, please comment out " << endl;
            cout << "--- the corresponding lines (line no. 239-241) in tmvaglob.C" << endl;
            cout << "--- --------------------------------------------------------------------" << endl;
            c->Print(epsName);
            c->Print(pngName);
            // c->Print(gifName);
         }
      }
   }

   TImage * findImage(const char * imageName) 
   { 
      // looks for the image in macropath
      TString macroPath(gROOT->GetMacroPath()); // look for the image in here
      Ssiz_t curIndex(0);
      TImage *img(0);
      while(1) {
         Ssiz_t pathStart = curIndex;
         curIndex = macroPath.Index(":",curIndex);
         Ssiz_t pathEnd = (curIndex==-1)?macroPath.Length():curIndex;
         TString path(macroPath(pathStart,pathEnd-pathStart));
         
         gSystem->ExpandPathName(path);
         const char* fullName = Form("%s/%s", path.Data(), imageName);

         Bool_t fileFound = ! gSystem->AccessPathName(fullName);

         if(fileFound) {
            img = TImage::Open(fullName);
            break;
         }
         if(curIndex==-1) break;
         curIndex++;
      }
      return img;
   }

   void plot_logo( Float_t v_scale = 1.0, Float_t skew = 1.0 )
   {

      TImage *img = findImage("tmva_logo.gif");
      if (!img) {
         cout << "+++ Could not open image tmva_logo.gif" << endl;
         return;
      }
      
      img->SetConstRatio(kFALSE);
      UInt_t h_ = img->GetHeight();
      UInt_t w_ = img->GetWidth();

      Float_t r = w_/h_;
      gPad->Update();
      Float_t rpad = Double_t(gPad->VtoAbsPixel(0) - gPad->VtoAbsPixel(1))/(gPad->UtoAbsPixel(1) - gPad->UtoAbsPixel(0));
      r *= rpad;

      Float_t d = 0.055;
      // absolute coordinates
      Float_t x1R = 1 - gStyle->GetPadRightMargin(); 
      Float_t y1B = 1 - gStyle->GetPadTopMargin()+.01; // we like the logo to sit a bit above the histo 

      Float_t x1L = x1R - d*r/skew;
      Float_t y1T = y1B + d*v_scale*skew;
      if (y1T>0.99) y1T = 0.99;

      TPad *p1 = new TPad("imgpad", "imgpad", x1L, y1B, x1R, y1T );
      p1->SetRightMargin(0);
      p1->SetBottomMargin(0);
      p1->SetLeftMargin(0);
      p1->SetTopMargin(0);
      p1->Draw();

      Int_t xSizeInPixel = p1->UtoAbsPixel(1) - p1->UtoAbsPixel(0);
      Int_t ySizeInPixel = p1->VtoAbsPixel(0) - p1->VtoAbsPixel(1);
      if (xSizeInPixel<=25 || ySizeInPixel<=25) {
         delete p1;
         return; // ROOT doesn't draw smaller than this
      }

      p1->cd();
      img->Draw();
   } 

   void NormalizeHist( TH1* h ) 
   {
      if (h==0) return;
      if (h->GetSumw2N() == 0) h->Sumw2();
      if(h->GetSumOfWeights()!=0) {
         Float_t dx = (h->GetXaxis()->GetXmax() - h->GetXaxis()->GetXmin())/h->GetNbinsX();
         h->Scale( 1.0/h->GetSumOfWeights()/dx );
      }
   }
   void NormalizeHists( TH1* sig, TH1* bkg = 0 ) 
   {
      if (sig->GetSumw2N() == 0) sig->Sumw2();
      if (bkg && bkg->GetSumw2N() == 0) bkg->Sumw2();
      
      if(sig->GetSumOfWeights()!=0) {
         Float_t dx = (sig->GetXaxis()->GetXmax() - sig->GetXaxis()->GetXmin())/sig->GetNbinsX();
         sig->Scale( 1.0/sig->GetSumOfWeights()/dx );
      }
      if (bkg != 0 && bkg->GetSumOfWeights()!=0) {
         Float_t dx = (bkg->GetXaxis()->GetXmax() - bkg->GetXaxis()->GetXmin())/bkg->GetNbinsX();
         bkg->Scale( 1.0/bkg->GetSumOfWeights()/dx );
      }
   }

   // the following are tools to help handling different methods and titles


   void GetMethodName( TString & name, TKey * mkey ) {
      if (mkey==0) return;
      name = mkey->GetName();
      name.ReplaceAll("Method_","");
   }

   void GetMethodTitle( TString & name, TKey * ikey ) {
      if (ikey==0) return;
      name = ikey->GetName();
   }

   void GetMethodName( TString & name, TDirectory * mdir ) {
      if (mdir==0) return;
      name = mdir->GetName();
      name.ReplaceAll("Method_","");
   }

   void GetMethodTitle( TString & name, TDirectory * idir ) {
      if (idir==0) return;
      name = idir->GetName();
   }

   TKey *NextKey( TIter & keyIter, TString className) {
      TKey *key=(TKey *)keyIter.Next();
      TKey *rkey=0;
      Bool_t loop=(key!=0);
      //
      while (loop) {
         TClass *cl = gROOT->GetClass(key->GetClassName());
         if (cl->InheritsFrom(className.Data())) {
            loop = kFALSE;
            rkey = key;
         } else {
            key = (TKey *)keyIter.Next();
            if (key==0) loop = kFALSE;
         }
      }
      return rkey;
   }

   UInt_t GetListOfKeys( TList& keys, TString inherits, TDirectory *dir=0 )
   {
      // get a list of keys with a given inheritance
      // the list contains TKey objects
      if (dir==0) dir = gDirectory;
      TIter mnext(dir->GetListOfKeys());
      TKey *mkey;
      keys.Clear();
      keys.SetOwner(kFALSE);
      UInt_t ni=0;
      while ((mkey = (TKey*)mnext())) {
         // make sure, that we only look at TDirectory with name Method_<xxx>
         TClass *cl = gROOT->GetClass(mkey->GetClassName());
         if (cl->InheritsFrom(inherits)) {
            keys.Add(mkey);
            ni++;
         }
      }
      return ni;
   }

   Int_t GetNumberOfTargets( TDirectory *dir )
   {
      if (!dir) {
         cout << "tmvaglob::GetNumberOfTargets is called with *dir==NULL :( " << endl;
         return 0;
      }
      TIter next(dir->GetListOfKeys());
      TKey* key    = 0;
      Int_t noTrgts = 0;
      
      while ((key = (TKey*)next())) {
         if (key->GetCycle() != 1) continue;        
         if (TString(key->GetName()).Contains("__Regression_target")) noTrgts++;
      }
      return noTrgts;
   }

   Int_t GetNumberOfInputVariables( TDirectory *dir )
   {
      TIter next(dir->GetListOfKeys());
      TKey* key    = 0;
      Int_t noVars = 0;
         
      while ((key = (TKey*)next())) {
         if (key->GetCycle() != 1) continue;
         
         // count number of variables (signal is sufficient), exclude target(s)
         if (TString(key->GetName()).Contains("__Signal") || (TString(key->GetName()).Contains("__Regression") && !(TString(key->GetName()).Contains("__Regression_target")))) noVars++;
      }
      
      return noVars;
   }

   std::vector<TString> GetInputVariableNames(TDirectory *dir )
   {
      TIter next(dir->GetListOfKeys());
      TKey* key = 0;
      //set<std::string> varnames;
      std::vector<TString> names;
      
      while ((key = (TKey*)next())) {
         if (key->GetCycle() != 1) continue;
         TClass *cl = gROOT->GetClass(key->GetClassName());
         if (!cl->InheritsFrom("TH1")) continue;
         TString name(key->GetName());
         Int_t pos = name.First("__");
         name.Remove(pos);
         Bool_t hasname = false;
         std::vector<TString>::const_iterator iter = names.begin();
         while(iter != names.end()){
            if(name.CompareTo(*iter)==0)
               hasname=true;
            iter++;
         }
         if(!hasname)
            names.push_back(name);
      }
      return names;
   }

   Int_t GetNumberOfInputVariablesMultiClass( TDirectory *dir ){
      std::vector<TString> names(GetInputVariableNames(dir));
      return names.end() - names.begin();
   }
   
   std::vector<TString> GetClassNames(TDirectory *dir )
   {      
      
      TIter next(dir->GetListOfKeys());
      TKey* key = 0;
      //set<std::string> varnames;
      std::vector<TString> names;
      
      while ((key = (TKey*)next())) {
         if (key->GetCycle() != 1) continue;
         TClass *cl = gROOT->GetClass(key->GetClassName());
         if (!cl->InheritsFrom("TH1")) continue;
         TString name(key->GetName());
         name.ReplaceAll("_Deco","");
         name.ReplaceAll("_Gauss","");
         name.ReplaceAll("_PCA","");
         name.ReplaceAll("_Id","");
         name.ReplaceAll("_vs_","");
         char c = '_';
         Int_t pos = name.Last(c);
         name.Remove(0,pos+1);
         
         /*Int_t pos = name.First("__");
         name.Remove(0,pos+2);
         char c = '_';
         pos = name.Last(c);
         name.Remove(pos);
         if(name.Contains("Gauss")){
            pos = name.Last(c);
            name.Remove(pos);
         }
         pos = name.Last(c);
         if(pos!=-1)
            name.Remove(0,pos+1);
         */
         Bool_t hasname = false;
         std::vector<TString>::const_iterator iter = names.begin();
         while(iter != names.end()){
            if(name.CompareTo(*iter)==0)
               hasname=true;
            iter++;
         }
         if(!hasname)
            names.push_back(name);
      }
      return names;
   }


   TKey* FindMethod( TString name, TDirectory *dir=0 )
   {
      // find the key for a method
      if (dir==0) dir = gDirectory;
      TIter mnext(dir->GetListOfKeys());
      TKey *mkey;
      TKey *retkey=0;
      Bool_t loop=kTRUE;
      while (loop) {
         mkey = (TKey*)mnext();
         if (mkey==0) {
            loop = kFALSE;
         } 
         else {
            TString clname = mkey->GetClassName();
            TClass *cl = gROOT->GetClass(clname);
            if (cl->InheritsFrom("TDirectory")) {
               TString mname = mkey->GetName(); // method name
               TString tname = "Method_"+name;  // target name
               if (mname==tname) { // target found!
                  loop = kFALSE;
                  retkey = mkey;
               }
            }
         }
      }
      return retkey;
   }

   Bool_t ExistMethodName( TString name, TDirectory *dir=0 )
   {
      // find the key for a method
      if (dir==0) dir = gDirectory;
      TIter mnext(dir->GetListOfKeys());
      TKey *mkey;
      Bool_t loop=kTRUE;
      while (loop) {
         mkey = (TKey*)mnext();
         if (mkey==0) {
            loop = kFALSE;
         } 
         else {
            TString clname  = mkey->GetClassName();
            TString keyname = mkey->GetName();
            TClass *cl = gROOT->GetClass(clname);
            if (keyname.Contains("Method") && cl->InheritsFrom("TDirectory")) {

               TDirectory* d_ = (TDirectory*)dir->Get( keyname );
               if (!d_) {
                  cout << "HUUUGE TROUBLES IN TMVAGlob::ExistMethodName() --> contact authors" << endl;
                  return kFALSE;
               }

               TIter mnext_(d_->GetListOfKeys());
               TKey *mkey_;
               while ((mkey_ = (TKey*)mnext_())) {
                  TString clname_ = mkey_->GetClassName();
                  TClass *cl_ = gROOT->GetClass(clname_);
                  if (cl_->InheritsFrom("TDirectory")) {
                     TString mname = mkey_->GetName(); // method name
                     if (mname==name) { // target found!                  
                        return kTRUE;
                     }
                  }
               }
            }
         }
      }
      return kFALSE;
   }

   UInt_t GetListOfMethods( TList & methods, TDirectory *dir=0 )
   {
      // get a list of methods
      // the list contains TKey objects
      if (dir==0) dir = gDirectory;
      TIter mnext(dir->GetListOfKeys());
      TKey *mkey;
      methods.Clear();
      methods.SetOwner(kFALSE);
      UInt_t ni=0;
      while ((mkey = (TKey*)mnext())) {
         // make sure, that we only look at TDirectory with name Method_<xxx>
         TString name = mkey->GetClassName();
         TClass *cl = gROOT->GetClass(name);
         if (cl->InheritsFrom("TDirectory")) {
            if (TString(mkey->GetName()).BeginsWith("Method_")) {
               methods.Add(mkey);
               ni++;
            }
         }
      }
      cout << "--- Found " << ni << " classifier types" << endl;
      return ni;
   }

   UInt_t GetListOfJobs( TFile* file, TList& jobdirs)
   {
      // get a list of all jobs in all method directories
      // based on ideas by Peter and Joerg found in macro deviations.C
      TIter next(file->GetListOfKeys());
      TKey *key(0);   
      while ((key = (TKey*)next())) {
         
         if (TString(key->GetName()).BeginsWith("Method_")) {
            if (gROOT->GetClass(key->GetClassName())->InheritsFrom("TDirectory")) {

               TDirectory* mDir = (TDirectory*)key->ReadObj();
               
               TIter keyIt(mDir->GetListOfKeys());
               TKey *jobkey;
               while ((jobkey = (TKey*)keyIt())) {
                  if (!gROOT->GetClass(jobkey->GetClassName())->InheritsFrom("TDirectory")) continue;
                  
                  TDirectory *jobDir = (TDirectory *)jobkey->ReadObj();
                  cout << "jobdir name  " << jobDir->GetName() << endl;
                  jobdirs.Add(jobDir);
               }
            }
         }
      }
      return jobdirs.GetSize();
   }

   UInt_t GetListOfTitles( TDirectory *rfdir, TList & titles )
   {
      // get a list of titles (i.e TDirectory) given a method dir
      UInt_t ni=0;
      if (rfdir==0) return 0;
      TList *keys = rfdir->GetListOfKeys();
      if (keys==0) {
         cout << "+++ Directory '" << rfdir->GetName() << "' contains no keys" << endl;
         return 0;
      }
      //
      TIter rfnext(rfdir->GetListOfKeys());
      TKey *rfkey;
      titles.Clear();
      titles.SetOwner(kFALSE);
      while ((rfkey = (TKey*)rfnext())) {
         // make sure, that we only look at histograms
         TClass *cl = gROOT->GetClass(rfkey->GetClassName());
         if (cl->InheritsFrom("TDirectory")) {
            titles.Add(rfkey);
            ni++;
         }
      }
      cout << "--- Found " << ni << " instance(s) of the method " << rfdir->GetName() << endl;
      return ni;
   }

   UInt_t GetListOfTitles( TString & methodName, TList & titles, TDirectory *dir=0 )
   {
      // get the list of all titles for a given method
      // if the input dir is 0, gDirectory is used
      // returns a list of keys
      UInt_t ni=0;
      if (dir==0) dir = gDirectory;
      TDirectory* rfdir = (TDirectory*)dir->Get( methodName );
      if (rfdir==0) {
         cout << "+++ Could not locate directory '" << methodName << endl;
         return 0;
      }

      return GetListOfTitles( rfdir, titles );

      TList *keys = rfdir->GetListOfKeys();
      if (keys==0) {
         cout << "+++ Directory '" << methodName << "' contains no keys" << endl;
         return 0;
      }
      //
      TIter rfnext(rfdir->GetListOfKeys());
      TKey *rfkey;
      titles.Clear();
      titles.SetOwner(kFALSE);
      while ((rfkey = (TKey*)rfnext())) {
         // make sure, that we only look at histograms
         TClass *cl = gROOT->GetClass(rfkey->GetClassName());
         if (cl->InheritsFrom("TDirectory")) {
            titles.Add(rfkey);
            ni++;
         }
      }
      cout << "--- Found " << ni << " instance(s) of the method " << methodName << endl;
      return ni;
   }

   TDirectory *GetInputVariablesDir( TMVAGlob::TypeOfPlot type, TDirectory *dir=0 )
   {
      // get the InputVariables directory
      const TString directories[TMVAGlob::kNumOfMethods] = { "InputVariables_Id",
                                                             "InputVariables_Deco",
                                                             "InputVariables_PCA",
                                                             "InputVariables_Gauss_Deco" };
      if (dir==0) dir = gDirectory;

      // get top dir containing all hists of the variables
      dir = (TDirectory*)gDirectory->Get( directories[type] );
      if (dir==0) {
         cout << "+++ Could not locate input variable directory '" << directories[type] << endl;
         return 0;
      }
      return dir;
   }

   TDirectory *GetCorrelationPlotsDir( TMVAGlob::TypeOfPlot type, TDirectory *dir=0 )
   {
      // get the CorrelationPlots directory
      if (dir==0) dir = GetInputVariablesDir( type, 0 );
      if (dir==0) return 0;
      //
      TDirectory* corrdir = (TDirectory*)dir->Get( "CorrelationPlots" );
      if (corrdir==0) {
         cout << "+++ Could not find CorrelationPlots directory 'CorrelationPlots'" << endl;
         return 0;
      }
      return corrdir;
   }

}
enum PlotType { EffPurity = 0 };

class MethodInfo : public TNamed {
public:
   MethodInfo() :
      methodName(""),
      methodTitle(""),
      sig(0),
      bgd(0),
      origSigE(0),
      origBgdE(0),
      sigE(0),
      bgdE(0),
      purS(0),
      sSig(0),
      effpurS(0),
      canvas(0),
      line1(0),
      line2(0),
      rightAxis(0),
      maxSignificance(0),
      maxSignificanceErr(0)
   {}
   virtual ~MethodInfo();

   TString  methodName;
   TString  methodTitle;
   TH1*     sig;
   TH1*     bgd;
   TH1*     origSigE;
   TH1*     origBgdE;
   TH1*     sigE;
   TH1*     bgdE;
   TH1*     purS;
   TH1*     sSig;    
   TH1*     effpurS;
   TCanvas* canvas;
   TLatex*  line1;
   TLatex*  line2;
   TGaxis*  rightAxis;
   Double_t maxSignificance;
   Double_t maxSignificanceErr;

   void SetResultHists() 
   {
      TString pname    = "purS_"         + methodTitle;
      TString epname   = "effpurS_"      + methodTitle;
      TString ssigname = "significance_" + methodTitle;

      sigE = (TH1*)origSigE->Clone("sigEffi");
      bgdE = (TH1*)origBgdE->Clone("bgdEffi");
      
      Int_t nbins = sigE->GetNbinsX();
      Double_t low = sigE->GetBinLowEdge(1);
      Double_t high = sigE->GetBinLowEdge(nbins+1);
      purS    = new TH1F(pname, pname, nbins, low, high);
      sSig    = new TH1F(ssigname, ssigname, nbins, low, high);
      effpurS = new TH1F(epname, epname, nbins, low, high);        

      // chop off useless stuff
      sigE->SetTitle( Form("Cut efficiencies for %s classifier", methodTitle.Data()) );
         
      // set the histogram style
      TMVAGlob::SetSignalAndBackgroundStyle( sigE, bgdE );
      TMVAGlob::SetSignalAndBackgroundStyle( purS, bgdE );
      TMVAGlob::SetSignalAndBackgroundStyle( effpurS, bgdE );
/*      TMVAGlob::SetSignalAndBackgroundStyle( sigE, bgdE );
      TMVAGlob::SetSignalAndBackgroundStyle( purS, bgdE );
      TMVAGlob::SetSignalAndBackgroundStyle( effpurS, bgdE );
  */    sigE->SetFillStyle( 0 );
      bgdE->SetFillStyle( 0 );
      sSig->SetFillStyle( 0 );
      sigE->SetLineWidth( 3 );
      bgdE->SetLineWidth( 3 );
      sSig->SetLineWidth( 3 );

      // the purity and quality
      purS->SetFillStyle( 0 );
      purS->SetLineWidth( 2 );
      purS->SetLineStyle( 5 );
      effpurS->SetFillStyle( 0 );
      effpurS->SetLineWidth( 2 );
      effpurS->SetLineStyle( 6 );
   }

   ClassDef(MethodInfo,0)
};

MethodInfo::~MethodInfo() 
{
   delete sigE;
   delete bgdE;
   delete purS;
   delete sSig;
   delete effpurS;
   if(gROOT->GetListOfCanvases()->FindObject(canvas))
      delete canvas;
}

class StatDialogMVAEffs {  

   RQ_OBJECT("StatDialogMVAEffs")
      
   public:

   StatDialogMVAEffs(const TGWindow* p, Float_t ns, Float_t nb);
   virtual ~StatDialogMVAEffs();
   
   void SetFormula(const TString& f) { fFormula = f; }
   TString GetFormula();
   TString GetFormulaString() { return fFormula; }
   TString GetLatexFormula();
   
   void ReadHistograms(TFile* file);
   void UpdateSignificanceHists();
   void DrawHistograms();

   void RaiseDialog() { if (fMain) { fMain->RaiseWindow(); fMain->Layout(); fMain->MapWindow(); } }

private:

   TGMainFrame *fMain;
   Float_t fNSignal;
   Float_t fNBackground;  
   TString fFormula;
   TList * fInfoList;

   TGNumberEntry* fSigInput;
   TGNumberEntry* fBkgInput;

   TGHorizontalFrame* fButtons;
   TGTextButton* fDrawButton;
   TGTextButton* fCloseButton;

   Int_t maxLenTitle;

   void UpdateCanvases();

public:

   // slots
   void SetNSignal(); //*SIGNAL*
   void SetNBackground(); //*SIGNAL*
   void Redraw(); //*SIGNAL*
   void Close(); //*SIGNAL*

   // result printing
   void PrintResults( const MethodInfo* info );
};

void StatDialogMVAEffs::SetNSignal() 
{
   fNSignal = fSigInput->GetNumber();
}

void StatDialogMVAEffs::SetNBackground() 
{
   fNBackground = fBkgInput->GetNumber();
}

TString StatDialogMVAEffs::GetFormula() 
{
   TString f = fFormula;
   f.ReplaceAll("S","x");
   f.ReplaceAll("B","y");
   return f;
}


TString StatDialogMVAEffs::GetLatexFormula() 
{
   TString f = fFormula;
   f.ReplaceAll("(","{");
   f.ReplaceAll(")","}");
   f.ReplaceAll("sqrt","#sqrt");
   return f;
}

void StatDialogMVAEffs::Redraw() 
{
   UpdateSignificanceHists();
   UpdateCanvases();
}

void StatDialogMVAEffs::Close() 
{
   delete this;
}

StatDialogMVAEffs::~StatDialogMVAEffs() 
{
   if (fInfoList) { 
      TIter next(fInfoList);
      MethodInfo *info(0);
      while ( (info = (MethodInfo*)next()) ) {
         delete info;
      }
      delete fInfoList;
      fInfoList=0;
   }


   fSigInput->Disconnect();
   fBkgInput->Disconnect();
   fDrawButton->Disconnect();
   fCloseButton->Disconnect();

   fMain->CloseWindow();
   fMain->Cleanup();
   fMain = 0;
}

StatDialogMVAEffs::StatDialogMVAEffs(const TGWindow* p, Float_t ns, Float_t nb) :
   fNSignal(ns),
   fNBackground(nb),
   fFormula(""),
   fInfoList(0),
   fSigInput(0),
   fBkgInput(0),
   fButtons(0),
   fDrawButton(0),
   fCloseButton(0),
   maxLenTitle(0)
{
   UInt_t totalWidth  = 500;
   UInt_t totalHeight = 300;
   // main frame
   fMain = new TGMainFrame(p, totalWidth, totalHeight, kMainFrame | kVerticalFrame);

   TGLabel *sigLab = new TGLabel(fMain,"Signal events");
   fMain->AddFrame(sigLab, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));

   fSigInput = new TGNumberEntry(fMain, (Double_t) fNSignal,5,-1,(TGNumberFormat::EStyle) 5);
   fSigInput->SetLimits(TGNumberFormat::kNELLimitMin,0,1);
   fMain->AddFrame(fSigInput, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fSigInput->Resize(100,24);

   TGLabel *bkgLab = new TGLabel(fMain, "Background events");
   fMain->AddFrame(bkgLab, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));

   fBkgInput = new TGNumberEntry(fMain, (Double_t) fNBackground,5,-1,(TGNumberFormat::EStyle) 5);
   fBkgInput->SetLimits(TGNumberFormat::kNELLimitMin,0,1);
   fMain->AddFrame(fBkgInput, new TGLayoutHints(kLHintsLeft | kLHintsTop,5,5,5,5));
   fBkgInput->Resize(100,24);

   fButtons = new TGHorizontalFrame(fMain, totalWidth,30);

   fCloseButton = new TGTextButton(fButtons,"&Close");
   fButtons->AddFrame(fCloseButton, new TGLayoutHints(kLHintsLeft | kLHintsTop));

   fDrawButton = new TGTextButton(fButtons,"&Draw");
   fButtons->AddFrame(fDrawButton, new TGLayoutHints(kLHintsRight | kLHintsTop,15));
  
   fMain->AddFrame(fButtons,new TGLayoutHints(kLHintsLeft | kLHintsBottom,5,5,5,5));

   fMain->SetWindowName("Significance");
   fMain->SetWMPosition(0,0);
   fMain->MapSubwindows();
   fMain->Resize(fMain->GetDefaultSize());
   fMain->MapWindow();

   fSigInput->Connect("ValueSet(Long_t)","StatDialogMVAEffs",this, "SetNSignal()");
   fBkgInput->Connect("ValueSet(Long_t)","StatDialogMVAEffs",this, "SetNBackground()");

   fDrawButton->Connect("Clicked()","TGNumberEntry",fSigInput, "ValueSet(Long_t)");
   fDrawButton->Connect("Clicked()","TGNumberEntry",fBkgInput, "ValueSet(Long_t)");
   fDrawButton->Connect("Clicked()", "StatDialogMVAEffs", this, "Redraw()");   

   fCloseButton->Connect("Clicked()", "StatDialogMVAEffs", this, "Close()");
}

void StatDialogMVAEffs::UpdateCanvases() 
{
   if (fInfoList==0) return;
   if (fInfoList->First()==0) return;
   MethodInfo* info = (MethodInfo*)fInfoList->First();
   if ( info->canvas==0 ) {
      DrawHistograms();
      return;
   }
   TIter next(fInfoList);
   while ( (info = (MethodInfo*)next()) ) {
      info->canvas->Update();
      info->rightAxis->SetWmax(1.1*info->maxSignificance);
      info->canvas->Modified(kTRUE);
      info->canvas->Update();
      info->canvas->Paint();
   }
}

void StatDialogMVAEffs::UpdateSignificanceHists() 
{
   TFormula f("sigf",GetFormula());
   TIter next(fInfoList);
   MethodInfo* info(0);
   TString cname = "Classifier";
   if (cname.Length() >  maxLenTitle)  maxLenTitle = cname.Length();
   TString str = Form( "%*s   (  #signal, #backgr.)  Optimal-cut  %s      NSig      NBkg   EffSig   EffBkg", 
                       maxLenTitle, cname.Data(), GetFormulaString().Data() );
   cout << "--- " << setfill('=') << setw(str.Length()) << "" << setfill(' ') << endl;
   cout << "--- " << str << endl;
   cout << "--- " << setfill('-') << setw(str.Length()) << "" << setfill(' ') << endl;
   Double_t maxSig    = -1;
   Double_t maxSigErr = -1;
   while ((info = (MethodInfo*)next())) {
      for (Int_t i=1; i<=info->origSigE->GetNbinsX(); i++) {
         Float_t eS = info->origSigE->GetBinContent( i );
         Float_t S = eS * fNSignal;
         Float_t B = info->origBgdE->GetBinContent( i ) * fNBackground;
         info->purS->SetBinContent( i, (S+B==0)?0:S/(S+B) );
         
         Double_t sig = f.Eval(S,B);
         if (sig > maxSig) {
            maxSig    = sig;
            if (GetFormulaString() == "S/sqrt(B)") {
               maxSigErr = sig * sqrt( 1./S + 1./(2.*B));
            }
         }
         info->sSig->SetBinContent( i, sig );
         info->effpurS->SetBinContent( i, eS*info->purS->GetBinContent( i ) );
      }
      
      info->maxSignificance    = info->sSig->GetMaximum();
      info->maxSignificanceErr = (maxSigErr > 0) ? maxSigErr : 0;
      info->sSig->Scale(1/info->maxSignificance);

      // update the text in the lower left corner
      PrintResults( info );
   }
   cout << "--- " << setfill('-') << setw(str.Length()) << "" << setfill(' ') << endl << endl;
}

void StatDialogMVAEffs::ReadHistograms(TFile* file) 
{
   if (fInfoList) { 
      TIter next(fInfoList);
      MethodInfo *info(0);
      while ( (info = (MethodInfo*)next()) ) {
         delete info;
      }
      delete fInfoList;
      fInfoList=0;
   }
   fInfoList = new TList;

   // search for the right histograms in full list of keys
   TIter next(file->GetListOfKeys());
   TKey *key(0);   
   while( (key = (TKey*)next()) ) {

      if (!TString(key->GetName()).BeginsWith("Method_")) continue;
      if( ! gROOT->GetClass(key->GetClassName())->InheritsFrom("TDirectory") ) continue;

      cout << "--- Found directory: " << ((TDirectory*)key->ReadObj())->GetName() << endl;

      TDirectory* mDir = (TDirectory*)key->ReadObj();

      TIter keyIt(mDir->GetListOfKeys());
      TKey *titkey;
      while((titkey = (TKey*)keyIt())) {
         if( ! gROOT->GetClass(titkey->GetClassName())->InheritsFrom("TDirectory") ) continue;
        
         MethodInfo* info = new MethodInfo();
         TDirectory* titDir = (TDirectory *)titkey->ReadObj();

         TMVAGlob::GetMethodName(info->methodName,key);
         TMVAGlob::GetMethodTitle(info->methodTitle,titDir);        
         if (info->methodTitle.Length() > maxLenTitle) maxLenTitle = info->methodTitle.Length();
         TString hname = "MVA_" + info->methodTitle;
        
         cout << "--- Classifier: " << info->methodTitle << endl;
        
         info->sig = dynamic_cast<TH1*>(titDir->Get( hname + "_S" ));
         info->bgd = dynamic_cast<TH1*>(titDir->Get( hname + "_B" ));
         info->origSigE = dynamic_cast<TH1*>(titDir->Get( hname + "_effS" ));
         info->origBgdE = dynamic_cast<TH1*>(titDir->Get( hname + "_effB" ));      
         if (info->origSigE==0 || info->origBgdE==0) { delete info; continue; }

         info->SetResultHists();
         fInfoList->Add(info);
      }
   }
   return;
}

void StatDialogMVAEffs::DrawHistograms() 
{
   // counter variables
   Int_t countCanvas = 0;

   // define Canvas layout here!
   const Int_t width = 600;   // size of canvas
   Int_t signifColor = TColor::GetColor( "#00aa00" );

   TIter next(fInfoList);
   MethodInfo* info(0);
   while ( (info = (MethodInfo*)next()) ) {

      // create new canvas
      TCanvas *c = new TCanvas( Form("canvas%d", countCanvas+1), 
                                Form("Cut efficiencies for %s classifier",info->methodTitle.Data()), 
                                countCanvas*50+200, countCanvas*20, width, Int_t(width*0.78) ); 
      info->canvas = c;

      // draw grid
      c->SetGrid(1);
      c->SetTickx(0);
      c->SetTicky(0);

      TStyle *TMVAStyle = gROOT->GetStyle("Plain"); // our style is based on Plain
      TMVAStyle->SetLineStyleString( 5, "[32 22]" );
      TMVAStyle->SetLineStyleString( 6, "[12 22]" );
         
      c->SetTopMargin(.2);
      
      // and the signal purity and quality
      info->effpurS->SetTitle("Cut efficiencies and optimal cut value");
      if (info->methodTitle.Contains("Cuts")) {
         info->effpurS->GetXaxis()->SetTitle( "Signal Efficiency" );
      }
      else {
         info->effpurS->GetXaxis()->SetTitle( TString("Cut value applied on ") + info->methodTitle + " output" );
      }
      info->effpurS->GetYaxis()->SetTitle( "Efficiency (Purity)" );
      TMVAGlob::SetFrameStyle( info->effpurS );

      c->SetTicks(0,0);
      c->SetRightMargin ( 2.0 );

      info->effpurS->SetMaximum(1.1);
      info->effpurS->Draw("histl");

      info->purS->Draw("samehistl");      

      // overlay signal and background histograms
      info->sigE->Draw("samehistl");
      info->bgdE->Draw("samehistl");

      info->sSig->SetLineColor( signifColor );
      info->sSig->Draw("samehistl");

      // redraw axes
      info->effpurS->Draw( "sameaxis" );

      // Draw legend               
      TLegend *legend1= new TLegend( c->GetLeftMargin(), 1 - c->GetTopMargin(), 
                                     c->GetLeftMargin() + 0.4, 1 - c->GetTopMargin() + 0.12 );
      legend1->SetFillStyle( 1 );
      legend1->AddEntry(info->sigE,"Signal efficiency","L");
      legend1->AddEntry(info->bgdE,"Background efficiency","L");
      legend1->Draw("same");
      legend1->SetBorderSize(1);
      legend1->SetMargin( 0.3 );

      TLegend *legend2= new TLegend( c->GetLeftMargin() + 0.4, 1 - c->GetTopMargin(), 
                                     1 - c->GetRightMargin(), 1 - c->GetTopMargin() + 0.12 );
      legend2->SetFillStyle( 1 );
      legend2->AddEntry(info->purS,"Signal purity","L");
      legend2->AddEntry(info->effpurS,"Signal efficiency*purity","L");
      legend2->AddEntry(info->sSig,GetLatexFormula().Data(),"L");
      legend2->Draw("same");
      legend2->SetBorderSize(1);
      legend2->SetMargin( 0.3 );
         
      // line to indicate maximum efficiency
      TLine* effline = new TLine( info->sSig->GetXaxis()->GetXmin(), 1, info->sSig->GetXaxis()->GetXmax(), 1 );
      effline->SetLineWidth( 1 );
      effline->SetLineColor( 1 );
      effline->Draw();

      // print comments
      TLatex tl;
      tl.SetNDC();
      tl.SetTextSize( 0.033 );
      Int_t maxbin = info->sSig->GetMaximumBin();
      info->line1 = tl.DrawLatex( 0.15, 0.23, Form("For %1.0f signal and %1.0f background", fNSignal, fNBackground));
      tl.DrawLatex( 0.15, 0.19, "events the maximum "+GetLatexFormula()+" is");

      if (info->maxSignificanceErr > 0) {
         info->line2 = tl.DrawLatex( 0.15, 0.15, Form("%5.2f +- %4.2f when cutting at %5.2f", 
                                                      info->maxSignificance, 
                                                      info->maxSignificanceErr, 
                                                      info->sSig->GetXaxis()->GetBinCenter(maxbin)) );
      }
      else {
         info->line2 = tl.DrawLatex( 0.15, 0.15, Form("%4.2f when cutting at %5.2f", 
                                                      info->maxSignificance, 
                                                      info->sSig->GetXaxis()->GetBinCenter(maxbin)) );
      }

      // add comment for Method cuts
      if (info->methodTitle.Contains("Cuts")){
         tl.DrawLatex( 0.13, 0.77, "Method Cuts provides a bundle of cut selections, each tuned to a");
         tl.DrawLatex(0.13, 0.74, "different signal efficiency. Shown is the purity for each cut selection.");
      }
      // save canvas to file
      c->Update();

      // Draw second axes
      info->rightAxis = new TGaxis(c->GetUxmax(), c->GetUymin(),
                                   c->GetUxmax(), c->GetUymax(),0,1.1*info->maxSignificance,510,"+L");
      info->rightAxis->SetLineColor ( signifColor );
      info->rightAxis->SetLabelColor( signifColor );
      info->rightAxis->SetTitleColor( signifColor );

      info->rightAxis->SetTitleSize( info->sSig->GetXaxis()->GetTitleSize() );
      info->rightAxis->SetTitle( "Significance" );
      info->rightAxis->Draw();

      c->Update();

      // switches
//added colsyst, here ptMin and ptMax not working so we changed with ptmin ptmax cos they are defined in .h file to save the canvas.
      const Bool_t Save_Images = kTRUE;
        
        if (Save_Images) {
      TMVAGlob::imgconv( c, Form("plots/mvaeffs_%s_%s_%.0f_%.0f", info->methodTitle.Data(),colsyst.Data(),ptmin,ptmax )); 
      }
      countCanvas++;
   }
}

void StatDialogMVAEffs::PrintResults( const MethodInfo* info )
{
   Int_t maxbin = info->sSig->GetMaximumBin();
   if (info->line1 !=0 )
      info->line1->SetText( 0.15, 0.23, Form("For %1.0f signal and %1.0f background", fNSignal, fNBackground));
   
   if (info->line2 !=0 ) {
      if (info->maxSignificanceErr > 0) {
         info->line2->SetText( 0.15, 0.15, Form("%3.2g +- %3.2g when cutting at %3.2g", 
                                                info->maxSignificance, 
                                                info->maxSignificanceErr, 
                                                info->sSig->GetXaxis()->GetBinCenter(maxbin)) );
      }
      else {
         info->line2->SetText( 0.15, 0.15, Form("%3.4f when cutting at %3.4f", info->maxSignificance, 
                                                info->sSig->GetXaxis()->GetBinCenter(maxbin)) );
      }

   }

   if (info->maxSignificanceErr <= 0) {
      TString opt = Form( "%%%is:  (%%9.8g,%%9.8g)    %%9.4f   %%10.6g  %%8.7g  %%8.7g %%8.4g %%8.4g", 
                          maxLenTitle );
      cout << "--- " 
           << Form( opt.Data(),
                    info->methodTitle.Data(), fNSignal, fNBackground, 
                    info->sSig->GetXaxis()->GetBinCenter( maxbin ),
                    info->maxSignificance,
                    info->origSigE->GetBinContent( maxbin )*fNSignal,   
                    info->origBgdE->GetBinContent( maxbin )*fNBackground,
                    info->origSigE->GetBinContent( maxbin ),
                    info->origBgdE->GetBinContent( maxbin ) )
           << endl;
   }
   else {
      TString opt = Form( "%%%is:  (%%9.8g,%%9.8g)    %%9.4f   (%%8.3g  +-%%6.3g)  %%8.7g  %%8.7g %%8.4g %%8.4g", 
                          maxLenTitle );
      cout << "--- " 
           << Form( opt.Data(),
                    info->methodTitle.Data(), fNSignal, fNBackground, 
                    info->sSig->GetXaxis()->GetBinCenter( maxbin ),
                    info->maxSignificance,
                    info->maxSignificanceErr,
                    info->origSigE->GetBinContent( maxbin )*fNSignal,   
                    info->origBgdE->GetBinContent( maxbin )*fNBackground,
                    info->origSigE->GetBinContent( maxbin ),
                    info->origBgdE->GetBinContent( maxbin ) )
           << endl;
   }
}
//added colsyst ,MVA, ptMin and ptMax for reading root files ////cn
void mvaeffs( TString MVA,TString colsyst,float ptMin=5,float ptMax=10, 
              Bool_t useTMVAStyle = kTRUE, TString formula="S/sqrt(S+B)" )
{
TString fin = Form("ROOT/TMVA_%s_%s_%.0f_%.0f.root", MVA.Data() ,colsyst.Data(),ptMin,ptMax);
  std::cout << "starts mvaeffs\n";

   ptmin=ptMin;
   std::cout << "1\n";
   ptmax=ptMax;
   TMVAGlob::Initialize( useTMVAStyle );

   std::cout << "2\n";
   StatDialogMVAEffs* gGui = new StatDialogMVAEffs(gClient->GetRoot(), 1000, 1000);
   std::cout << "3\n";

   TFile* file = TMVAGlob::OpenFile( fin );
   std::cout << "4\n";
   gGui->ReadHistograms(file);
   std::cout << "5\n";
   gGui->SetFormula(formula);
   std::cout << "6\n";
   gGui->UpdateSignificanceHists();
   gGui->DrawHistograms();
   gGui->RaiseDialog();   
}
