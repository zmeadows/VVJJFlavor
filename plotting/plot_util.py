from ROOT import *
from math import *
import array as arr
import os

def sane_defaults(wide_plot = False):
    # 43 -> Helvetica fixed size, not based on pad/canvas/whatever size
    gStyle.SetLabelFont(43, "X")
    gStyle.SetLabelFont(43, "Y")
    gStyle.SetLabelFont(43, "Z")
    gStyle.SetTitleFont(43, "X")
    gStyle.SetTitleFont(43, "Y")
    gStyle.SetTitleFont(43, "Z")

    s = 22 if wide_plot else 19

    gStyle.SetLabelSize(s, "X")
    gStyle.SetLabelSize(s, "Y")
    gStyle.SetLabelSize(s, "Z")
    gStyle.SetTitleSize(s, "X")
    gStyle.SetTitleSize(s, "Y")
    gStyle.SetTitleSize(s, "Z")

    gStyle.SetLegendFont(43)
    gStyle.SetLegendBorderSize(0)
    gStyle.SetLegendFillColor(0)
    gStyle.SetStatFont(43)
    gROOT.ForceStyle()

class DMDRawLoader(object):
    ''' A class that holds a TFile, and has convenient methods for
    accessing the histograms it contains. It expects the structure of the
    file to correspond to files produced with dmd-control-plots in the
    DataMCdijets package
    '''

    def __init__(self, filepath):
        self.filepath = filepath
        self.tfile = TFile(self.filepath, "READ")

        # check that the file exists and opened successfully
        if not self.tfile:
            print("Failed to open raw control plot file: " + self.filepath)
            print "exiting..."
            exit(1)

    def get_hist(self, branch, gen_name, var_name):
        ''' Grab a histogram from the file.
        branch: "nominal" or one of the systematics acronyms
        gen_name: "data", "pythia", "herwig", etc
        var_name: "rljet_m", etc
        '''

        hist = self.tfile.Get(branch).Get(gen_name).Get(var_name + "_" + gen_name)
        assert(hist)
        return hist

def set_highres_color_palette():
    NRGBs      = 5
    NCont      = 255
    stops      = [ 0.00, 0.34, 0.61, 0.84, 1.00 ]
    red        = [ 0.00, 0.00, 0.87, 1.00, 0.51 ]
    green      = [ 0.00, 0.81, 1.00, 0.20, 0.00 ]
    blue       = [ 0.51, 1.00, 0.12, 0.00, 0.00 ]
    stopsArray = array('d', stops)
    redArray   = array('d', red)
    greenArray = array('d', green)
    blueArray  = array('d', blue)
    TColor.CreateGradientColorTable(NRGBs, stopsArray, redArray, greenArray, blueArray, NCont)
    gStyle.SetNumberContours(NCont)

def fill_underflow(hist):
    underflow_count = hist.GetBinContent(0)
    hist.SetBinContent(1, hist.GetBinContent(1) + underflow_count)

def fill_overflow(hist):
    max_vis_bin = hist.GetXaxis().GetLast()
    NUM_BINS_TOTAL = hist.GetNbinsX()
    overflow = 0
    for i in range(max_vis_bin + 1, NUM_BINS_TOTAL + 1):
        overflow += hist.GetBinContent(i + 1)
    hist.SetBinContent(max_vis_bin, hist.GetBinContent(max_vis_bin) + overflow)

def format_bin_width(bin_spacing):
    """ format the bin width decimal string for the Y-axis, in a smart way. """
    if bin_spacing < 0.5:
        return str(round(bin_spacing * 20) / 20)
    if bin_spacing < 1.0:
        return str(round(bin_spacing * 10) / 10)
    elif bin_spacing < 10:
        if (int(bin_spacing) == bin_spacing):
            return str(int(round(bin_spacing * 4) / 4))
        else:
            return str(round(bin_spacing * 4) / 4)
    elif bin_spacing < 100:
        return str(round(bin_spacing * 2) / 2)
    else:
        return str(int(round(bin_spacing)))

def set_data_style_simple(hist):
        hist.SetFillStyle(0)
        hist.SetLineColor(kBlack)
        hist.SetLineWidth(2)
        hist.SetMarkerColor(kBlack)
        hist.SetMarkerSize(1)

def set_mc_style_marker(hist, col, shape = 20, line_style = 1, alpha = 1):
        hist.SetFillStyle(0)
        hist.SetFillColor(0)
        hist.SetLineColor(col)
        hist.SetMarkerColorAlpha(col, alpha)
        hist.SetMarkerSize(1.1)
        hist.SetMarkerStyle(shape)
        hist.SetLineStyle(line_style)
        hist.SetLineWidth(2)

def set_mc_style_simple_hist(hist, col):
        hist.SetFillStyle(1001)
        hist.SetFillColor(col)
        hist.SetLineWidth(0)
        hist.SetMarkerSize(0)

def set_mc_sys_err_style(hist):
        hist.SetFillStyle(1001)
        hist.SetFillColorAlpha(kGreen+2,0.5)
        hist.SetLineWidth(0)
        hist.SetMarkerSize(0)

def make_dir(dir_path):
    try:
        os.makedirs(dir_path)
    except OSError:
        if not os.path.isdir(dir_path):
            raise

