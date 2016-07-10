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

OUTPUT_DIR = OUTPUT_ROOT_DIR + "/qq_qg_gg_events"
make_dir(OUTPUT_ROOT_DIR)
make_dir(OUTPUT_DIR)

DEFAULT_EXTRA_LINES = [ "Pythia 8 QCD dijet" ]

class PlotEventQuarkGluonControl(PlotBase):
    def __init__(self,
            h_qq,
            h_qg,
            h_gg,
            normalize = False,
            **kwargs):

        super(PlotEventQuarkGluonControl, self).__init__(**kwargs)

        self.h_qq = h_qq.Clone()
        self.h_qg = h_qg.Clone()
        self.h_gg = h_gg.Clone()

        # make sure the histograms exist
        if (not self.h_qq):
            raise IOError("could not locate histogram: " + self.h_qq.GetName())

        if (not self.h_qg):
            raise IOError("could not locate histogram: " + self.h_qg.GetName())

        if (not self.h_gg):
            raise IOError("could not locate histogram: " + self.h_gg.GetName())

        assert(self.h_qq.GetSize() == self.h_qg.GetSize())
        assert(self.h_qq.GetSize() == self.h_gg.GetSize())

        if normalize:
            qg_qq_norm_ratio = self.h_qg.Integral() / self.h_qq.Integral()
            qg_gg_norm_ratio = self.h_qg.Integral() / self.h_gg.Integral()
            self.h_qq.Scale(qg_qq_norm_ratio)
            self.h_gg.Scale(qg_gg_norm_ratio)

        new_x_min = self.h_qq.GetXaxis().GetXmin()
        new_x_max = self.h_qq.GetXaxis().GetXmax()

        if (self.x_max): new_x_max = self.x_max
        if (self.x_min): new_x_min = self.x_min

        self.h_qg.GetXaxis().SetRangeUser(new_x_min , new_x_max)
        self.h_qq.GetXaxis().SetRangeUser(new_x_min , new_x_max)
        self.h_gg.GetXaxis().SetRangeUser(new_x_min , new_x_max)

        if self.show_overflow:
            fill_overflow(self.h_qg)
            fill_overflow(self.h_qq)
            fill_overflow(self.h_gg)

        if self.show_underflow:
            fill_underflow(self.h_qg)
            fill_underflow(self.h_qq)
            fill_underflow(self.h_gg)

        self.h_qg.SetMinimum(0.01)
        self.h_qq.SetMinimum(0.01)
        self.h_gg.SetMinimum(0.01)

        if self.log_scale: self.canvas.SetLogy()

        # rescale y-axis to add/subtract empty space (for readability)
        if self.empty_scale != 1:
            y_max = max([self.h_qg.GetMaximum(), self.h_qq.GetMaximum(), self.h_gg.GetMaximum()])

            if self.log_scale:
                y_max *= 10 * self.empty_scale
            else:
                y_max *= self.empty_scale

            self.h_qg.SetMaximum(y_max)
            self.h_qq.SetMaximum(y_max)
            self.h_gg.SetMaximum(y_max)

        if (self.y_min != None):
            self.h_qg.SetMinimum(self.y_min)
            self.h_qq.SetMinimum(self.y_min)
            self.h_gg.SetMinimum(self.y_min)

        # axis titles
        x_units_str = ""
        if self.x_units != "":
            x_units_str = "[" + self.x_units + "]"

        bin_width = self.h_qg.GetXaxis().GetBinWidth(1)
        y_axis_label_str = "Arbitrary Units / " + format_bin_width(bin_width) + " " + self.x_units

        for h in [self.h_qg, self.h_qq, self.h_gg]:
            h.GetYaxis().SetTitle(y_axis_label_str)
            h.GetYaxis().SetTitleOffset(1.5)
            h.GetYaxis().SetLabelOffset(0.01)
            h.GetXaxis().SetTitle(get_vvjj_axis_title(self.name) + " " + x_units_str)

        if normalize:
            set_mc_style_marker(self.h_qg, kBlue, shape = 23, alpha = 0.8)
            set_mc_style_marker(self.h_qq, kGreen, shape = 22, alpha = 0.8)
            set_mc_style_marker(self.h_gg, kRed, shape = 21, alpha = 0.8)
        else:
            set_mc_style_marker(self.h_qg, kBlue, shape = 23, alpha = 0.8)
            set_mc_style_marker(self.h_qq, kGreen, shape = 22, alpha = 0.8)
            set_mc_style_marker(self.h_gg, kRed, shape = 21, alpha = 0.8)

        if normalize:
            self.h_qg.Draw("PE1")
            self.h_qq.Draw("PE1,same")
            self.h_gg.Draw("PE1,same")
        else:
            self.h_qg.Draw("hist")
            self.h_gg.Draw("hist,same")
            self.h_qq.Draw("hist,same")

        self.leg.AddEntry(self.h_qg, "quark-gluon")
        self.leg.AddEntry(self.h_qq, "quark-quark")
        self.leg.AddEntry(self.h_gg, "gluon-gluon")
        self.leg.Draw()

        self.canvas.RedrawAxis()
        self.canvas.Update()
        self.canvas.Modified()


