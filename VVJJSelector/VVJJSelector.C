#define VVJJSelector_cxx

#include "VVJJSelector.h"

#include <cassert>
#include <sstream>

#include <TH1F.h>
#include <TStyle.h>
#include <TSelector.h>

TH1Topo::TH1Topo(std::string var_name_, float x_min_, float x_max_, float bin_spacing_) :
    h_inclusive(nullptr),
    h_q(nullptr),
    h_g(nullptr),
    h_qq(nullptr),
    h_qg(nullptr),
    h_gg(nullptr),
    var_name(var_name_),
    x_min(x_min_),
    x_max(x_max_),
    bin_spacing(bin_spacing_),
    num_bins( (x_max - x_min) / bin_spacing )
{ }

TH1Topo::~TH1Topo(void)
{
    if (h_inclusive != nullptr)
        delete h_inclusive;
    if (h_q != nullptr)
        delete h_q;
    if (h_g != nullptr)
        delete h_g;
    if (h_qq != nullptr)
        delete h_qq;
    if (h_qg != nullptr)
        delete h_qg;
    if (h_gg != nullptr)
        delete h_gg;

    for (auto const& h : this->hs_inclusive_tagged)
        delete h.second;
    for (auto const& h : this->hs_q_tagged)
        delete h.second;
    for (auto const& h : this->hs_g_tagged)
        delete h.second;
    for (auto const& h : this->hs_qq_tagged)
        delete h.second;
    for (auto const& h : this->hs_qg_tagged)
        delete h.second;
    for (auto const& h : this->hs_gg_tagged)
        delete h.second;
}

void
TH1Topo::fill_inclusive(float val, float weight)
{
    if (h_inclusive == nullptr) {
        h_inclusive = new TH1F(var_name.c_str(), var_name.c_str(), num_bins, x_min, x_max);
        h_inclusive->Sumw2();
    }

    h_inclusive->Fill(val, weight);
}

void
TH1Topo::fill_inclusive_tagged(std::string tag_name, bool tag_condition,
        float val, float weight)
{
    if (!tag_condition) return;

    bool tagged_histogram_exists = hs_inclusive_tagged.find(tag_name) != hs_inclusive_tagged.end();

    if (!tagged_histogram_exists) {
        std::string name = var_name + "_" + tag_name;
        hs_inclusive_tagged.emplace(tag_name,
                new TH1F(name.c_str(), name.c_str(), num_bins, x_min, x_max));
        hs_inclusive_tagged.at(tag_name)->Sumw2();
    }

    hs_inclusive_tagged.at(tag_name)->Fill(val, weight);
}

void
TH1Topo::fill_event_topo(EventFlavorTopo event_topo, float val, float weight)
{
    if (event_topo == EventFlavorTopo::QuarkQuark) {

        if (h_qq == nullptr) {
            std::string name = var_name + "_qq";
            h_qq = new TH1F(name.c_str(), name.c_str(), num_bins, x_min, x_max);
            h_qq->Sumw2();
        }

        h_qq->Fill(val, weight);

    } else if (event_topo == EventFlavorTopo::QuarkGluon) {

        if (h_qg == nullptr) {
            std::string name = var_name + "_qg";
            h_qg = new TH1F(name.c_str(), name.c_str(), num_bins, x_min, x_max);
            h_qg->Sumw2();
        }

        h_qg->Fill(val, weight);

    } else {

        assert(event_topo == EventFlavorTopo::GluonGluon);

        if (h_gg == nullptr) {
            std::string name = var_name + "_gg";
            h_gg = new TH1F(name.c_str(), name.c_str(), num_bins, x_min, x_max);
            h_gg->Sumw2();
        }

        h_gg->Fill(val, weight);
    }
}