def make_histo_symm_errs(h_nominal, systematics):
    NUM_BINS = h_nominal.GetSize()

    h_sys = h_nominal.Clone(h_nominal.GetName() + "_sys")

    total_err = [0.0] * NUM_BINS

    # don't forget to include stat. error
    for ibin in range(0, NUM_BINS):
        nom_stat_err = h_nominal.GetBinError(ibin)
        total_err[ibin] += nom_stat_err**2

    for sys_name, (h_sys_up, h_sys_down) in systematics.iteritems():
        assert(h_sys_up.GetSize() == h_nominal.GetSize())
        assert(h_sys_down.GetSize() == h_nominal.GetSize())

        for ibin in range(0, NUM_BINS):
            bin_err_up = h_sys_up.GetBinContent(ibin) - h_nominal.GetBinContent(ibin)
            bin_err_down = h_sys_down.GetBinContent(ibin) - h_nominal.GetBinContent(ibin)

            max_bin_err = max( abs(bin_err_up), abs(bin_err_down) )

            total_err[ibin] += max_bin_err**2

    for ibin in range(0, NUM_BINS):
        h_sys.SetBinError(ibin, sqrt(total_err[ibin]))

    return h_sys

def make_tgraph_asymm_errs(h_nominal, systematics):
    tgraph = TGraphAsymmErrors(h_nominal)
    tgraph.SetName(h_nominal.GetName() + "_graph_errs")

    total_errs_up = [0.0] * (h_nominal.GetSize() - 2)
    total_errs_down = [0.0] * (h_nominal.GetSize() - 2)

    #for ibin in range(1, h_nominal.GetSize() - 1):
    #    nom_stat_err = h_nominal.GetBinError(ibin)
    #    total_errs_up[ibin - 1] += nom_stat_err**2
    #    total_errs_down[ibin - 1] += nom_stat_err**2

    for sys_name, (h_sys_up, h_sys_down) in systematics.iteritems():
        assert(h_sys_up.GetSize() == h_nominal.GetSize())
        assert(h_sys_down.GetSize() == h_nominal.GetSize())

        sys_errs_up = [0.0] * h_nominal.GetSize()
        sys_errs_down = [0.0] * h_nominal.GetSize()

        for ibin in range(1, h_nominal.GetSize() - 1):
            bin_err_up = h_sys_up.GetBinContent(ibin) - h_nominal.GetBinContent(ibin)
            bin_err_down = h_sys_down.GetBinContent(ibin) - h_nominal.GetBinContent(ibin)

            if (bin_err_up * bin_err_down > 0): # same sign up/down shift
                max_bin_err = max( abs(bin_err_up), abs(bin_err_down) )
                bin_err_up = max_bin_err
                bin_err_down = max_bin_err

            sys_errs_up[ibin - 1] += bin_err_up**2.
            sys_errs_down[ibin - 1] += bin_err_down**2.

            total_errs_up[ibin - 1] += bin_err_up**2.
            total_errs_down[ibin - 1] += bin_err_down**2.

    for ibin in range(0, h_nominal.GetSize() - 2):
        x_err = tgraph.GetErrorX(ibin)
        tgraph.SetPointError(ibin, x_err, x_err, sqrt(total_errs_down[ibin]), sqrt(total_errs_up[ibin]))

    return tgraph

def divide_tgraph_asymm_errs(tgraph_num, tgraph_den):
    assert(tgraph_num.GetN() == tgraph_den.GetN())

    NUM_POINTS = tgraph_num.GetN()

    tgraph_ratio = TGraphAsymmErrors(NUM_POINTS)

    for ibin in range(NUM_POINTS):
        num_valX, num_valY = Double(0), Double(0)
        den_valX, den_valY = Double(0), Double(0)

        tgraph_num.GetPoint(ibin, num_valX, num_valY)
        tgraph_den.GetPoint(ibin, den_valX, den_valY)

        assert(num_valX == den_valX)

        num_exl = tgraph_num.GetErrorXlow(ibin)
        num_exh = tgraph_num.GetErrorXhigh(ibin)
        den_exl = tgraph_den.GetErrorXlow(ibin)
        den_exh = tgraph_den.GetErrorXhigh(ibin)

        # the assumption is that we are working with tgraphs with only
        # the Y errors varying, and this check is only to ensure that this is actually true
        assert(num_exl == num_exh == den_exl == den_exh)

        num_eyl = tgraph_num.GetErrorYlow(ibin)
        num_eyh = tgraph_num.GetErrorYhigh(ibin)
        den_eyl = tgraph_den.GetErrorYlow(ibin)
        den_eyh = tgraph_den.GetErrorYhigh(ibin)

        tgraph_num.GetPoint(ibin, num_valX, num_valY)
        tgraph_den.GetPoint(ibin, den_valX, den_valY)

        if (den_valY == 0 and num_valY == 0):
            ratio_valY = -1000
        else:
            ratio_valY = num_valY / den_valY

        ratio_eyl = 0
        ratio_eyh = 0

        if (num_valY != 0 and den_valY != 0):
            ratio_eyl = sqrt( ratio_valY**2 * ( (num_eyl / num_valY)**2 + (den_eyl / den_valY)**2 ) )
            ratio_eyh = sqrt( ratio_valY**2 * ( (num_eyh / num_valY)**2 + (den_eyh / den_valY)**2 ) )
        else:
            ratio_eyl = 99
            ratio_eyh = 99

        ratio_exl = num_exl
        ratio_exh = num_exh

        # symmetrize (for now)
        ratio_y_error = max(ratio_eyl, ratio_eyh)

        tgraph_ratio.SetPoint(ibin, num_valX, ratio_valY)

        tgraph_ratio.SetPointError(ibin, ratio_exl, ratio_exh, ratio_y_error, ratio_y_error)

    return tgraph_ratio
