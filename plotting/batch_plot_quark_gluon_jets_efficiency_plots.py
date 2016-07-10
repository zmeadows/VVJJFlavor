from ROOT import *
import atlas_style

import os
import math
from sys import argv, exit

from plot_base import *
from plot_util import *
from plot_vvjj import *

RAW_INPUT_PATH = argv[1]
OUTPUT_ROOT_DIR = argv[2]

RAW_TFILE = TFile(RAW_INPUT_PATH, "READ")

if not RAW_TFILE:
    print("Failed to open raw control plot file: " + RAW_INPUT_PATH)
    print "exiting..."
    exit(1)

gROOT.SetBatch()
sane_defaults(wide_plot = True)
TGaxis.SetMaxDigits(4)

OUTPUT_DIR = OUTPUT_ROOT_DIR + "/quark_gluon_jets_efficiency"
make_dir(OUTPUT_ROOT_DIR)
make_dir(OUTPUT_DIR)

def make_eff_plot(h_num, h_den):
    tgraph_eff = TGraphAsymmErrors()
    tgraph_eff.Divide(h_num, h_den, "n")
    return tgraph_eff

class PlotQuarkGluonJetEfficiency(PlotBase):
    def __init__(self,
            h_q_num, h_q_den,
            h_g_num, h_g_den,
            **kwargs):

        super(PlotQuarkGluonJetEfficiency, self).__init__(**kwargs)

        self.tgraph_q_eff = make_eff_plot(h_q_num, h_q_den)
        self.tgraph_g_eff = make_eff_plot(h_g_num, h_g_den)

        new_x_min = self.tgraph_q_eff.GetXaxis().GetXmin()
        new_x_max = self.tgraph_q_eff.GetXaxis().GetXmax()

        if (self.x_max): new_x_max = self.x_max
        if (self.x_min): new_x_min = self.x_min

        self.tgraph_q_eff.GetXaxis().SetRangeUser(new_x_min , new_x_max)
        self.tgraph_g_eff.GetXaxis().SetRangeUser(new_x_min , new_x_max)

        self.tgraph_q_eff.SetMaximum(self.y_max)
        self.tgraph_g_eff.SetMaximum(self.y_max)

        if (self.y_min != None):
            self.tgraph_q_eff.SetMinimum(self.y_min)
            self.tgraph_g_eff.SetMinimum(self.y_min)

        self.canvas.RedrawAxis()

        # axis titles
        x_units_str = ""
        if self.x_units != "":
            x_units_str = "[" + self.x_units + "]"

        bin_width = h_q_num.GetXaxis().GetBinWidth(1)
        y_axis_label_str = "Selection Efficiency / " + format_bin_width(bin_width) + " " + self.x_units

        for graph in [self.tgraph_q_eff, self.tgraph_g_eff]:
            graph.GetYaxis().SetTitle(y_axis_label_str)
            graph.GetYaxis().SetTitleOffset(1.5)
            graph.GetYaxis().SetLabelOffset(0.01)
            graph.GetXaxis().SetTitle(get_vvjj_axis_title(self.name) + " " + x_units_str)

        set_mc_style_marker(self.tgraph_q_eff, kBlue, shape = 22, alpha = 0.8)
        set_mc_style_marker(self.tgraph_g_eff, kRed, shape = 23, alpha = 0.8)

        self.tgraph_q_eff.Draw("APE")
        self.tgraph_g_eff.Draw("PE,same")

        self.leg.AddEntry(self.tgraph_q_eff, "quark jets")
        self.leg.AddEntry(self.tgraph_g_eff, "gluon jets")
        self.leg.Draw()

        self.canvas.RedrawAxis()
        self.canvas.Update()
        self.canvas.Modified()

DEFAULT_EXTRA_LINES = [ "Pythia 8 QCD dijet" ]