void
TH1Topo::fill_event_topo_tagged(EventFlavorTopo event_topo, std::string tag_name,
        bool tag_condition, float val, float weight)
{
    if (!tag_condition) return;

    if (event_topo == EventFlavorTopo::QuarkQuark) {

        bool tagged_qq_histogram_exists =
            hs_qq_tagged.find(tag_name) != hs_qq_tagged.end();

        if (!tagged_qq_histogram_exists) {
            std::string name = var_name + "_" + tag_name + "_qq";
            hs_qq_tagged.emplace(tag_name,
                    new TH1F(name.c_str(), name.c_str(), num_bins, x_min, x_max));
            hs_qq_tagged.at(tag_name)->Sumw2();
        }

        hs_qq_tagged.at(tag_name)->Fill(val, weight);

    } else if (event_topo == EventFlavorTopo::QuarkGluon) {

        bool tagged_qg_histogram_exists =
            hs_qg_tagged.find(tag_name) != hs_qg_tagged.end();

        if (!tagged_qg_histogram_exists) {
            std::string name = var_name + "_" + tag_name + "_qg";
            hs_qg_tagged.emplace(tag_name,
                    new TH1F(name.c_str(), name.c_str(), num_bins, x_min, x_max));
            hs_qg_tagged.at(tag_name)->Sumw2();
        }

        hs_qg_tagged.at(tag_name)->Fill(val, weight);

    } else {
        assert(event_topo == EventFlavorTopo::GluonGluon);

        bool tagged_gg_histogram_exists =
            hs_gg_tagged.find(tag_name) != hs_gg_tagged.end();

        if (!tagged_gg_histogram_exists) {
            std::string name = var_name + "_" + tag_name + "_gg";
            hs_gg_tagged.emplace(tag_name,
                    new TH1F(name.c_str(), name.c_str(), num_bins, x_min, x_max));
            hs_gg_tagged.at(tag_name)->Sumw2();
        }

        hs_gg_tagged.at(tag_name)->Fill(val, weight);
    }
}

void
TH1Topo::fill_jet_topo(JetTopo jet_topo, float val, float weight)
{
    if (jet_topo == JetTopo::Quark) {

        if (h_q == nullptr) {
            std::string name = var_name + "_q";
            h_q = new TH1F(name.c_str(), name.c_str(), num_bins, x_min, x_max);
            h_q->Sumw2();
        }

        h_q->Fill(val, weight);

    } else {

        assert(jet_topo == JetTopo::Gluon);

        if (h_g == nullptr) {
            std::string name = var_name + "_g";
            h_g = new TH1F(name.c_str(), name.c_str(), num_bins, x_min, x_max);
            h_g->Sumw2();
        }

        h_g->Fill(val, weight);
    }
}

void
TH1Topo::fill_jet_topo_tagged(JetTopo jet_topo, std::string tag_name, bool tag_condition,
        float val, float weight)
{
    if (!tag_condition) return;

    if (jet_topo == JetTopo::Quark) {
        bool tagged_q_histogram_exists =
            hs_q_tagged.find(tag_name) != hs_q_tagged.end();

        if (!tagged_q_histogram_exists) {
            std::string name = var_name + "_" + tag_name + "_q";
            hs_q_tagged.emplace(tag_name,
                    new TH1F(name.c_str(), name.c_str(), num_bins, x_min, x_max));
            hs_q_tagged.at(tag_name)->Sumw2();
        }

        hs_q_tagged.at(tag_name)->Fill(val, weight);

    } else {
        assert(jet_topo == JetTopo::Gluon);

        bool tagged_g_histogram_exists =
            hs_g_tagged.find(tag_name) != hs_g_tagged.end();

        if (!tagged_g_histogram_exists) {
            std::string name = var_name + "_" + tag_name + "_g";
            hs_g_tagged.emplace(tag_name,
                    new TH1F(name.c_str(), name.c_str(), num_bins, x_min, x_max));
            hs_g_tagged.at(tag_name)->Sumw2();
        }

        hs_g_tagged.at(tag_name)->Fill(val, weight);
    }
}

void
TH1Topo::write_all_histograms(void) const
{
    if (h_inclusive != nullptr)
        h_inclusive->Write();
    if (h_q != nullptr)
        h_q->Write();
    if (h_g != nullptr)
        h_g->Write();
    if (h_qq != nullptr)
        h_qq->Write();
    if (h_qg != nullptr)
        h_qg->Write();
    if (h_gg != nullptr)
        h_gg->Write();

    for (auto const& h : this->hs_inclusive_tagged)
        h.second->Write();
    for (auto const& h : this->hs_q_tagged)
        h.second->Write();
    for (auto const& h : this->hs_g_tagged)
        h.second->Write();
    for (auto const& h : this->hs_qq_tagged)
        h.second->Write();
    for (auto const& h : this->hs_qg_tagged)
        h.second->Write();
    for (auto const& h : this->hs_gg_tagged)
        h.second->Write();
}

