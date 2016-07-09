#ifndef VVJJSelector_h
#define VVJJSelector_h

#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <string>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TSelector.h>

enum class EventFlavorTopo {
    QuarkQuark,
    QuarkGluon,
    GluonGluon
};

enum class JetTopo {
    Quark,
    Gluon
};

class TH1Topo {
    private:
        TH1F* h_inclusive;
        TH1F* h_q;
        TH1F* h_g;
        TH1F* h_qq;
        TH1F* h_qg;
        TH1F* h_gg;

        std::unordered_map<std::string, TH1F*> hs_inclusive_tagged;
        std::unordered_map<std::string, TH1F*> hs_q_tagged;
        std::unordered_map<std::string, TH1F*> hs_g_tagged;
        std::unordered_map<std::string, TH1F*> hs_qq_tagged;
        std::unordered_map<std::string, TH1F*> hs_qg_tagged;
        std::unordered_map<std::string, TH1F*> hs_gg_tagged;

    public:
        TH1Topo(std::string var_name_, float x_min_, float x_max_, float bin_spacing_);
        virtual ~TH1Topo(void);

        const std::string var_name;
        const float x_min;
        const float x_max;
        const float bin_spacing;
        const int num_bins;

        void fill_inclusive(float val, float weight);
        void fill_inclusive_tagged(std::string tag_name, bool tag_condition,
                float val, float weight);

        void fill_event_topo(EventFlavorTopo event_topo, float val, float weight);
        void fill_event_topo_tagged(EventFlavorTopo event_topo, std::string tag_name,
                bool tag_condition, float val, float weight);

        void fill_jet_topo(JetTopo jet_topo, float val, float weight);
        void fill_jet_topo_tagged(JetTopo jet_topo, std::string tag_name, bool tag_condition,
                float val, float weight);

        void write_all_histograms(void) const;

        ClassDef(TH1Topo, 0);
};

void RunVVJJSelector(std::string input_path, std::string output_path);

class VVJJSelector : public TSelector {
    public :
        TTree          *fChain;   //!pointer to the analyzed TTree or TChain

        const std::string output_path;

        int num_events_total;
        int num_events_baseline_selection;

        std::unique_ptr<TH1Topo> h_first_jet_pt;
        std::unique_ptr<TH1Topo> h_first_jet_eta;
        std::unique_ptr<TH1Topo> h_first_jet_phi;
        std::unique_ptr<TH1Topo> h_first_jet_m;
        std::unique_ptr<TH1Topo> h_first_jet_D2;
        std::unique_ptr<TH1Topo> h_first_jet_ungNtrk;

        std::unique_ptr<TH1Topo> h_second_jet_pt;
        std::unique_ptr<TH1Topo> h_second_jet_eta;
        std::unique_ptr<TH1Topo> h_second_jet_phi;
        std::unique_ptr<TH1Topo> h_second_jet_m;
        std::unique_ptr<TH1Topo> h_second_jet_D2;
        std::unique_ptr<TH1Topo> h_second_jet_ungNtrk;

        std::unique_ptr<TH1Topo> h_dijet_mass;

