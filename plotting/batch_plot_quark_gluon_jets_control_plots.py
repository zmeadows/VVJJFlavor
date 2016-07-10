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
TGaxis.SetMaxDigits(3)

OUTPUT_DIR = OUTPUT_ROOT_DIR + "/quark_gluon_jets"
make_dir(OUTPUT_ROOT_DIR)
make_dir(OUTPUT_DIR)

class PlotQuarkGluonControl(PlotBase):
    def __init__(self,
            h_quark,
            h_gluon,
            normalize = False,
            **kwargs):

        super(PlotQuarkGluonControl, self).__init__(**kwargs)

        self.h_quark = h_quark.Clone()
        self.h_gluon = h_gluon.Clone()

        # make sure the histograms exist
        if (not self.h_quark):
            raise IOError("could not locate histogram: " + self.h_quark.GetName())

        if (not self.h_gluon):
            raise IOError("could not locate histogram: " + self.h_gluon.GetName())

        assert(self.h_quark.GetSize() == self.h_gluon.GetSize())

        if normalize:
            quark_gluon_norm_ratio = self.h_quark.Integral() / self.h_gluon.Integral()
            self.h_gluon.Scale(quark_gluon_norm_ratio)

        new_x_min = self.h_quark.GetXaxis().GetXmin()
        new_x_max = self.h_quark.GetXaxis().GetXmax()

        if (self.x_max): new_x_max = self.x_max
        if (self.x_min): new_x_min = self.x_min

        self.h_quark.GetXaxis().SetRangeUser(new_x_min , new_x_max)
        self.h_gluon.GetXaxis().SetRangeUser(new_x_min , new_x_max)

        if self.show_overflow:
            fill_overflow(self.h_quark)
            fill_overflow(self.h_gluon)

        if self.show_underflow:
            fill_underflow(self.h_quark)
            fill_underflow(self.h_gluon)

        self.h_quark.SetMinimum(0.01)
        self.h_gluon.SetMinimum(0.01)

        if self.log_scale: self.canvas.SetLogy()

        # rescale y-axis to add/subtract empty space (for readability)
        if self.empty_scale != 1:
            y_max = max([self.h_quark.GetMaximum(), self.h_gluon.GetMaximum()])

            if self.log_scale:
                y_max *= 10 * self.empty_scale
            else:
                y_max *= self.empty_scale

            self.h_quark.SetMaximum(y_max)
            self.h_gluon.SetMaximum(y_max)

        if (self.y_min != None):
            self.h_quark.SetMinimum(self.y_min)
            self.h_gluon.SetMinimum(self.y_min)

        # axis titles
        x_units_str = ""
        if self.x_units != "":
            x_units_str = "[" + self.x_units + "]"

        bin_width = self.h_quark.GetXaxis().GetBinWidth(1)
        y_axis_label_str = "Arbitrary Units / " + format_bin_width(bin_width) + " " + self.x_units

        for h in [self.h_gluon, self.h_quark]:
            h.GetYaxis().SetTitle(y_axis_label_str)
            h.GetYaxis().SetTitleOffset(1.5)
            h.GetYaxis().SetLabelOffset(0.01)
            h.GetXaxis().SetTitle(get_vvjj_axis_title(self.name) + " " + x_units_str)

        set_mc_style_marker(self.h_quark, kBlue, shape = 22, alpha = 0.8)
        set_mc_style_marker(self.h_gluon, kRed, shape = 23, alpha = 0.8)

        if normalize:
            self.h_quark.Draw("PE1")
            self.h_gluon.Draw("PE1,same")
        else:
            self.h_gluon.Draw("hist")
            self.h_quark.Draw("hist,same")

        self.leg.AddEntry(self.h_quark, "quark jets")
        self.leg.AddEntry(self.h_gluon, "gluon jets")
        self.leg.Draw()

        self.canvas.RedrawAxis()
        self.canvas.Update()
        self.canvas.Modified()

