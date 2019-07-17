// This script is to find the tracking efficiency for all the runs //
// date : 16 july , 2019 //

#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1F.h>
#include <fstream>
#include <iostream>
#include <TROOT.h>
#include <cstdlib>
#include <TDirectory.h>
#include "TGraph.h"

using namespace std;

void trk() {

  gStyle->SetOptStat(0);
  gStyle->SetOptStat(0);
  gStyle->SetTitleFontSize(0.05);
  gStyle->SetNdivisions(505);
  gStyle->SetCanvasColor(10);
  gStyle->SetPadTopMargin(0.10);
  gStyle->SetPadLeftMargin(0.08);
  gStyle->SetPadRightMargin(0.08);
  gStyle->SetPadBottomMargin(.14);
  gStyle->SetTitleYOffset(1.09);
  gStyle->SetTitleXOffset(0.855);
  gStyle->SetTitleYSize(0.03);
  gStyle->SetTitleXSize(0.03);
  gStyle->SetLabelFont(62,"X");
  gStyle->SetLabelFont(62,"Y");
  gStyle->SetTitleFont(62,"X");
  gStyle->SetTitleFont(62,"Y");
  gStyle->SetLabelSize(0.025,"X");
  gStyle->SetLabelSize(0.025,"Y");
  gStyle->SetPaperSize(23,24);
  gStyle->SetOptFit(1);

string spec;
cout << "spectrometer : shms / hms" << endl;
cin >> spec;


for(Int_t k = 2484; k<2485; k++){



  //TFile *f1 = new TFile(Form("../../abishek/hallc-replay-f2xem/ROOTfiles/realpass-0-shms/%s_replay_production_%d_-1.root",spec.c_str(),k),"READ");
  
TFile *f1 = new TFile(Form("ROOTfiles/%s_replay_production_all_%d_500000.root",spec.c_str(),k),"READ");
 
if((!f1) || (f1->IsZombie())) {
delete f1;  continue; 
}

TTree *T =(TTree*)f1->Get("T");

Int_t totev = T -> GetEntries();

Double_t cal_etotnorm;

T->SetBranchAddress("P.cal.etotnorm",&cal_etotnorm);

 TH1F *hcal_etotnorm = new TH1F("hcal_etotnorm","hcal_etotnorm",100., 0., 1.2);
 TH1F *hnpe_Sum = new TH1F("hnpe_Sum", "hnpe_Sum", "h")
for(Int_t k=0; k<totev; k++){
T->GetEntry(k);

hcal_etotnorm->Fill(cal_etotnorm);

} 
 TCanvas *c1 = new TCanvas("c1","scaler rate vs scaler time Hall C", 1200, 900);

c1->cd();
 hcal_etotnorm->Draw();
}


}
