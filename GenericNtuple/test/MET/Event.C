#define  Event_cxx  
#include "Event.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TH1F.h>
#include <TH2F.h>
#include <iostream>
#include <TH1D.h>
#include <TH2D.h>

#include "Jet.h"
#include "Lepton.h"
#include "TtFullLepKinSolver.C"

  typedef std::vector<Jet> Jets;
  typedef Jets* JetsP;

  bool compByPtJet(Jet a, Jet b)
  {
    return a.Pt() > b.Pt();
  };
  bool compByCSVJet(Jet a, Jet b)
  {
    return a.CSV_ > b.CSV_;
  };

  typedef std::vector<Lepton> Leptons;
  typedef Leptons* LeptonsP;

  bool compByPtLep(Lepton a, Lepton b)
  {
     return a.Pt() > b.Pt();
  };
  typedef std::vector<int> ints;
  typedef std::vector<unsigned int> uints;
  typedef std::vector<double> doubles;
  typedef ints* intsP;
  typedef uints* uintsP;
  typedef doubles* doublesP;

void Event::Loop(char *Name,char* DecayMode)
{
//   if (fChain == 0) return;

    double tmassbegin_= 100;
    double tmassend_  = 300;
    double tmassstep_ = 1;

    double myints[] = {30.7137,56.2880,23.0744,59.1015,24.9145};
    std::vector<double> nupars_ (myints, myints + sizeof(myints) / sizeof(int) );
    TtFullLepKinSolver* solver = new TtFullLepKinSolver(tmassbegin_, tmassend_, tmassstep_, nupars_);

  TLorentzVector met, metMVA, metGen;//, muons[15], electrons[15], jets[30];
   ///////////////////////////////
   LeptonsP muons_;
   LeptonsP electrons_;
   muons_ = new Leptons;
   electrons_ = new Leptons;
   JetsP jets_;
   jets_ = new Jets;

   double metTrue_, pfmet_, mvamet_;
   double metTruePx_, pfmetPx_, mvametPx_;
   double metTruePy_, pfmetPy_, mvametPy_;
   double metTruePhi_, pfmetPhi_, mvametPhi_;

   double kinweight=0, Nu1Et=0, Nu2Et=0, t1M=0, t2M=0, l1Pt=0, l2Pt=0, j1Pt=0, j2Pt=0, ttM=0, j1CSV=0, j2CSV=0;
   int genMu=0, genEl=0, genNJet=0, NJet=0, Nnu=0;
   double METnu_=-10, METnuphi_=-10;
   TTree *newTree = new TTree("kinsolution","KIN solution");
   newTree->Branch("kinweight",&kinweight, "kinweight/d");
   newTree->Branch("Nu1Et",&Nu1Et, "Nu1Et/d");
   newTree->Branch("Nu2Et",&Nu2Et, "Nu2Et/d");
 /*  newTree->Branch("t1M",&t1M, "t1M/d");
   newTree->Branch("t2M",&t2M, "t2M/d");
   newTree->Branch("l1Pt",&l1Pt, "l1Pt/d");
   newTree->Branch("l2Pt",&l2Pt, "l2Pt/d");
   newTree->Branch("j1Pt",&j1Pt, "j1Pt/d");
   newTree->Branch("j2Pt",&j2Pt, "j2Pt/d");
   newTree->Branch("j1CSV",&j1CSV, "j1CSV/d");
   newTree->Branch("j2CSV",&j2CSV, "j2CSV/d");
*/
//   newTree->Branch("ttM",&ttM, "ttM/d");
   newTree->Branch("genMu",  &genMu,   "genMu/I");
   newTree->Branch("genEl",  &genEl,   "genEl/I");
   newTree->Branch("genNJet",&genNJet, "genNJet/I");

   newTree->Branch("NJet",&NJet, "NJet/I");
   newTree->Branch("Nnu",&Nnu, "Nnu/I");

   newTree->Branch("METnu",      &METnu_,       "METnu/d");
   newTree->Branch("METnuphi",   &METnuphi_,    "METnuphi/d");

   newTree->Branch("metTrue",   &metTrue_,    "metTrue/d");
   newTree->Branch("metTruePx", &metTruePx_,  "metTruePx/d");
   newTree->Branch("metTruePy", &metTruePy_,  "metTruePy/d");
   newTree->Branch("metTruePhi",&metTruePhi_, "metTruePhi/d");

   newTree->Branch("pfmet",    &pfmet_,     "pfmet/d"  );
   newTree->Branch("pfmetPx",  &pfmetPx_,   "pfmetPx/d"  );
   newTree->Branch("pfmetPy",  &pfmetPy_,   "pfmetPy/d"  );
   newTree->Branch("pfmetPhi", &pfmetPhi_,  "pfmetPhi/d"  );

   newTree->Branch("mvamet",    &mvamet_,     "mvamet/d" );
   newTree->Branch("mvametPx",  &mvametPx_,   "mvametPx/d" );
   newTree->Branch("mvametPy",  &mvametPy_,   "mvametPy/d" );
   newTree->Branch("mvametPhi", &mvametPhi_,  "mvametPhi/d" );

   LeptonsP genParticles_;
   genParticles_ = new Leptons;

   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) 
   {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      //if(jentry%100==0) cout << "id : "<< jentry << endl;
//////////////////////////////////////////////////////// 
      muons_->clear();     electrons_->clear();
      jets_->clear();      genParticles_->clear();
      genEl=0;        genMu=0;
      genNJet=0;      NJet=0;

      met.SetPtEtaPhiM(met_pt,0,met_phi,0);
      metMVA.SetPtEtaPhiM(metMVA_pt,0,metMVA_phi,0);
      metGen.SetPtEtaPhiM(metGEN_pt,0,metGEN_phi,0);

      metTrue_ = metGEN_pt;;
      pfmet_   = met_pt;
      mvamet_  = metMVA_pt;
      
      metTruePx_ = metGen.Px();
      pfmetPx_   = met.Px();
      mvametPx_  = metMVA.Px();
      
      metTruePy_ = metGen.Py();
      pfmetPy_   = met.Py();
      mvametPy_  = metMVA.Py(); 
      
      metTruePhi_ = metGEN_phi;
      pfmetPhi_   = met_phi;
      mvametPhi_  = metMVA_phi;
/////////////////////////////////
      for(int i=0;i<genParticles_pdgId->size();i++ )
      {
          if(abs(genParticles_pdgId->at(i)) == 11 ) { genEl++;  }
          if(abs(genParticles_pdgId->at(i)) == 13 ) { genMu++;  }

          if(abs(genParticles_pdgId->at(i))==12 || abs(genParticles_pdgId->at(i))==14 || abs(genParticles_pdgId->at(i))==16 )// pdgId12=Electron Nutrion  14=muon Nutrio14=muon Nutrionn
          //if(abs(genParticles_pdgId->at(i))==12 || abs(genParticles_pdgId->at(i))==14 || abs(genParticles_pdgId->at(i))==16)
          {
         //    std::cout << "gen Ptls"<< genParticles_pt->size()  <<" : " <<  genParticles_pt->at(i) << " : " << genParticles_phi->at(i) << std::endl;
              double x_ = genParticles_pt->at(i)*TMath::Cos(genParticles_phi->at(i));
              double y_ = genParticles_pt->at(i)*TMath::Sin(genParticles_phi->at(i));
              double z_ = genParticles_pt->at(i)*TMath::SinH(genParticles_eta->at(i));
              double e_ = TMath::Sqrt(x_*x_+y_*y_+z_*z_);
              Lepton gP_(TLorentzVector( x_,y_,z_,e_), 0, 0, genParticles_pdgId->at(i));
              genParticles_->push_back(gP_); 
          }
      }
      Nnu = genParticles_->size();
      if(genMu<1) continue; // for MuMu decay

      for(int i=0;i<electrons_pt->size();i++ )
      {
    //     std::cout << "electron"<< electrons_pt->size()  <<" : " <<  electrons_pt->at(i) << std::endl;
        //electrons[i].SetPtEtaPhiM( electrons_pt->at(i),  electrons_eta->at(i),  electrons_phi->at(i),0);
         if(electrons_pt->at(i)>20. && abs(electrons_eta->at(i))<2.4 && electrons_type->at(i)>99)
         {
            double x_ = electrons_pt->at(i)*TMath::Cos(electrons_phi->at(i));
            double y_ = electrons_pt->at(i)*TMath::Sin(electrons_phi->at(i));
            double z_ = electrons_pt->at(i)*TMath::SinH(electrons_eta->at(i));
        //jets_[i].SetPtEtaPhiM( genJets_pt->at(i),  genJets_eta->at(i),  genJets_phi->at(i),genJets_m->at(i));
            double e_ = TMath::Sqrt(x_*x_+y_*y_+z_*z_+electrons_m->at(i)*electrons_m->at(i));
            Lepton el_(TLorentzVector( x_,y_,z_,e_), electrons_relIso->at(i), electrons_Q->at(i),11);
            electrons_->push_back(el_);
         }
      }
      std::sort(electrons_->begin(), electrons_->end(), compByPtLep);
      for(int i=0;i<muons_pt->size();i++ )
      {
         if(muons_pt->at(i)>20. && abs(muons_eta->at(i))<2.4 && muons_type->at(i)>9)
         {
          //  nLep++;
            double x_ = muons_pt->at(i)*TMath::Cos(muons_phi->at(i));
            double y_ = muons_pt->at(i)*TMath::Sin(muons_phi->at(i));
            double z_ = muons_pt->at(i)*TMath::SinH(muons_eta->at(i));
            double e_ = TMath::Sqrt(x_*x_+y_*y_+z_*z_+muons_m->at(i)*muons_m->at(i));
            Lepton mu_(TLorentzVector( x_,y_,z_,e_), muons_relIso->at(i), muons_Q->at(i),13);
            muons_->push_back(mu_);
         }
      }
      std::sort(muons_->begin(), muons_->end(), compByPtLep);
      
      int njet =0, nBtagL=0, ngenjet = 0;

      for(int i=0;i<jets_pt->size();i++ )
      {
         if(jets_pt->at(i)>30 && abs(jets_eta->at(i))<2.5 )//&& !overlapEl && !overlapMu)
         {
            bool  isFill =true;
            double x_ = jets_pt->at(i)*TMath::Cos(jets_phi->at(i));
            double y_ = jets_pt->at(i)*TMath::Sin(jets_phi->at(i));
            double z_ = jets_pt->at(i)*sinh(jets_eta->at(i));
            double e_ = TMath::Sqrt(x_*x_+y_*y_+z_*z_+jets_m->at(i)*jets_m->at(i));
            Jet jet_(TLorentzVector( x_,y_,z_,e_),jets_bTag->at(i),jets_partonflavor->at(i));

            for(int j=0;j<electrons_->size();j++ )
            {
                if( fabs(electrons_->at(j).vec_.DeltaR(jet_.vec_))<0.5) isFill=false;
            }   
            for(int j=0;j<muons_->size();j++ )
            {
                if( fabs(muons_->at(j).vec_.DeltaR(jet_.vec_))<0.5) isFill=false;
            }
            if(isFill)
            {  
               njet++;
               if (jets_bTag->at(i)>0.244) nBtagL++;//
               jets_->push_back(jet_);
            }
         }
      }
      std::sort(jets_->begin(), jets_->end(), compByCSVJet);
      NJet=njet;

//      int nBtagL=0;
      for(int i=0;i<genJets_pt->size();i++ )
      {
        if (genJets_pt->at(i)>30 && fabs(genJets_eta->at(i))<2.4 ) ngenjet++;
      }
      genNJet=ngenjet;

      TLorentzVector metnu__(0,0,0,0);
      for(int i=0;i<genParticles_->size();i++)
      {
          if(i==0) metnu__ = genParticles_->at(i).vec_; 
          else     metnu__ = (metnu__+genParticles_->at(i).vec_);  
      }
      METnu_ = metnu__.Et();
      METnuphi_ = metnu__.Phi();
 

//////////////////////////////////////////////////    
     bool precut = false;
     if (!strcmp(DecayMode,"MuMu"))  precut = (muons_->size()>1);
     if (!strcmp(DecayMode,"ElEl"))  precut = (electrons_->size()>1);
     if (!strcmp(DecayMode,"MuEl"))  precut = (muons_->size()>0 && electrons_->size()> 0);
     bool metcut = ( met_pt > 30 );

   //////////////pre selection     
     if (precut)
     {
         TLorentzVector Z ;
         bool opp=false, iso=false; 
         if(!strcmp(DecayMode, "MuMu")) 
         {
              Z = ((muons_->at(0).vec_)+(muons_->at(1).vec_));
              opp = (muons_->at(0).Q_*muons_->at(1).Q_<0) && (Z.M()>12.); 
              iso = ((muons_->at(0).Iso_<0.15) && (muons_->at(1).Iso_<0.15));

              // only for MuMu
              if(opp && iso ) newTree->Fill();
         }
         if(!strcmp(DecayMode, "ElEl")) 
         {
              Z = ((electrons_->at(0).vec_)+(electrons_->at(1).vec_)); 
              opp = (electrons_->at(0).Q_*electrons_->at(1).Q_<0) && (Z.M()>12.); 
         }
         if(!strcmp(DecayMode, "MuEl")) 
         {
              Z = ((muons_->at(0).vec_)+(electrons_->at(0).vec_));     
              opp = (muons_->at(0).Q_*electrons_->at(0).Q_<0) && (Z.M()>12.);     
         }
     }
  }
}

