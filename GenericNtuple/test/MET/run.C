void run(char *str, const char* DecayMode){
//void run(){

    gROOT->ProcessLine(".L TtFullLepKinSolver.C+g");
    gROOT->ProcessLine(".L Lepton.h+g");
    gROOT->ProcessLine(".L Jet.h+g");
    gROOT->ProcessLine(".L Event.C+g");
    TFile f(Form("/afs/cern.ch/work/d/dygyun/public/MET/%s.root",str));
//    TFile f(Form("/afs/cern.ch/work/y/youngjo/public/For8Tev/KrAFT_20140224/%s.root",str));
 //   TTree *atree = dynamic_cast<TTree *>(f.Get("MuMu/event"));
   
    TTree *atree = dynamic_cast<TTree *>(f.Get( Form("%s/event",DecayMode) ));
    TH1F *hevents = dynamic_cast<TH1F *>(f.Get( Form("%s/hEventCounter",DecayMode) ));
  

    int nEvents = hevents->GetBinContent(1);
   if (atree != 0)
    {
        Event t(atree);
        TFile fout(Form("result_%s_%s.root",DecayMode,str), "RECREATE");
        t.Loop(str, DecayMode);
  //  cout <<  Form("%s/hEventCounter",DecayMode)  << endl;  
    
  cout << "nEvents: " << nEvents << endl;
 // t.Loop(DecayMode);
        fout.Write();
        fout.Close();
    }
    f.Close();
}
