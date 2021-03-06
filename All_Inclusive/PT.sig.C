#define Delphes_cxx
#include "Delphes.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include "Riostream.h"

void Delphes::Loop(Int_t index=0)
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   
   // Create Variables
   Float_t PT;
   Float_t Eta;
   //
   Float_t Lumin=100;
   Float_t sigma=50000;
   Float_t BR=0.01;
   Float_t Event_weight = Lumin*sigma*BR/50000;
   //
   //Define Histogram for MT solely
   TH1F* h_sig = new TH1F("h_sig", "p_{T}, signal and #gamma+jet background", 50, 0, 250);
   // 

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
     
      // Fill Histogram 
     PT = TMath::MaxElement(Photon_,Photon_PT);
     Eta = TMath::Abs(Photon_Eta[TMath::LocMax(Photon_,Photon_PT)]);
     
     if( Eta < 1.44) {
     	h_sig->Fill(PT);
     }
     // progress indicator
     if (((jentry+1)*100) % nentries == 0) {
     	 Float_t percent;
     	 percent = static_cast<Float_t>((jentry+1)*100)/nentries;
     	 printf("\r%.0f%% complete",percent);
     	 cout.flush();
     }
   }
   cout << endl;
   // Normalise histogram
   //auto norm = h_sig->Integral();
   //h_sig->Scale(1.0/norm);
   // Draw Histogram
   //h_sig->Draw("HIST");
   //h_sig->GetXaxis()->SetTitle("p_{T} / GeV");
   //h_sig->GetXaxis()->CenterTitle();
   //h_sig->GetYaxis()->SetTitle("normalised frequency");
   //h_sig->GetYaxis()->CenterTitle();
   // 
   TFile *hfile = new TFile("PT.root", "UPDATE");
   h_sig->Write();
   hfile->Close();
}