FIRST_JET, SECOND_JET, BOTH_JETS = range(3)

def make_quark_gluon_jet_control_plot(var_name, which_jet, **kwargs):
    h_q1 = RAW_TFILE.Get("first_" + var_name + "_q")
    h_q2 = RAW_TFILE.Get("second_" + var_name + "_q")

    h_g1 = RAW_TFILE.Get("first_" + var_name + "_g")
    h_g2 = RAW_TFILE.Get("second_" + var_name + "_g")

    h_q = h_q1
    h_g = h_g1

    if which_jet == FIRST_JET:
        h_q = h_q1
        h_g = h_g1
    elif which_jet == SECOND_JET:
        h_q = h_q2
        h_g = h_g2
    elif which_jet == BOTH_JETS:
        h_q.Add(h_q1,h_q2)
        h_g.Add(h_g1,h_g2)
    else:
        print("improper 'which_jet' argument passed to make_quark_gluon_jets_control_plot.")
        print("exiting...")

    return PlotQuarkGluonControl(
            h_q,
            h_g,
            extra_lines_loc = [0.2,0.83],
            legend_loc = [0.74,0.92,0.94,0.82],
            extra_legend_lines = [ "Pythia 8 QCD dijet" ],
            **kwargs)

def make_first_jet_control_plot(var_name, **kwargs):
    return [
        make_quark_gluon_jet_control_plot( var_name, FIRST_JET,
            name = "first_" + var_name + "_lumi_normalized", **kwargs),
        make_quark_gluon_jet_control_plot( var_name, FIRST_JET,
            name = "first_" + var_name + "_area_normalized", normalize = True, **kwargs)
        ]

def make_second_jet_control_plot(var_name, **kwargs):
    return [
        make_quark_gluon_jet_control_plot( var_name, SECOND_JET,
            name = "second_" + var_name + "_lumi_normalized", **kwargs),
        make_quark_gluon_jet_control_plot( var_name, SECOND_JET,
            name = "second_" + var_name + "_area_normalized", normalize = True, **kwargs),
        ]

def make_both_jets_control_plot(var_name, **kwargs):
    return [
        make_quark_gluon_jet_control_plot( var_name, BOTH_JETS,
            name = "both_" + var_name + "_lumi_normalized", **kwargs),
        make_quark_gluon_jet_control_plot( var_name, BOTH_JETS,
            name = "both_" + var_name + "_area_normalized", normalize = True, **kwargs)
	]

def make_all_jet_control_plots(var_name, **kwargs):
    plots = []
    plots.append(make_first_jet_control_plot(var_name, **kwargs))
    plots.append(make_second_jet_control_plot(var_name, **kwargs))
    plots.append(make_both_jets_control_plot(var_name, **kwargs))
    return flatten(plots)

quark_gluon_jet_plots = flatten([
        make_all_jet_control_plots(
            "jet_m",
            x_min = 50,
            x_max = 200,
            empty_scale = 1.5,
            ),

        make_all_jet_control_plots(
            "jet_ntrk",
            x_units = "",
            empty_scale = 1.6,
            x_max = 80,
            ),

        make_all_jet_control_plots(
            "jet_D2",
            x_units = "",
            empty_scale = 1.5,
            ),

        make_all_jet_control_plots(
            "jet_phi",
            empty_scale = 1.5,
            x_units = "rad",
            ),

        make_all_jet_control_plots(
            "jet_eta",
            empty_scale = 1.5,
            x_units = "",
            ),

        make_all_jet_control_plots(
            "jet_pt",
            empty_scale = 1.5,
            log_scale = True,
            x_min = 500,
            x_max = 2500
            )
        ])

for i in range(len(quark_gluon_jet_plots)):
    quark_gluon_jet_plots[i].print_to_file(OUTPUT_DIR + "/" + quark_gluon_jet_plots[i].name + ".pdf")