        // Declaration of leaf types
        Double_t        weight;
        Double_t        pileup_weight;
        Double_t        jet1_pt;
        Double_t        jet1_phi;
        Double_t        jet1_eta;
        Double_t        jet1_m;
        Double_t        jet1_y;
        Double_t        jet1_nMuSeg;
        Double_t        jet1_nSubJets;
        Double_t        jet1_upt;
        Double_t        jet1_ueta;
        Double_t        jet1_uphi;
        Double_t        jet1_um;
        Double_t        jet1_d2;
        Double_t        jet1_ntrk;
        Double_t        jet1_ungrtrk500;
        Double_t        jet1_ungrtrkW500;
        Double_t        jet1_nconst;
        Double_t        first_jet_pt;
        Double_t        first_jet_eta;
        Double_t        first_jet_phi;
        Double_t        first_jet_m;
        Double_t        first_jet_D2;
        Double_t        first_jet_ntrk;
        Double_t        first_jet_passedWSubstructure;
        Double_t        first_jet_passedZSubstructure;
        Double_t        first_jet_passedWMassCut;
        Double_t        first_jet_passedZMassCut;
        Double_t        first_jet_pdgid;
        Double_t        jet2_pt;
        Double_t        jet2_phi;
        Double_t        jet2_eta;
        Double_t        jet2_m;
        Double_t        jet2_y;
        Double_t        jet2_nMuSeg;
        Double_t        jet2_nSubJets;
        Double_t        jet2_upt;
        Double_t        jet2_ueta;
        Double_t        jet2_uphi;
        Double_t        jet2_um;
        Double_t        jet2_d2;
        Double_t        jet2_ntrk;
        Double_t        jet2_ungrtrk500;
        Double_t        jet2_ungrtrkW500;
        Double_t        jet2_nconst;
        Double_t        second_jet_pt;
        Double_t        second_jet_eta;
        Double_t        second_jet_phi;
        Double_t        second_jet_m;
        Double_t        second_jet_D2;
        Double_t        second_jet_ntrk;
        Double_t        second_jet_passedWSubstructure;
        Double_t        second_jet_passedZSubstructure;
        Double_t        second_jet_passedWMassCut;
        Double_t        second_jet_passedZMassCut;
        Double_t        second_jet_pdgid;
        Double_t        jet1_cpt;
        Double_t        jet1_ceta;
        Double_t        jet1_cphi;
        Double_t        jet1_cm;
        Double_t        jet1_ctdr;
        Double_t        jet1_ctpt;
        Double_t        jet1_cteta;
        Double_t        jet1_ctphi;
        Double_t        jet1_ctm;
        Double_t        jet1_tpt;
        Double_t        jet1_teta;
        Double_t        jet1_tphi;
        Double_t        jet1_tm;
        Double_t        jet1_td2;
        Double_t        jet1_tdr;
        Double_t        jet2_tpt;
        Double_t        jet2_teta;
        Double_t        jet2_tphi;
        Double_t        jet2_tm;
        Double_t        jet2_td2;
        Double_t        jet2_tdr;
        Double_t        jet1_cyfilt;
        Double_t        jet1_cntrk;
        Double_t        jet1_cnconst;
        Double_t        jet1_cungrtrk500;
        Double_t        jet1_cungrtrkW500;
        Double_t        jet2_cpt;
        Double_t        jet2_ceta;
        Double_t        jet2_cphi;
        Double_t        jet2_cm;
        Double_t        jet2_ctpt;
        Double_t        jet2_cteta;
        Double_t        jet2_ctphi;
        Double_t        jet2_ctm;
        Double_t        jet2_cyfilt;
        Double_t        jet2_cntrk;
        Double_t        jet2_cnconst;
        Double_t        jet2_cungrtrk500;
        Double_t        jet2_cungrtrkW500;
        Double_t        jet12_m;
        Double_t        jet12_um;
        Double_t        jet12_cm;
        Double_t        dijet_mass_massordered;
        Double_t        ptasym;
        Double_t        dyjj;
        Double_t        uptasym;
        Double_t        udyjj;
        Double_t        cptasym;
        Double_t        cdyjj;
        Double_t        npv0;
        Double_t        avgMu;
        Double_t        avgIntPerX;
        Double_t        actMu;
        Int_t           run;
        ULong64_t       event;
        Double_t        n_muons;
        Double_t        n_elecs;
        Double_t        n_jets;
        Double_t        n_sigMu;
        Double_t        n_sigEl;
        Double_t        hasPV;
        Double_t        passHLT_J460_A10R_L1J100;
        Double_t        passHLT_J360_A10R_L1J100;

