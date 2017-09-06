
Double_t gaussian(Double_t *x, Double_t *p) {
    return p[0]*TMath::Exp(-0.5*((x[0]-p[1])/p[2])* ((x[0]-p[1])/p[2]))
      + p[3]*x[0]+p[4];
}

Double_t polynomialTwo(Double_t *x, Double_t *p) {
    return p[0]*x[0]*x[0] + p[1]*x[0] + p[2];
}

int massFit() {

  // open file
  TFile *f = TFile::Open("/data/leure/bsTMVACDozen/merged-files/BDT_DATA.root");
  
  TTree* tree = (TTree*) f->Get("ntphi");
  tree->AddFriend("BDT_Tree");

  // plot data
  TH1D *h1 = new TH1D("h1",
      "Bmass;Bmass; ",
      100,5,6);
  TH1D * h2 = new TH1D("h2",
      "Bmass;Bmass; ",
      100,5,6);
  TCanvas *c11 = new TCanvas("c11","Bmass");
  tree->Draw("Bmass>>h1","BDT>-.0979");
  tree->Draw("Bmass>>h2","BDT>-.0979");

  h1->SetLineColor(kGreen);
  h1->SetLineWidth(4);
  h1->Draw();

  h2->SetLineColor(kBlue);
  h2->SetLineWidth(4);
  h2->Draw("same");

  // quadratic for background
  TF1 *func = new TF1("fit", polynomialTwo, 5,6,3);
  func->SetParameters(500,h2->GetMean(),h2->GetRMS());
  func->SetLineColor(kYellow);

  h2->Fit("fit");
  func->Draw("same");


  // gaussian fit for signal
  TF1 *func2 = new TF1("fit2", gaussian, 5,6,5);
  func2->SetParameters(500,h1->GetMean(),h1->GetRMS());

  h1->Fit("fit2");
  func2->Draw("same");

  for(int i=0; i<6; ++i)
    std::cout << "fit2 parameter " << i << " is " << func2->GetParameter(i)
      << std::endl;

  TLegend* legend = new TLegend(.5,.5,.4,.4);
  legend->SetHeader(" ");
  legend->AddEntry(h1, "signal");
  legend->AddEntry(h2, "background");
  legend->AddEntry(func2, "signal fit");
  legend->AddEntry(func, "background fit");
  legend->Draw();

  c11->Print("massplots.pdf)","Title:Bmass");
  return 0;
}
