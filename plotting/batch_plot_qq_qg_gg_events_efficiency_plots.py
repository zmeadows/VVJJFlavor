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

OUTPUT_DIR = OUTPUT_ROOT_DIR + "/qq_qg_gg_events_efficiency"
make_dir(OUTPUT_ROOT_DIR)
make_dir(OUTPUT_DIR)

def make_eff_plot(h_num, h_den):
    tgraph_eff = TGraphAsymmErrors()
    tgraph_eff.Divide(h_num, h_den, "n")
    return tgraph_eff

class PlotEventQuarkGluonEfficiency(PlotBase):
    def __init__(self,
            h_qq_num, h_qq_den,
            h_qg_num, h_qg_den,
            h_gg_num, h_gg_den,
            **kwargs):

        super(PlotEventQuarkGluonEfficiency, self).__init__(**kwargs)

        self.tgraph_qq_eff = make_eff_plot(h_qq_num, h_qq_den)
        self.tgraph_qg_eff = make_eff_plot(h_qg_num, h_qg_den)
        self.tgraph_gg_eff = make_eff_plot(h_gg_num, h_gg_den)

        new_x_min = self.tgraph_qq_eff.GetXaxis().GetXmin()
        new_x_max = self.tgraph_qq_eff.GetXaxis().GetXmax()

        if (self.x_max): new_x_max = self.x_max
        if (self.x_min): new_x_min = self.x_min

        self.tgraph_qg_eff.GetXaxis().SetRangeUser(new_x_min , new_x_max)
        self.tgraph_qq_eff.GetXaxis().SetRangeUser(new_x_min , new_x_max)
        self.tgraph_gg_eff.GetXaxis().SetRangeUser(new_x_min , new_x_max)

        self.tgraph_qg_eff.SetMaximum(self.y_max)
        self.tgraph_qq_eff.SetMaximum(self.y_max)
        self.tgraph_gg_eff.SetMaximum(self.y_max)

        if (self.y_min != None):
            self.tgraph_qg_eff.SetMinimum(self.y_min)
            self.tgraph_qq_eff.SetMinimum(self.y_min)
            self.tgraph_gg_eff.SetMinimum(self.y_min)

        self.canvas.RedrawAxis()

        # axis titles
        x_units_str = ""
        if self.x_units != "":
            x_units_str = "[" + self.x_units + "]"

        bin_width = h_qg_num.GetXaxis().GetBinWidth(1)
        y_axis_label_str = "Selection Efficiency / " + format_bin_width(bin_width) + " " + self.x_units

        for graph in [self.tgraph_qg_eff, self.tgraph_qq_eff, self.tgraph_gg_eff]:
            graph.GetYaxis().SetTitle(y_axis_label_str)
            graph.GetYaxis().SetTitleOffset(1.5)
            graph.GetYaxis().SetLabelOffset(0.01)
            graph.GetXaxis().SetTitle(get_vvjj_axis_title(self.name) + " " + x_units_str)

        set_mc_style_marker(self.tgraph_qg_eff, kBlue, shape = 23, alpha = 0.8)
        set_mc_style_marker(self.tgraph_qq_eff, 8, shape = 22, alpha = 0.8)
        set_mc_style_marker(self.tgraph_gg_eff, kRed, shape = 21, alpha = 0.8)

        self.tgraph_qg_eff.Draw("APE")
        self.tgraph_gg_eff.Draw("PE,same")
        self.tgraph_qq_eff.Draw("PE,same")

        self.leg.AddEntry(self.tgraph_qg_eff, "quark-gluon")
        self.leg.AddEntry(self.tgraph_qq_eff, "quark-quark")
        self.leg.AddEntry(self.tgraph_gg_eff, "gluon-gluon")
        self.leg.Draw()

        self.canvas.RedrawAxis()
        self.canvas.Update()
        self.canvas.Modified()

DEFAULT_EXTRA_LINES = [ "Pythia 8 QCD dijet" ]

def make_qq_qg_gg_event_efficiency_plot(num_var_name, den_var_name, **kwargs):
    return PlotEventQuarkGluonEfficiency(
            RAW_TFILE.Get(num_var_name + "_qq"),
            RAW_TFILE.Get(den_var_name + "_qq"),
            RAW_TFILE.Get(num_var_name + "_qg"),
            RAW_TFILE.Get(den_var_name + "_qg"),
            RAW_TFILE.Get(num_var_name + "_gg"),
            RAW_TFILE.Get(den_var_name + "_gg"),
            extra_lines_loc = [0.2,0.83],
            legend_loc = [0.72,0.91,0.94,0.77],
            **kwargs)

def make_dijet_eff_plot(num_var_name, **kwargs):
    return make_qq_qg_gg_event_efficiency_plot(
            num_var_name,
            "dijet_mass",
            name = num_var_name + "_efficiency",
            extra_legend_lines = DEFAULT_EXTRA_LINES + [ get_vvjj_selection_tex(num_var_name) ],
            x_min = 1000,
            x_max = 2500,
            x_title = "m_{JJ}",
            y_min = 0.000001,
            **kwargs
            )

quark_gluon_plots = [
        make_dijet_eff_plot("dijet_mass_partial_ntrk", y_max = 0.45),
        make_dijet_eff_plot("dijet_mass_WW_partial_mass", y_max = 0.1),
        make_dijet_eff_plot("dijet_mass_WZ_partial_mass", y_max = 0.12),
        make_dijet_eff_plot("dijet_mass_ZZ_partial_mass", y_max = 0.1),
        make_dijet_eff_plot("dijet_mass_WW_partial_D2", y_max = 0.5),
        make_dijet_eff_plot("dijet_mass_WZ_partial_D2", y_max = 0.5),
        make_dijet_eff_plot("dijet_mass_ZZ_partial_D2", y_max = 0.5),
        make_dijet_eff_plot("dijet_mass_WW_partial_massD2", y_max = 0.005),
        make_dijet_eff_plot("dijet_mass_WZ_partial_massD2", y_max = 0.005),
        make_dijet_eff_plot("dijet_mass_ZZ_partial_massD2", y_max = 0.005),
        make_dijet_eff_plot("dijet_mass_WW_partial_massNtrk", y_max = 0.02),
        make_dijet_eff_plot("dijet_mass_WZ_partial_massNtrk", y_max = 0.02),
        make_dijet_eff_plot("dijet_mass_ZZ_partial_massNtrk", y_max = 0.02),
        make_dijet_eff_plot("dijet_mass_WW_partial_ntrkD2", y_max = 0.05),
        make_dijet_eff_plot("dijet_mass_WZ_partial_ntrkD2", y_max = 0.05),
        make_dijet_eff_plot("dijet_mass_ZZ_partial_ntrkD2", y_max = 0.05),
        make_dijet_eff_plot("dijet_mass_WW_full", y_max = 0.002),
        make_dijet_eff_plot("dijet_mass_WZ_full", y_max = 0.002),
        make_dijet_eff_plot("dijet_mass_ZZ_full", y_max = 0.002)
    ]

for i in range(len(quark_gluon_plots)):
    quark_gluon_plots[i].print_to_file(OUTPUT_DIR + "/" + quark_gluon_plots[i].name + ".pdf")