        // List of branches
        TBranch        *b_weight;   //!
        TBranch        *b_pileup_weight;   //!
        TBranch        *b_jet1_pt;   //!
        TBranch        *b_jet1_phi;   //!
        TBranch        *b_jet1_eta;   //!
        TBranch        *b_jet1_m;   //!
        TBranch        *b_jet1_y;   //!
        TBranch        *b_jet1_nMuSeg;   //!
        TBranch        *b_jet1_nSubJets;   //!
        TBranch        *b_jet1_upt;   //!
        TBranch        *b_jet1_ueta;   //!
        TBranch        *b_jet1_uphi;   //!
        TBranch        *b_jet1_um;   //!
        TBranch        *b_jet1_d2;   //!
        TBranch        *b_jet1_ntrk;   //!
        TBranch        *b_jet1_ungrtrk500;   //!
        TBranch        *b_jet1_ungrtrkW500;   //!
        TBranch        *b_jet1_nconst;   //!
        TBranch        *b_first_jet_pt;   //!
        TBranch        *b_first_jet_eta;   //!
        TBranch        *b_first_jet_phi;   //!
        TBranch        *b_first_jet_m;   //!
        TBranch        *b_first_jet_D2;   //!
        TBranch        *b_first_jet_ntrk;   //!
        TBranch        *b_first_jet_passedWSubstructure;   //!
        TBranch        *b_first_jet_passedZSubstructure;   //!
        TBranch        *b_first_jet_passedWMassCut;   //!
        TBranch        *b_first_jet_passedZMassCut;   //!
        TBranch        *b_first_jet_pdgid;   //!
        TBranch        *b_jet2_pt;   //!
        TBranch        *b_jet2_phi;   //!
        TBranch        *b_jet2_eta;   //!
        TBranch        *b_jet2_m;   //!
        TBranch        *b_jet2_y;   //!
        TBranch        *b_jet2_nMuSeg;   //!
        TBranch        *b_jet2_nSubJets;   //!
        TBranch        *b_jet2_upt;   //!
        TBranch        *b_jet2_ueta;   //!
        TBranch        *b_jet2_uphi;   //!
        TBranch        *b_jet2_um;   //!
        TBranch        *b_jet2_d2;   //!
        TBranch        *b_jet2_ntrk;   //!
        TBranch        *b_jet2_ungrtrk500;   //!
        TBranch        *b_jet2_ungrtrkW500;   //!
        TBranch        *b_jet2_nconst;   //!
        TBranch        *b_second_jet_pt;   //!
        TBranch        *b_second_jet_eta;   //!
        TBranch        *b_second_jet_phi;   //!
        TBranch        *b_second_jet_m;   //!
        TBranch        *b_second_jet_D2;   //!
        TBranch        *b_second_jet_ntrk;   //!
        TBranch        *b_second_jet_passedWSubstructure;   //!
        TBranch        *b_second_jet_passedZSubstructure;   //!
        TBranch        *b_second_jet_passedWMassCut;   //!
        TBranch        *b_second_jet_passedZMassCut;   //!
        TBranch        *b_second_jet_pdgid;   //!
        TBranch        *b_jet1_cpt;   //!
        TBranch        *b_jet1_ceta;   //!
        TBranch        *b_jet1_cphi;   //!
        TBranch        *b_jet1_cm;   //!
        TBranch        *b_jet1_ctdr;   //!
        TBranch        *b_jet1_ctpt;   //!
        TBranch        *b_jet1_cteta;   //!
        TBranch        *b_jet1_ctphi;   //!
        TBranch        *b_jet1_ctm;   //!
        TBranch        *b_jet1_tpt;   //!
        TBranch        *b_jet1_teta;   //!
        TBranch        *b_jet1_tphi;   //!
        TBranch        *b_jet1_tm;   //!
        TBranch        *b_jet1_td2;   //!
        TBranch        *b_jet1_tdr;   //!
        TBranch        *b_jet2_tpt;   //!
        TBranch        *b_jet2_teta;   //!
        TBranch        *b_jet2_tphi;   //!
        TBranch        *b_jet2_tm;   //!
        TBranch        *b_jet2_td2;   //!
        TBranch        *b_jet2_tdr;   //!
        TBranch        *b_jet1_cyfilt;   //!
        TBranch        *b_jet1_cntrk;   //!
        TBranch        *b_jet1_cnconst;   //!
        TBranch        *b_jet1_cungrtrk500;   //!
        TBranch        *b_jet1_cungrtrkW500;   //!
        TBranch        *b_jet2_cpt;   //!
        TBranch        *b_jet2_ceta;   //!
        TBranch        *b_jet2_cphi;   //!
        TBranch        *b_jet2_cm;   //!
        TBranch        *b_jet2_ctpt;   //!
        TBranch        *b_jet2_cteta;   //!
        TBranch        *b_jet2_ctphi;   //!
        TBranch        *b_jet2_ctm;   //!
        TBranch        *b_jet2_cyfilt;   //!
        TBranch        *b_jet2_cntrk;   //!
        TBranch        *b_jet2_cnconst;   //!
        TBranch        *b_jet2_cungrtrk500;   //!
        TBranch        *b_jet2_cungrtrkW500;   //!
        TBranch        *b_jet12_m;   //!
        TBranch        *b_jet12_um;   //!
        TBranch        *b_jet12_cm;   //!
        TBranch        *b_dijet_mass_massordered;   //!
        TBranch        *b_ptasym;   //!
        TBranch        *b_dyjj;   //!
        TBranch        *b_uptasym;   //!
        TBranch        *b_udyjj;   //!
        TBranch        *b_cptasym;   //!
        TBranch        *b_cdyjj;   //!
        TBranch        *b_npv0;   //!
        TBranch        *b_avgMu;   //!
        TBranch        *b_avgIntPerX;   //!
        TBranch        *b_actMu;   //!
        TBranch        *b_run;   //!
        TBranch        *b_event;   //!
        TBranch        *b_n_muons;   //!
        TBranch        *b_n_elecs;   //!
        TBranch        *b_n_jets;   //!
        TBranch        *b_n_sigMu;   //!
        TBranch        *b_n_sigEl;   //!
        TBranch        *b_hasPV;   //!
        TBranch        *b_passHLT_J460_A10R_L1J100;   //!
        TBranch        *b_passHLT_J360_A10R_L1J100;   //!

