//const int nBins=3;
const int nBins=4;
double ptBins[nBins+1] = {5.,10.,15.,20.,60.};
//double ptBins[nBins+1] = {5.,10.,15.,60.};

double low = -0.3;
double high = 0.3;
int nBDT = 20;
double N = 20;
double ep = (high-low)/N;

const int BIN_NUM= 1197;
const int HMIN=1;
const int HMAX=300;

const double binsize=((double)(HMAX)-(double)(HMIN))/(double)(BIN_NUM);
//Double_t BRchain=6.09604e-5; //6.09604e-5
Double_t BRchain=3.1189e-5;//for Bs0