void RunVVJJSelector(std::string input_path, std::string output_path)
{
    // load the input file
    std::ifstream input_file(input_path.c_str(), std::ifstream::in);
    if (!input_file.is_open()) {
        std::cout << "ERROR: failed to open input file list: " << input_path << std::endl;
        return;
    }

    // A map from generator names to vectors of ntuple filepaths that share that generator name.
    // "generator name" = 'data', 'pythia', 'herwig', 'powheg_pythia', etc
    std::map< std::string, std::vector<std::string> > ntuple_filepath_map;

    // temporary variables for clarity in for-loops
    std::string ntuple_path, ntuple_gen;
    std::vector<std::string> ntuple_paths;

    // fill the ntuple path map one line at a time
    while (input_file >> ntuple_path >> ntuple_gen) {
        bool gen_exists_already = ntuple_filepath_map.find(ntuple_gen) != ntuple_filepath_map.end();

        if (!gen_exists_already) {
            std::vector<std::string> unit_vector;
            unit_vector.push_back(ntuple_path);
            ntuple_filepath_map[ntuple_gen] = unit_vector;
        } else {
            ntuple_filepath_map[ntuple_gen] . push_back( ntuple_path );
        }
    }

    // now construct and add files to the TChains
    std::map<std::string, TChain*> tchains;
    Int_t ret_code;

    for (auto const& x : ntuple_filepath_map)
    {
        ntuple_gen = x.first;
        ntuple_paths = x.second;

        bool tchain_exists = tchains.find(ntuple_gen) != tchains.end();

        if (!tchain_exists) {
            // create TChain for particular generator, if it doesn't yet exist
            tchains[ntuple_gen] = new TChain("Nominal");
        }

        // add each ntuple path to the corresponding TChain
        std::cout << std::endl << "### Loading files: " << ntuple_gen << " ###" << std::endl;
        for (auto const& path : ntuple_paths) {
            ret_code = tchains[ntuple_gen]->Add(path.c_str(), 0);

            if (ret_code != 1) {
                std::cout << "ERROR: failed to open input file: " << path << std::endl;
                return;
            } else {
                std::cout << "\t" + path << " FOUND." << std::endl;
            }
        }
        std::cout << std::endl;
    }

    // now actually process the TChains (i.e. ntuples) with the VVJJSelector
    TChain* tchain_gen;
    TSelector* vvjj_selector;

    for (auto& x : tchains)
    {
        vvjj_selector = new VVJJSelector(output_path);

        tchain_gen = x.second;
        tchain_gen->Process(vvjj_selector);
        delete vvjj_selector;
    }
}

VVJJSelector::VVJJSelector(std::string output_path_) :
    fChain(0),
    output_path(output_path_),
    num_entries_processed(0),
    next_print_percent(0.0),
    sum_weights_total(0),
    sum_weights_baseline_selection(0),
    sum_weights_qq(0),
    sum_weights_qg(0),
    sum_weights_gg(0),
    sum_weights_qg_firstjet_quark(0),
    sum_weights_qg_firstjet_gluon(0),
    sum_weights_non_quark_gluon_rejections(0)
{ }

void VVJJSelector::Begin(TTree * /*tree*/)
{
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).

    h_first_jet_pt   = make_unique<TH1Topo>("first_jet_pt"   , 0.   , 4000. , 100);
    h_first_jet_eta  = make_unique<TH1Topo>("first_jet_eta"  , -2.5 , 2.5   , 0.2);
    h_first_jet_phi  = make_unique<TH1Topo>("first_jet_phi"  , -3.2 , 3.2   , 0.2);
    h_first_jet_m    = make_unique<TH1Topo>("first_jet_m"    , 0.   , 400.  , 10.0);
    h_first_jet_D2   = make_unique<TH1Topo>("first_jet_D2"   , 0.   , 5.    , 0.2);
    h_first_jet_ungNtrk = make_unique<TH1Topo>("first_jet_ntrk" , 0.   , 100.  , 2.0);

    h_second_jet_pt   = make_unique<TH1Topo>("second_jet_pt"   , 0.   , 4000. , 100);
    h_second_jet_eta  = make_unique<TH1Topo>("second_jet_eta"  , -2.5 , 2.5   , 0.2);
    h_second_jet_phi  = make_unique<TH1Topo>("second_jet_phi"  , -3.2 , 3.2   , 0.2);
    h_second_jet_m    = make_unique<TH1Topo>("second_jet_m"    , 0.   , 400.  , 10.0);
    h_second_jet_D2   = make_unique<TH1Topo>("second_jet_D2"   , 0.   , 5.    , 0.2);
    h_second_jet_ungNtrk = make_unique<TH1Topo>("second_jet_ntrk" , 0.   , 100.  , 2.0);

    h_dijet_mass = make_unique<TH1Topo>("dijet_mass" , 0. , 8000. , 100);


    TString option = GetOption();
}