        VVJJSelector(std::string output_path_);

        virtual ~VVJJSelector() { }
        virtual Int_t   Version() const { return 2; }
        virtual void    Begin(TTree *tree);
        virtual void    SlaveBegin(TTree *tree);
        virtual void    Init(TTree *tree);
        virtual Bool_t  Notify();
        virtual Bool_t  Process(Long64_t entry);
        virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
        virtual void    SetOption(const char *option) { fOption = option; }
        virtual void    SetObject(TObject *obj) { fObject = obj; }
        virtual void    SetInputList(TList *input) { fInput = input; }
        virtual TList  *GetOutputList() const { return fOutput; }
        virtual void    SlaveTerminate();
        virtual void    Terminate();

        ClassDef(VVJJSelector,0);
};

#endif

#ifdef VVJJSelector_cxx
void VVJJSelector::Init(TTree *tree)
{
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set branch addresses and branch pointers
    if (!tree) return;
    fChain = tree;
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("weight", &weight, &b_weight);
    fChain->SetBranchAddress("pileup_weight", &pileup_weight, &b_pileup_weight);
    fChain->SetBranchAddress("jet1_pt", &jet1_pt, &b_jet1_pt);
    fChain->SetBranchAddress("jet1_phi", &jet1_phi, &b_jet1_phi);
    fChain->SetBranchAddress("jet1_eta", &jet1_eta, &b_jet1_eta);
    fChain->SetBranchAddress("jet1_m", &jet1_m, &b_jet1_m);
    fChain->SetBranchAddress("jet1_y", &jet1_y, &b_jet1_y);
    fChain->SetBranchAddress("jet1_nMuSeg", &jet1_nMuSeg, &b_jet1_nMuSeg);
    fChain->SetBranchAddress("jet1_nSubJets", &jet1_nSubJets, &b_jet1_nSubJets);
    fChain->SetBranchAddress("jet1_upt", &jet1_upt, &b_jet1_upt);
    fChain->SetBranchAddress("jet1_ueta", &jet1_ueta, &b_jet1_ueta);
    fChain->SetBranchAddress("jet1_uphi", &jet1_uphi, &b_jet1_uphi);
    fChain->SetBranchAddress("jet1_um", &jet1_um, &b_jet1_um);
    fChain->SetBranchAddress("jet1_d2", &jet1_d2, &b_jet1_d2);
    fChain->SetBranchAddress("jet1_ntrk", &jet1_ntrk, &b_jet1_ntrk);
    fChain->SetBranchAddress("jet1_ungrtrk500", &jet1_ungrtrk500, &b_jet1_ungrtrk500);
    fChain->SetBranchAddress("jet1_ungrtrkW500", &jet1_ungrtrkW500, &b_jet1_ungrtrkW500);
    fChain->SetBranchAddress("jet1_nconst", &jet1_nconst, &b_jet1_nconst);
    fChain->SetBranchAddress("first_jet_pt", &first_jet_pt, &b_first_jet_pt);
    fChain->SetBranchAddress("first_jet_eta", &first_jet_eta, &b_first_jet_eta);
    fChain->SetBranchAddress("first_jet_phi", &first_jet_phi, &b_first_jet_phi);
    fChain->SetBranchAddress("first_jet_m", &first_jet_m, &b_first_jet_m);
    fChain->SetBranchAddress("first_jet_D2", &first_jet_D2, &b_first_jet_D2);
    fChain->SetBranchAddress("first_jet_ntrk", &first_jet_ntrk, &b_first_jet_ntrk);
    fChain->SetBranchAddress("first_jet_passedWSubstructure", &first_jet_passedWSubstructure, &b_first_jet_passedWSubstructure);
    fChain->SetBranchAddress("first_jet_passedZSubstructure", &first_jet_passedZSubstructure, &b_first_jet_passedZSubstructure);
    fChain->SetBranchAddress("first_jet_passedWMassCut", &first_jet_passedWMassCut, &b_first_jet_passedWMassCut);
    fChain->SetBranchAddress("first_jet_passedZMassCut", &first_jet_passedZMassCut, &b_first_jet_passedZMassCut);
    fChain->SetBranchAddress("first_jet_pdgid", &first_jet_pdgid, &b_first_jet_pdgid);
    fChain->SetBranchAddress("jet2_pt", &jet2_pt, &b_jet2_pt);
    fChain->SetBranchAddress("jet2_phi", &jet2_phi, &b_jet2_phi);
    fChain->SetBranchAddress("jet2_eta", &jet2_eta, &b_jet2_eta);
    fChain->SetBranchAddress("jet2_m", &jet2_m, &b_jet2_m);
    fChain->SetBranchAddress("jet2_y", &jet2_y, &b_jet2_y);
    fChain->SetBranchAddress("jet2_nMuSeg", &jet2_nMuSeg, &b_jet2_nMuSeg);
    fChain->SetBranchAddress("jet2_nSubJets", &jet2_nSubJets, &b_jet2_nSubJets);
    fChain->SetBranchAddress("jet2_upt", &jet2_upt, &b_jet2_upt);
    fChain->SetBranchAddress("jet2_ueta", &jet2_ueta, &b_jet2_ueta);
    fChain->SetBranchAddress("jet2_uphi", &jet2_uphi, &b_jet2_uphi);
    fChain->SetBranchAddress("jet2_um", &jet2_um, &b_jet2_um);
    fChain->SetBranchAddress("jet2_d2", &jet2_d2, &b_jet2_d2);
    fChain->SetBranchAddress("jet2_ntrk", &jet2_ntrk, &b_jet2_ntrk);
    fChain->SetBranchAddress("jet2_ungrtrk500", &jet2_ungrtrk500, &b_jet2_ungrtrk500);
    fChain->SetBranchAddress("jet2_ungrtrkW500", &jet2_ungrtrkW500, &b_jet2_ungrtrkW500);
    fChain->SetBranchAddress("jet2_nconst", &jet2_nconst, &b_jet2_nconst);
    fChain->SetBranchAddress("second_jet_pt", &second_jet_pt, &b_second_jet_pt);
    fChain->SetBranchAddress("second_jet_eta", &second_jet_eta, &b_second_jet_eta);
    fChain->SetBranchAddress("second_jet_phi", &second_jet_phi, &b_second_jet_phi);
    fChain->SetBranchAddress("second_jet_m", &second_jet_m, &b_second_jet_m);
    fChain->SetBranchAddress("second_jet_D2", &second_jet_D2, &b_second_jet_D2);
    fChain->SetBranchAddress("second_jet_ntrk", &second_jet_ntrk, &b_second_jet_ntrk);
    fChain->SetBranchAddress("second_jet_passedWSubstructure", &second_jet_passedWSubstructure, &b_second_jet_passedWSubstructure);
    fChain->SetBranchAddress("second_jet_passedZSubstructure", &second_jet_passedZSubstructure, &b_second_jet_passedZSubstructure);
    fChain->SetBranchAddress("second_jet_passedWMassCut", &second_jet_passedWMassCut, &b_second_jet_passedWMassCut);
    fChain->SetBranchAddress("second_jet_passedZMassCut", &second_jet_passedZMassCut, &b_second_jet_passedZMassCut);
    fChain->SetBranchAddress("second_jet_pdgid", &second_jet_pdgid, &b_second_jet_pdgid);
    fChain->SetBranchAddress("jet1_cpt", &jet1_cpt, &b_jet1_cpt);
    fChain->SetBranchAddress("jet1_ceta", &jet1_ceta, &b_jet1_ceta);
    fChain->SetBranchAddress("jet1_cphi", &jet1_cphi, &b_jet1_cphi);
    fChain->SetBranchAddress("jet1_cm", &jet1_cm, &b_jet1_cm);
    fChain->SetBranchAddress("jet1_ctdr", &jet1_ctdr, &b_jet1_ctdr);
    fChain->SetBranchAddress("jet1_ctpt", &jet1_ctpt, &b_jet1_ctpt);
    fChain->SetBranchAddress("jet1_cteta", &jet1_cteta, &b_jet1_cteta);
    fChain->SetBranchAddress("jet1_ctphi", &jet1_ctphi, &b_jet1_ctphi);
    fChain->SetBranchAddress("jet1_ctm", &jet1_ctm, &b_jet1_ctm);
    fChain->SetBranchAddress("jet1_tpt", &jet1_tpt, &b_jet1_tpt);
    fChain->SetBranchAddress("jet1_teta", &jet1_teta, &b_jet1_teta);
    fChain->SetBranchAddress("jet1_tphi", &jet1_tphi, &b_jet1_tphi);
    fChain->SetBranchAddress("jet1_tm", &jet1_tm, &b_jet1_tm);
    fChain->SetBranchAddress("jet1_td2", &jet1_td2, &b_jet1_td2);
    fChain->SetBranchAddress("jet1_tdr", &jet1_tdr, &b_jet1_tdr);
    fChain->SetBranchAddress("jet2_tpt", &jet2_tpt, &b_jet2_tpt);
    fChain->SetBranchAddress("jet2_teta", &jet2_teta, &b_jet2_teta);
    fChain->SetBranchAddress("jet2_tphi", &jet2_tphi, &b_jet2_tphi);
    fChain->SetBranchAddress("jet2_tm", &jet2_tm, &b_jet2_tm);
    fChain->SetBranchAddress("jet2_td2", &jet2_td2, &b_jet2_td2);
    fChain->SetBranchAddress("jet2_tdr", &jet2_tdr, &b_jet2_tdr);
    fChain->SetBranchAddress("jet1_cyfilt", &jet1_cyfilt, &b_jet1_cyfilt);
    fChain->SetBranchAddress("jet1_cntrk", &jet1_cntrk, &b_jet1_cntrk);
    fChain->SetBranchAddress("jet1_cnconst", &jet1_cnconst, &b_jet1_cnconst);
    fChain->SetBranchAddress("jet1_cungrtrk500", &jet1_cungrtrk500, &b_jet1_cungrtrk500);
    fChain->SetBranchAddress("jet1_cungrtrkW500", &jet1_cungrtrkW500, &b_jet1_cungrtrkW500);
    fChain->SetBranchAddress("jet2_cpt", &jet2_cpt, &b_jet2_cpt);
    fChain->SetBranchAddress("jet2_ceta", &jet2_ceta, &b_jet2_ceta);
    fChain->SetBranchAddress("jet2_cphi", &jet2_cphi, &b_jet2_cphi);
    fChain->SetBranchAddress("jet2_cm", &jet2_cm, &b_jet2_cm);
    fChain->SetBranchAddress("jet2_ctpt", &jet2_ctpt, &b_jet2_ctpt);
    fChain->SetBranchAddress("jet2_cteta", &jet2_cteta, &b_jet2_cteta);
    fChain->SetBranchAddress("jet2_ctphi", &jet2_ctphi, &b_jet2_ctphi);
    fChain->SetBranchAddress("jet2_ctm", &jet2_ctm, &b_jet2_ctm);
    fChain->SetBranchAddress("jet2_cyfilt", &jet2_cyfilt, &b_jet2_cyfilt);
    fChain->SetBranchAddress("jet2_cntrk", &jet2_cntrk, &b_jet2_cntrk);
    fChain->SetBranchAddress("jet2_cnconst", &jet2_cnconst, &b_jet2_cnconst);
    fChain->SetBranchAddress("jet2_cungrtrk500", &jet2_cungrtrk500, &b_jet2_cungrtrk500);
    fChain->SetBranchAddress("jet2_cungrtrkW500", &jet2_cungrtrkW500, &b_jet2_cungrtrkW500);
    fChain->SetBranchAddress("jet12_m", &jet12_m, &b_jet12_m);
    fChain->SetBranchAddress("jet12_um", &jet12_um, &b_jet12_um);
    fChain->SetBranchAddress("jet12_cm", &jet12_cm, &b_jet12_cm);
    fChain->SetBranchAddress("dijet_mass_massordered", &dijet_mass_massordered, &b_dijet_mass_massordered);
    fChain->SetBranchAddress("ptasym", &ptasym, &b_ptasym);
    fChain->SetBranchAddress("dyjj", &dyjj, &b_dyjj);
    fChain->SetBranchAddress("uptasym", &uptasym, &b_uptasym);
    fChain->SetBranchAddress("udyjj", &udyjj, &b_udyjj);
    fChain->SetBranchAddress("cptasym", &cptasym, &b_cptasym);
    fChain->SetBranchAddress("cdyjj", &cdyjj, &b_cdyjj);
    fChain->SetBranchAddress("npv0", &npv0, &b_npv0);
    fChain->SetBranchAddress("avgMu", &avgMu, &b_avgMu);
    fChain->SetBranchAddress("avgIntPerX", &avgIntPerX, &b_avgIntPerX);
    fChain->SetBranchAddress("actMu", &actMu, &b_actMu);
    fChain->SetBranchAddress("run", &run, &b_run);
    fChain->SetBranchAddress("event", &event, &b_event);
    fChain->SetBranchAddress("n_muons", &n_muons, &b_n_muons);
    fChain->SetBranchAddress("n_elecs", &n_elecs, &b_n_elecs);
    fChain->SetBranchAddress("n_jets", &n_jets, &b_n_jets);
    fChain->SetBranchAddress("n_sigMu", &n_sigMu, &b_n_sigMu);
    fChain->SetBranchAddress("n_sigEl", &n_sigEl, &b_n_sigEl);
    fChain->SetBranchAddress("hasPV", &hasPV, &b_hasPV);
    fChain->SetBranchAddress("passHLT_J460_A10R_L1J100", &passHLT_J460_A10R_L1J100, &b_passHLT_J460_A10R_L1J100);
    fChain->SetBranchAddress("passHLT_J360_A10R_L1J100", &passHLT_J360_A10R_L1J100, &b_passHLT_J360_A10R_L1J100);
}

Bool_t VVJJSelector::Notify()
{
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return kTRUE;
}

    template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}

#endif // #ifdef VVJJSelector_cxx

