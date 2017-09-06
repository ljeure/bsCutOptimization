#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include "../readxml/Tools.h"
#include "../readxml/readxml.h"
#include "style.h"
using namespace std;
TLegend* myLegend(Double_t x1, Double_t y1, Double_t x2, Double_t y2);
void getCutGivenEffB(string mvatype="MLP", string colsys = "pp", float ptmin = 5, float ptmax = 7, int nvar = 6, float EffB = 0.1){

	printf("MYV type: %s\nStage: %d\n", mvatype.c_str(), nvar);	
	TFile* f;
	TH1F* h;
	TH1F* heffB;
	float EffS = -1;

	// if its not cut base algorithm, find the discriminant value
	std::size_t found = mvatype.find("Cuts");
	float cutval = -999;
  std::cout << "1\n";
	if (found==std::string::npos){
  std::cout << "2\n";

    //TODO instead of MLP have a general file name
		f = new TFile(Form("../myTMVA/ROOT/TMVA_MLP_%s_%.0f_%.0f.root",
          colsys.c_str(), ptmin, ptmax));
  std::cout << "3\n";
		h = (TH1F*)f->Get(Form("dataset/Method_%s/%s/MVA_%s_effBvsS",
          mvatype.c_str(), mvatype.c_str(), mvatype.c_str()));
  std::cout << "4\n";
  double asdf = h->GetNbinsX();

  std::cout << "5\n";
		for(int i = 1; i < h->GetNbinsX(); i++){
  std::cout << "for\n";
			if(EffB >= h->GetBinContent(i) && EffB <= h->GetBinContent(i+1)){
        std::cout << "if\n";
        EffS = (h->GetBinCenter(i+1)-h->GetBinCenter(i))
          /(h->GetBinContent(i+1)-h->GetBinContent(i))
          *(EffB-h->GetBinContent(i))+h->GetBinCenter(i);
				//printf("%f, %f\n", EffS, h->GetBinCenter(i));
			}
		}
    std::cout << "a\n";

		heffB = (TH1F*)f->Get(Form("dataset/Method_%s/%s/MVA_%s_effB", mvatype.c_str(), mvatype.c_str(), mvatype.c_str()));
		for(int i = 1; i < heffB->GetNbinsX(); i++){
			if( (EffB <= heffB->GetBinContent(i) && EffB >= heffB->GetBinContent(i+1)) ){
                cutval = (heffB->GetBinCenter(i+1)-heffB->GetBinCenter(i))/(heffB->GetBinContent(i+1)-heffB->GetBinContent(i))*(EffB-heffB->GetBinContent(i))+heffB->GetBinCenter(i);
				//printf("%f, %f\n", cutval, heffB->GetBinCenter(i));
			}
		}
    std::cout << "b\n";
		if(EffB < heffB->GetBinContent(heffB->GetNbinsX())) cutval = heffB->GetBinCenter(heffB->GetNbinsX());
	}


	// cut base methods, read the weight file directly
	else{
    std::cout << "else\n";
		TString ptstring = Form("(%.1f,%.1f)",ptmin,ptmax);
		TString fileName = Form("../myTMVA/dataset/weights/TMVA_%s_%s_%.0f_%.0f_varStage%d.weights.xml",mvatype.c_str(),colsys.c_str(),ptmin,ptmax,nvar);
		//TString fileName = Form("TMVA_CutsSA_pp_5_7_varStage1.weights.xml");
		const char* filename = fileName.Data();
		void *doc = TMVA::gTools().xmlengine().ParseFile(filename,TMVA::gTools().xmlenginebuffersize());
		void* rootnode = TMVA::gTools().xmlengine().DocGetRootElement(doc); // node "MethodSetup"
		TString fullMethodName("");
		TMVA::gTools().ReadAttr(rootnode, "Method", fullMethodName);
		void *opts = TMVA::gTools().GetChild(rootnode,"Options");
		void* opt = TMVA::gTools().GetChild(opts,"Option");

		TString varProp("");
		while (opt)
		{
			TString optname("");
			TMVA::gTools().ReadAttr(opt, "name", optname);
			if (optname=="VarProp") varProp = TMVA::gTools().GetContent(opt);
			opt = TMVA::gTools().GetNextChild(opt);
		}

		TObjArray *marginclass = varProp.Tokenize(" ");
		std::vector<TString> margins;//avoid objarrays
		for(int i=0;i<marginclass->GetEntries();i++)
		{
			margins.push_back(((TObjString *)(marginclass->At(i)))->String());
		}
		void* variables = TMVA::gTools().GetChild(rootnode,"Variables");
    std::cout << "c\n";
		unsigned int nVar=0;
		std::vector<TString> varnames;
		TMVA::gTools().ReadAttr(variables, "NVar", nVar);

		void* var = TMVA::gTools().GetChild(variables,"Variable");
		for(unsigned int k=0;k<nVar;k++)
		{
			TString varname("");
			TMVA::gTools().ReadAttr(var, "Expression", varname);
			TString tem = Form("Variable%i",k);
			varval[k] = varname;
			var = TMVA::gTools().GetNextChild(var);
			varnames.push_back(varname);
		}

    std::cout << "d\n";
		void* weight = TMVA::gTools().GetChild(rootnode,"Weights");
		void* eff = TMVA::gTools().GetChild(weight,"Bin");
		int n=0;
		TString cut[NEff];
		while(eff)
		{
			TMVA::gTools().ReadAttr(eff, "effS", effS[n]);
			TMVA::gTools().ReadAttr(eff, "effB", effB[n]);
			void* cutsnode = TMVA::gTools().GetChild(eff,"Cuts");
			for(unsigned long l=0;l<varnames.size();l++)
			{
				double min,max;
				TMVA::gTools().ReadAttr(cutsnode, TString("cutMin_")+l, min);
				TMVA::gTools().ReadAttr(cutsnode, TString("cutMax_")+l, max);
    std::cout << "comments\n";
//				cutvalMax[l].push_back(max);
//				cutvalMin[l].push_back(min);
				TString lessmax = "<"; lessmax+=max;
				TString moremin = ">"; moremin+=min;
				cut[l] = "";
				if(margins[l]=="FMin" || margins[l]=="NotEnforced")
				{
					cut[n]+=" && "+varnames[l]+lessmax;
				}
				if(margins[l]=="FMax" || margins[l]=="NotEnforced")
				{
					cut[n]+=" && "+varnames[l]+moremin;
				}
			}
			eff = TMVA::gTools().GetNextChild(eff);
			n++;
		}
		TMVA::gTools().xmlengine().FreeDoc(doc);
    std::cout << "a\n";

		for(int i = 0 ; i < NEff-1; i ++){
			if(effB[i] <= EffB && effB[i+1] >= EffB){
				printf("effS: %f ~ %f\n", effS[i], effS[i+1]);
				printf("effB: %f ~ %f\n", effB[i], effB[i+1]);
				printf("cut: %s\n",cut[i].Data());
				EffS = (effS[i+1]-effS[i])/(effB[i+1]-effB[i])*(EffB-effB[i])+effS[i];
			}
		}
	}

	printf("EffS (interpolated): %f\n",EffS);
	printf("Cut value (non-linear method): %f\n", cutval);

}