def make_quark_gluon_jet_efficiency_plot(num_var_name, den_var_name, **kwargs):
    return PlotQuarkGluonJetEfficiency(
            RAW_TFILE.Get(num_var_name + "_q"),
            RAW_TFILE.Get(den_var_name + "_q"),
            RAW_TFILE.Get(num_var_name + "_g"),
            RAW_TFILE.Get(den_var_name + "_g"),
            extra_lines_loc = [0.2,0.83],
            legend_loc = [0.72,0.91,0.94,0.77],
            **kwargs)

def make_first_jet_pt_efficiency_plot(tag_string, **kwargs):
    num_name = "first_jet_pt_" + tag_string
    return make_quark_gluon_jet_efficiency_plot(
            num_name,
            "first_jet_pt",
            name = num_name + "_efficiency",
            extra_legend_lines = DEFAULT_EXTRA_LINES + [get_vvjj_selection_tex(tag_string)],
            x_min = 500,
            x_max = 1500,
            y_min = 0.000001,
            **kwargs
            )

def make_second_jet_pt_efficiency_plot(tag_string, **kwargs):
    num_name = "second_jet_pt_" + tag_string
    return make_quark_gluon_jet_efficiency_plot(
            num_name,
            "second_jet_pt",
            name = num_name + "_efficiency",
            extra_legend_lines = DEFAULT_EXTRA_LINES + [get_vvjj_selection_tex(tag_string)],
            x_min = 500,
            x_max = 1500,
            y_min = 0.000001,
            **kwargs
            )

efficiency_plots = [
        make_first_jet_pt_efficiency_plot("partial_ntrk", y_max = 0.7),

        make_first_jet_pt_efficiency_plot("W_partial_mass", y_max = 0.2),
        make_first_jet_pt_efficiency_plot("W_partial_D2", y_max = 1.5),
        make_first_jet_pt_efficiency_plot("W_partial_massD2", y_max = 0.1),
        make_first_jet_pt_efficiency_plot("W_partial_massNtrk", y_max = 0.15),
        make_first_jet_pt_efficiency_plot("W_partial_ntrkD2", y_max = 0.4),
        make_first_jet_pt_efficiency_plot("W_full", y_max = 0.04),

        make_first_jet_pt_efficiency_plot("Z_partial_mass", y_max = 0.2),
        make_first_jet_pt_efficiency_plot("Z_partial_D2", y_max = 1.5),
        make_first_jet_pt_efficiency_plot("Z_partial_massD2", y_max = 0.1),
        make_first_jet_pt_efficiency_plot("Z_partial_massNtrk", y_max = 0.15),
        make_first_jet_pt_efficiency_plot("Z_partial_ntrkD2", y_max = 0.4),
        make_first_jet_pt_efficiency_plot("Z_full", y_max = 0.04),

        make_second_jet_pt_efficiency_plot("partial_ntrk", y_max = 0.8),

        make_second_jet_pt_efficiency_plot("W_partial_mass", y_max = 1.5),
        make_second_jet_pt_efficiency_plot("W_partial_D2", y_max = 0.4),
        make_second_jet_pt_efficiency_plot("W_partial_massD2", y_max = 0.1),
        make_second_jet_pt_efficiency_plot("W_partial_massNtrk", y_max = 0.4),
        make_second_jet_pt_efficiency_plot("W_partial_ntrkD2", y_max = 0.4),
        make_second_jet_pt_efficiency_plot("W_full", y_max = 0.06),

        make_second_jet_pt_efficiency_plot("Z_partial_mass", y_max = 0.5),
        make_second_jet_pt_efficiency_plot("Z_partial_D2", y_max = 0.4),
        make_second_jet_pt_efficiency_plot("Z_partial_massD2", y_max = 0.1),
        make_second_jet_pt_efficiency_plot("Z_partial_massNtrk", y_max = 0.3),
        make_second_jet_pt_efficiency_plot("Z_partial_ntrkD2", y_max = 0.3),
        make_second_jet_pt_efficiency_plot("Z_full", y_max = 0.05)
    ]

for i in range(len(efficiency_plots)):
    efficiency_plots[i].print_to_file(OUTPUT_DIR + "/" + efficiency_plots[i].name + ".pdf")

