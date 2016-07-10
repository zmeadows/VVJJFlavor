#ifndef TH1Topo_h
#define TH1Topo_h

#include <unordered_map>

#include <TH1F.h>

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

#endif // #ifdef TH1Topo_h
