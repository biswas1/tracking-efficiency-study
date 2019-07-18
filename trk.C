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
  gStyle->SetTitleFont(22,"");
  
  gStyle->SetTitleFontSize(0.05);
  gStyle->SetNdivisions(505);
  gStyle->SetCanvasColor(10);
  gStyle->SetPadTopMargin(0.10);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.08);
  gStyle->SetPadBottomMargin(.14);
  gStyle->SetTitleYOffset(1.4);
  gStyle->SetTitleXOffset(1.4);
  gStyle->SetTitleYSize(0.04);
  gStyle->SetTitleXSize(0.04);
  gStyle->SetLabelFont(22,"X");
  gStyle->SetLabelFont(22,"Y");
  gStyle->SetTitleFont(132,"X");
  gStyle->SetTitleFont(132,"Y");
  gStyle->SetLabelSize(0.04,"X");
  gStyle->SetLabelSize(0.04,"Y");
  gStyle->SetPaperSize(23,24);
  gStyle->SetOptFit(1);
  gStyle->SetStatX(0.50);
  gStyle->SetStatY(0.89);

string spec;
cout << "spectrometer : shms / hms" << endl;
cin >> spec;


gROOT->SetBatch(kTRUE); 

 std::vector<Double_t> trk_eff;
 std::vector<Double_t> Run_no; 
for(Int_t k = 2484; k<2486; k++){



  // TFile *f1 = new TFile(Form("../../../../abishek/hallc-replay-f2xem/ROOTfiles/realpass-0-shms/%s_replay_production_%d_-1.root",spec.c_str(),k),"READ");
  
  //TFile *f1 = new TFile(Form("ROOTfiles/%s_replay_production_all_%d_500000.root",spec.c_str(),k),"READ");

  TFile *f1 = new TFile(Form("/volatile/hallc/xem2/biswas/ROOTfiles/abi-update-12jul-2019/%s_replay_production_%d_50000.root",spec.c_str(),k),"READ");
  
 
if((!f1) || (f1->IsZombie())) {
delete f1;  continue; 
}

 Run_no.push_back(k);

TTree *T =(TTree*)f1->Get("T");

Int_t totev = T -> GetEntries();

 Double_t cal_etotnorm, cer_npeSum, dc_ntrack;

T->SetBranchAddress("P.cal.etotnorm",&cal_etotnorm);
T->SetBranchAddress("P.ngcer.npeSum",&cer_npeSum);
T->SetBranchAddress("P.dc.ntrack",&dc_ntrack);

 TH1F *hcal_etotnorm = new TH1F("hcal_etotnorm","hcal_etotnorm",100., 0., 2.0);
 TH1F *hcer_npeSum = new TH1F("hcer_npeSum","hcer_npeSum",100., -1.0, 80.0);
 TH1F *hdc_ntrack = new TH1F("dc_ntrack","dc_ntack",100., 0.0, 20.0);
 
 Int_t electron_should = 0; 
 Int_t track_did = 0;
 Double_t tracking_efficiency; 

 // TH1F *hnpe_Sum = new TH1F("hnpe_Sum", "hnpe_Sum", "h")
 for(Int_t k=0; k<totev; k++){
T->GetEntry(k);
 
 hcal_etotnorm->Fill(cal_etotnorm);
 hcer_npeSum->Fill(cer_npeSum);
 hdc_ntrack->Fill(dc_ntrack); 
 
if(cer_npeSum > 2.0 && cal_etotnorm > 0.7 ){
 electron_should++ ; 
 }

 if(cer_npeSum > 2.0 && cal_etotnorm > 0.7  && dc_ntrack > 0){
   track_did++;
 }
} 
 cout << electron_should << endl; 
 cout << track_did << endl;
 tracking_efficiency = (float(track_did)/float(electron_should));
 cout << tracking_efficiency << endl ; 
 
 trk_eff.push_back(tracking_efficiency) ;
 


 

 TCanvas *c1 = new TCanvas("c1","scaler rate vs scaler time Hall C", 1200, 900);

 c1->Divide(2,2);
 c1->cd(1);
 hcal_etotnorm->Draw();
 //Int_t binmax_cal =  hcal_etotnorm->GetMaximumBin();
 //Double_t x_cal =  hcal_etotnorm->GetXaxis()->GetBinCenter(binmax_cal);
 //Double_t maxy = binmax_cal ; 
 TLine *l1 = new TLine(0.6,0.0,0.6, 10000.0);
 l1->Draw("same");
 TLine *l2 = new TLine(1.6,0.0,1.6, 10000.0);
 l2->Draw("same");

 c1->cd(2);

 hcer_npeSum->Draw();

 TLine *l3 = new TLine(2.0,0.0,2.0, 10000.0);
 l3->Draw("same");
 
 c1->cd(3);
 hdc_ntrack->Draw();




TImage *img1 = TImage::Create();
img1->FromPad(c1);
img1->WriteImage("plots/trk_effi_vs_run_no.png");

}

 Int_t size = trk_eff.size();
 cout << size << endl ; 

 for(Int_t i = 0; i<size; i++){
   cout << Run_no[i] <<" trk eff :"  <<trk_eff[i] << endl; 
 }




 TCanvas *c2 = new TCanvas("c2","Tracking Efficiency vs Run No ", 1200, 900);
 TGraph *gr1 = new TGraph(size, &Run_no[0], &trk_eff[0]);
 c2->cd();
 gr1->SetMarkerStyle(26);
 gr1->SetMarkerSize(1.0);
 gr1->SetMarkerColor(kRed);
 gr1->SetTitle("Tracking Efficiency vs Run No : SHMS; Run No;Tracking Efficiency");
 gr1->GetXaxis()->CenterTitle();
 gr1->GetYaxis()->CenterTitle();
 gr1->Draw(" ape ");
 
 
 TImage *img2 = TImage::Create();
 img2->FromPad(c2);
 img2->WriteImage("trk_effi_vs_run_no.png");
}