void VVJJSelector::SlaveBegin(TTree * /*tree*/)
{
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();

}

Bool_t VVJJSelector::Process(Long64_t entry)
{
    // The Process() function is called for each entry in the tree (or possibly
    // keyed object in the case of PROOF) to be processed. The entry argument
    // specifies which entry in the currently loaded tree is to be processed.
    // It can be passed to either VVJJSelector::GetEntry() or TBranch::GetEntry()
    // to read either all or the required parts of the data. When processing
    // keyed objects with PROOF, the object is already loaded and is available
    // via the fObject pointer.
    //
    // This function should contain the "body" of the analysis. It can contain
    // simple or elaborate selection criteria, run algorithms on the data
    // of the event and typically fill histograms.
    //
    // The processing can be stopped by calling Abort().
    //
    // Use fStatus to set the return value of TTree::Process().
    //
    // The return value is currently not used.

    num_entries_processed++;

    double percent_done = 100 * (float) num_entries_processed / (float) this->fChain->GetEntries();
    if (percent_done >= next_print_percent) {
        std::cout << next_print_percent << "%..." << std::flush;
        if (percent_done == 100.0) {
            std::cout << "DONE." << std::endl;
        } else {
            next_print_percent += 10.0;
        }
    }

    b_weight->GetEntry(entry);
    b_pileup_weight->GetEntry(entry);

    b_dijet_mass_massordered->GetEntry(entry);

    b_first_jet_pt->GetEntry(entry);
    b_first_jet_eta->GetEntry(entry);
    b_first_jet_phi->GetEntry(entry);
    b_first_jet_m->GetEntry(entry);
    b_first_jet_D2->GetEntry(entry);
    b_first_jet_passedWSubstructure->GetEntry(entry);
    b_first_jet_passedZSubstructure->GetEntry(entry);
    b_first_jet_passedWMassCut->GetEntry(entry);
    b_first_jet_passedZMassCut->GetEntry(entry);
    b_first_jet_pdgid->GetEntry(entry);

    b_second_jet_pt->GetEntry(entry);
    b_second_jet_eta->GetEntry(entry);
    b_second_jet_phi->GetEntry(entry);
    b_second_jet_m->GetEntry(entry);
    b_second_jet_D2->GetEntry(entry);
    b_second_jet_passedWSubstructure->GetEntry(entry);
    b_second_jet_passedZSubstructure->GetEntry(entry);
    b_second_jet_passedWMassCut->GetEntry(entry);
    b_second_jet_passedZMassCut->GetEntry(entry);
    b_second_jet_pdgid->GetEntry(entry);

    b_dyjj->GetEntry(entry);
    b_ptasym->GetEntry(entry);
    b_n_muons->GetEntry(entry);
    b_n_elecs->GetEntry(entry);

    b_jet1_m->GetEntry(entry);
    b_jet2_m->GetEntry(entry);
    b_jet1_ungrtrk500->GetEntry(entry);
    b_jet2_ungrtrk500->GetEntry(entry);

    const float full_weight = weight * pileup_weight;

    /****************************/
    /* BASELINE EVENT SELECTION */
    /****************************/

    sum_weights_total += full_weight;

    if (first_jet_pt / 1000. <= 450
            || first_jet_m / 1000. <= 50
            || second_jet_m / 1000. <= 50
            || std::abs(first_jet_eta) >= 2.0
            || std::abs(second_jet_eta) >= 2.0
            || dijet_mass_massordered / 1000 <= 1000
            || std::abs(dyjj) >= 1.2
            || std::abs(ptasym) >= 0.15
            ) return kFALSE;

    sum_weights_baseline_selection += full_weight;

    /***************************/
    /* COMPUTE EXTRA VARIABLES */
    /***************************/

    Double_t first_jet_ungNtrk, second_jet_ungNtrk;

    if (jet1_m >= jet2_m) {
        first_jet_ungNtrk = jet1_ungrtrk500;
        second_jet_ungNtrk = jet2_ungrtrk500;
    } else {
        first_jet_ungNtrk = jet2_ungrtrk500;
        second_jet_ungNtrk = jet1_ungrtrk500;
    }

    const bool first_jet_passedNtrk = first_jet_ungNtrk < 30;
    const bool second_jet_passedNtrk = second_jet_ungNtrk < 30;

    /***************************************/
    /* DETERMINE EVENT/JET TOPOLOGIES/TAGS */
    /***************************************/

    JetTopo first_jet_topo;
    JetTopo second_jet_topo;
    EventFlavorTopo event_topo;

    if (first_jet_pdgid >= 1 && first_jet_pdgid <= 6) {
        first_jet_topo = JetTopo::Quark;
    } else if (first_jet_pdgid == 21) {
        first_jet_topo = JetTopo::Gluon;
    } else {
        sum_weights_non_quark_gluon_rejections += full_weight;
        return kFALSE;
    }

    if (second_jet_pdgid >= 1 && second_jet_pdgid <= 6) {
        second_jet_topo = JetTopo::Quark;
    } else if (second_jet_pdgid == 21) {
        second_jet_topo = JetTopo::Gluon;
    } else {
        sum_weights_non_quark_gluon_rejections += full_weight;
        return kFALSE;
    }

    if (first_jet_topo == JetTopo::Quark && second_jet_topo == JetTopo::Quark) {
        event_topo = EventFlavorTopo::QuarkQuark;
        sum_weights_qq += full_weight;
    } else if (first_jet_topo == JetTopo::Quark && second_jet_topo == JetTopo::Gluon) {
        event_topo = EventFlavorTopo::QuarkGluon;
        sum_weights_qg += full_weight;
    } else if (first_jet_topo == JetTopo::Gluon && second_jet_topo == JetTopo::Quark) {
        event_topo = EventFlavorTopo::QuarkGluon;
        sum_weights_qg += full_weight;
    } else {
        assert(first_jet_topo == JetTopo::Gluon && second_jet_topo == JetTopo::Gluon);
        event_topo = EventFlavorTopo::GluonGluon;
        sum_weights_gg += full_weight;
    }

    if (event_topo == EventFlavorTopo::QuarkGluon) {
        if (first_jet_topo == JetTopo::Quark) {
            sum_weights_qg_firstjet_quark += full_weight;
        } else {
            assert(first_jet_topo == JetTopo::Gluon);
            sum_weights_qg_firstjet_gluon += full_weight;
        }
    }

    first_jet_tag_map["partial_ntrk"]       = first_jet_passedNtrk;
    first_jet_tag_map["W_partial_mass"]     = first_jet_passedWMassCut;
    first_jet_tag_map["W_partial_D2"]       = first_jet_passedWSubstructure;
    first_jet_tag_map["W_partial_massD2"]   = first_jet_passedWSubstructure  && first_jet_passedWMassCut;
    first_jet_tag_map["W_partial_massNtrk"] = first_jet_passedWMassCut       && first_jet_passedNtrk;
    first_jet_tag_map["W_partial_ntrkD2"]   = first_jet_passedWSubstructure  && first_jet_passedNtrk;
    first_jet_tag_map["W_full"]             = first_jet_passedWSubstructure  && first_jet_passedWMassCut  && first_jet_passedNtrk;

    first_jet_tag_map["Z_partial_mass"]     = first_jet_passedZMassCut;
    first_jet_tag_map["Z_partial_D2"]       = first_jet_passedZSubstructure;
    first_jet_tag_map["Z_partial_massD2"]   = first_jet_passedZSubstructure  && first_jet_passedZMassCut;
    first_jet_tag_map["Z_partial_massNtrk"] = first_jet_passedZMassCut       && first_jet_passedNtrk;
    first_jet_tag_map["Z_partial_ntrkD2"]   = first_jet_passedZSubstructure  && first_jet_passedNtrk;
    first_jet_tag_map["Z_full"]             = first_jet_passedZSubstructure  && first_jet_passedZMassCut  && first_jet_passedNtrk;

    second_jet_tag_map["partial_ntrk"]       = second_jet_passedNtrk;
    second_jet_tag_map["W_partial_mass"]     = second_jet_passedWMassCut;
    second_jet_tag_map["W_partial_D2"]       = second_jet_passedWSubstructure;
    second_jet_tag_map["W_partial_massD2"]   = second_jet_passedWSubstructure  && second_jet_passedWMassCut;
    second_jet_tag_map["W_partial_massNtrk"] = second_jet_passedWMassCut       && second_jet_passedNtrk;
    second_jet_tag_map["W_partial_ntrkD2"]   = second_jet_passedWSubstructure  && second_jet_passedNtrk;
    second_jet_tag_map["W_full"]             = second_jet_passedWSubstructure  && second_jet_passedWMassCut  && second_jet_passedNtrk;

    second_jet_tag_map["Z_partial_mass"]     = second_jet_passedZMassCut;
    second_jet_tag_map["Z_partial_D2"]       = second_jet_passedZSubstructure;
    second_jet_tag_map["Z_partial_massD2"]   = second_jet_passedZSubstructure  && second_jet_passedZMassCut;
    second_jet_tag_map["Z_partial_massNtrk"] = second_jet_passedZMassCut       && second_jet_passedNtrk;
    second_jet_tag_map["Z_partial_ntrkD2"]   = second_jet_passedZSubstructure  && second_jet_passedNtrk;
    second_jet_tag_map["Z_full"]             = second_jet_passedZSubstructure  && second_jet_passedZMassCut  && second_jet_passedNtrk;

    event_tag_map["partial_ntrk"] = first_jet_passedNtrk && second_jet_passedNtrk;

    event_tag_map["WW_partial_mass"]     = first_jet_tag_map["W_partial_mass"]     && second_jet_tag_map["W_partial_mass"];
    event_tag_map["WW_partial_D2"]       = first_jet_tag_map["W_partial_D2"]       && second_jet_tag_map["W_partial_D2"];
    event_tag_map["WW_partial_massD2"]   = first_jet_tag_map["W_partial_massD2"]   && second_jet_tag_map["W_partial_massD2"];
    event_tag_map["WW_partial_massNtrk"] = first_jet_tag_map["W_partial_massNtrk"] && second_jet_tag_map["W_partial_massNtrk"];
    event_tag_map["WW_partial_ntrkD2"]   = first_jet_tag_map["W_partial_ntrkD2"]   && second_jet_tag_map["W_partial_ntrkD2"];
    event_tag_map["WW_full"]             = first_jet_tag_map["W_full"]             && second_jet_tag_map["W_full"];

    event_tag_map["WZ_partial_mass"]     = first_jet_tag_map["Z_partial_mass"]     && second_jet_tag_map["W_partial_mass"];
    event_tag_map["WZ_partial_D2"]       = first_jet_tag_map["Z_partial_D2"]       && second_jet_tag_map["W_partial_D2"];
    event_tag_map["WZ_partial_massD2"]   = first_jet_tag_map["Z_partial_massD2"]   && second_jet_tag_map["W_partial_massD2"];
    event_tag_map["WZ_partial_massNtrk"] = first_jet_tag_map["Z_partial_massNtrk"] && second_jet_tag_map["W_partial_massNtrk"];
    event_tag_map["WZ_partial_ntrkD2"]   = first_jet_tag_map["Z_partial_ntrkD2"]   && second_jet_tag_map["W_partial_ntrkD2"];
    event_tag_map["WZ_full"]             = first_jet_tag_map["Z_full"]             && second_jet_tag_map["W_full"];

    event_tag_map["ZZ_partial_mass"]     = first_jet_tag_map["Z_partial_mass"]     && second_jet_tag_map["Z_partial_mass"];
    event_tag_map["ZZ_partial_D2"]       = first_jet_tag_map["Z_partial_D2"]       && second_jet_tag_map["Z_partial_D2"];
    event_tag_map["ZZ_partial_massD2"]   = first_jet_tag_map["Z_partial_massD2"]   && second_jet_tag_map["Z_partial_massD2"];
    event_tag_map["ZZ_partial_massNtrk"] = first_jet_tag_map["Z_partial_massNtrk"] && second_jet_tag_map["Z_partial_massNtrk"];
    event_tag_map["ZZ_partial_ntrkD2"]   = first_jet_tag_map["Z_partial_ntrkD2"]   && second_jet_tag_map["Z_partial_ntrkD2"];
    event_tag_map["ZZ_full"]             = first_jet_tag_map["Z_full"]             && second_jet_tag_map["Z_full"];

    /****************************/
    /* FILL UNTAGGED HISTOGRAMS */
    /****************************/

    h_dijet_mass->fill_inclusive(dijet_mass_massordered / 1000. , full_weight);
    h_dijet_mass->fill_event_topo(event_topo, dijet_mass_massordered / 1000. , full_weight);

    h_first_jet_pt->fill_inclusive(first_jet_pt / 1000. , full_weight);
    h_first_jet_pt->fill_event_topo(event_topo, first_jet_pt / 1000. , full_weight);
    h_first_jet_pt->fill_jet_topo(first_jet_topo, first_jet_pt / 1000. , full_weight);

    h_first_jet_eta->fill_inclusive(first_jet_eta, full_weight);
    h_first_jet_eta->fill_event_topo(event_topo, first_jet_eta, full_weight);
    h_first_jet_eta->fill_jet_topo(first_jet_topo, first_jet_eta, full_weight);

    h_first_jet_phi->fill_inclusive(first_jet_phi, full_weight);
    h_first_jet_phi->fill_event_topo(event_topo, first_jet_phi, full_weight);
    h_first_jet_phi->fill_jet_topo(first_jet_topo, first_jet_phi, full_weight);

    h_first_jet_m->fill_inclusive(first_jet_m / 1000., full_weight);
    h_first_jet_m->fill_event_topo(event_topo, first_jet_m / 1000., full_weight);
    h_first_jet_m->fill_jet_topo(first_jet_topo, first_jet_m / 1000., full_weight);

    h_first_jet_D2->fill_inclusive(first_jet_D2, full_weight);
    h_first_jet_D2->fill_event_topo(event_topo, first_jet_D2, full_weight);
    h_first_jet_D2->fill_jet_topo(first_jet_topo, first_jet_D2, full_weight);

    h_first_jet_ungNtrk->fill_inclusive(first_jet_ungNtrk, full_weight);
    h_first_jet_ungNtrk->fill_event_topo(event_topo, first_jet_ungNtrk, full_weight);
    h_first_jet_ungNtrk->fill_jet_topo(first_jet_topo, first_jet_ungNtrk, full_weight);

    h_second_jet_pt->fill_inclusive(second_jet_pt / 1000. , full_weight);
    h_second_jet_pt->fill_event_topo(event_topo, second_jet_pt / 1000. , full_weight);
    h_second_jet_pt->fill_jet_topo(second_jet_topo, second_jet_pt / 1000. , full_weight);

    h_second_jet_eta->fill_inclusive(second_jet_eta, full_weight);
    h_second_jet_eta->fill_event_topo(event_topo, second_jet_eta, full_weight);
    h_second_jet_eta->fill_jet_topo(second_jet_topo, second_jet_eta, full_weight);

    h_second_jet_phi->fill_inclusive(second_jet_phi, full_weight);
    h_second_jet_phi->fill_event_topo(event_topo, second_jet_phi, full_weight);
    h_second_jet_phi->fill_jet_topo(second_jet_topo, second_jet_phi, full_weight);

    h_second_jet_m->fill_inclusive(second_jet_m / 1000., full_weight);
    h_second_jet_m->fill_event_topo(event_topo, second_jet_m / 1000., full_weight);
    h_second_jet_m->fill_jet_topo(second_jet_topo, second_jet_m / 1000., full_weight);

    h_second_jet_D2->fill_inclusive(second_jet_D2, full_weight);
    h_second_jet_D2->fill_event_topo(event_topo, second_jet_D2, full_weight);
    h_second_jet_D2->fill_jet_topo(second_jet_topo, second_jet_D2, full_weight);

    h_second_jet_ungNtrk->fill_inclusive(second_jet_ungNtrk, full_weight);
    h_second_jet_ungNtrk->fill_event_topo(event_topo, second_jet_ungNtrk, full_weight);
    h_second_jet_ungNtrk->fill_jet_topo(second_jet_topo, second_jet_ungNtrk, full_weight);

    /**************************/
    /* FILL TAGGED HISTOGRAMS */
    /**************************/

    for (auto const& tag : event_tag_map) {
        h_dijet_mass->fill_event_topo_tagged(event_topo, tag.first, tag.second, dijet_mass_massordered / 1000., full_weight);
        h_first_jet_pt->fill_event_topo_tagged(event_topo, tag.first, tag.second, first_jet_pt / 1000., full_weight);
        h_second_jet_pt->fill_event_topo_tagged(event_topo, tag.first, tag.second, second_jet_pt / 1000., full_weight);
        h_first_jet_m->fill_event_topo_tagged(event_topo, tag.first, tag.second, first_jet_m / 1000., full_weight);
        h_second_jet_m->fill_event_topo_tagged(event_topo, tag.first, tag.second, second_jet_m / 1000., full_weight);
    }

    for (auto const& tag : first_jet_tag_map) {
        h_first_jet_pt->fill_jet_topo_tagged(first_jet_topo, tag.first, tag.second, first_jet_pt / 1000., full_weight);
        h_first_jet_m->fill_jet_topo_tagged(first_jet_topo, tag.first, tag.second, first_jet_m / 1000., full_weight);
    }

    for (auto const& tag : second_jet_tag_map) {
        h_second_jet_pt->fill_jet_topo_tagged(second_jet_topo, tag.first, tag.second, second_jet_pt / 1000., full_weight);
        h_second_jet_m->fill_jet_topo_tagged(second_jet_topo, tag.first, tag.second, second_jet_m / 1000., full_weight);
    }

    return kTRUE;
}