def make_quark_gluon_event_control_plot(var_name, **kwargs):
    return PlotEventQuarkGluonControl(
            RAW_TFILE.Get(var_name + "_qq"),
            RAW_TFILE.Get(var_name + "_qg"),
            RAW_TFILE.Get(var_name + "_gg"),
            extra_lines_loc = [0.2,0.83],
            legend_loc = [0.74,0.92,0.94,0.80],
            **kwargs)

def make_dijet_control_plot(var_name, **kwargs):
    return make_quark_gluon_event_control_plot(
        var_name,
        empty_scale = 5.0,
        extra_legend_lines = DEFAULT_EXTRA_LINES + [get_vvjj_selection_tex(var_name)],
        log_scale = True,
        x_min = 1000,
        x_max = 3500,
        x_title = "m_{JJ}",
        **kwargs
        )

def make_dijet_control_plot_area_normalized(var_name):
    return make_dijet_control_plot(
            var_name,
            name = var_name + "_area_normalized",
            normalize = True)

def make_dijet_control_plot_lumi_normalized(var_name):
    return make_dijet_control_plot(
            var_name,
            name = var_name + "_lumi_normalized",
            normalize = False)

dijet_mass_lumi_norm = make_dijet_control_plot_lumi_normalized("dijet_mass")

dijet_control_plots_area_normalized = [ make_dijet_control_plot_area_normalized(x) for x in [
        "dijet_mass",
        "dijet_mass_WW_full",
        "dijet_mass_WZ_full",
        "dijet_mass_ZZ_full",
        "dijet_mass_partial_ntrk",
        "dijet_mass_WW_partial_mass",
        "dijet_mass_WZ_partial_mass",
        "dijet_mass_ZZ_partial_mass",
        "dijet_mass_WW_partial_massD2",
        "dijet_mass_WZ_partial_massD2",
        "dijet_mass_ZZ_partial_massD2",
        "dijet_mass_WW_partial_massNtrk",
        "dijet_mass_WZ_partial_massNtrk",
        "dijet_mass_ZZ_partial_massNtrk",
        "dijet_mass_WW_partial_ntrkD2",
        "dijet_mass_WZ_partial_ntrkD2",
        "dijet_mass_ZZ_partial_ntrkD2",
        "dijet_mass_WW_partial_D2",
        "dijet_mass_WZ_partial_D2",
        "dijet_mass_ZZ_partial_D2"
        ]]

dijet_mass_lumi_norm.print_to_file(OUTPUT_DIR + "/" + dijet_mass_lumi_norm.name + ".pdf")

for i in range(len(dijet_control_plots_area_normalized)):
    dijet_control_plots_area_normalized[i].print_to_file(
            OUTPUT_DIR + "/" + dijet_control_plots_area_normalized[i].name + ".pdf")

