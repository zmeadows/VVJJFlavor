#define TH1Topo_cxx

#include <TH1F.h>

#include <cassert>
#include <unordered_map>

#include "TH1Topo.h"

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