void VVJJSelector::SlaveTerminate()
{
    // The SlaveTerminate() function is called after all entries or objects
    // have been processed. When running with PROOF SlaveTerminate() is called
    // on each slave server.

}

void VVJJSelector::Terminate()
{
    // The Terminate() function is the last function to be called during
    // a query. It always runs on the client, it can be used to present
    // the results graphically or save the results to file.

    auto print_percent = [] (Double_t numerator, Double_t denomenator) {
        Double_t percent = 100.0 * numerator / denomenator;

        std::stringstream ss;
        ss.precision(4);

        ss << std::fixed << numerator << " (" << percent << "%)";

        std::cout << ss.str() << std::endl;
    };

    std::cout << std::endl;

    std::cout << "TOTAL EVENT WEIGHT PROCESSED: " << sum_weights_total << std::endl;

    std::cout << "WEIGHT OF EVENTS PASSING BASELINE CUTS: ";
    print_percent(sum_weights_baseline_selection, sum_weights_total);

    std::cout << "WEIGHT OF BASELINE QUARK-QUARK EVENTS: ";
    print_percent(sum_weights_qq, sum_weights_baseline_selection);

    std::cout << "WEIGHT OF BASELINE QUARK-GLUON EVENTS: ";
    print_percent(sum_weights_qg, sum_weights_baseline_selection);

    std::cout << "WEIGHT OF BASELINE GLUON-GLUON EVENTS: ";
    print_percent(sum_weights_gg, sum_weights_baseline_selection);

    std::cout << "WEIGHT OF NON-QUARK-GLUON REJECTED EVENTS: ";
    print_percent(sum_weights_non_quark_gluon_rejections, sum_weights_baseline_selection);

    std::cout << std::endl;

    std::cout << "IN BASELINE QUARK-GLUON EVENTS:" << std::endl;
    std::cout << "\t" << "WEIGHT OF QUARK-INITIATED LEADING JET EVENTS: ";
    print_percent(sum_weights_qg_firstjet_quark, sum_weights_qg);
    std::cout << "\t" << "WEIGHT OF GLUON-INITIATED LEADING JET EVENTS: ";
    print_percent(sum_weights_qg_firstjet_gluon, sum_weights_qg);

    TFile output_file(output_path.c_str(), "RECREATE");

    h_first_jet_pt->write_all_histograms();
    h_first_jet_eta->write_all_histograms();
    h_first_jet_phi->write_all_histograms();
    h_first_jet_m->write_all_histograms();
    h_first_jet_D2->write_all_histograms();
    h_first_jet_ungNtrk->write_all_histograms();

    h_second_jet_pt->write_all_histograms();
    h_second_jet_eta->write_all_histograms();
    h_second_jet_phi->write_all_histograms();
    h_second_jet_m->write_all_histograms();
    h_second_jet_D2->write_all_histograms();
    h_second_jet_ungNtrk->write_all_histograms();

    h_dijet_mass->write_all_histograms();

    output_file.Close();
}
