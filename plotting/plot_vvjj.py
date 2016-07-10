from ROOT import *
from math import *
import array as arr
import os

def get_vvjj_axis_title(histo_name):
    for file_title, tex_title in AXIS_TITLES.iteritems():
        if histo_name.startswith(file_title):
            return tex_title
    return ""

AXIS_TITLES = {
        "both_jet_pt"     : "Large-R Jet #it{p}_{T}",
        "first_jet_pt"    : "Leading Large-R Jet #it{p}_{T}",
        "second_jet_pt"   : "Subleading Large-R Jet #it{p}_{T}",
        "both_jet_eta"    : "Large-R Jet #it{#eta}",
        "first_jet_eta"   : "Leading Large-R Jet #it{#eta}",
        "second_jet_eta"  : "Subleading Large-R Jet #it{#eta}",
        "both_jet_phi"    : "Large-R Jet #it{#phi}",
        "first_jet_phi"   : "Leading Large-R Jet #it{#phi}",
        "second_jet_phi"  : "Subleading Large-R Jet #it{#phi}",
        "both_jet_m"      : "Large-R Jet Mass",
        "first_jet_m"     : "Leading Large-R Jet Mass",
        "second_jet_m"    : "Subleading Large-R Jet Mass",
        "both_jet_ntrk"   : "Large-R Jet #it{n}_{trk}",
        "first_jet_ntrk"  : "Leading Large-R Jet #it{n}_{trk}",
        "second_jet_ntrk" : "Subleading Large-R Jet #it{n}_{trk}",
        "both_jet_D2"     : "Large-R Jet #it{D}_{2}^{#beta=1}",
        "first_jet_D2"    : "Leading Large-R Jet #it{D}_{2}^{#beta=1}",
        "second_jet_D2"   : "Subleading Large-R Jet #it{D}_{2}^{#beta=1}",
        "dijet_mass"      : "m_{JJ}"
        }

def get_vvjj_selection_tex(var_name):
    tmp_tex = ""
    for selection_substr, tex_str in SELECTION_TEX.iteritems():
        if selection_substr in var_name and len(tex_str) > len(tmp_tex):
            tmp_tex = tex_str
    return tmp_tex

SELECTION_TEX = {
        "partial_ntrk"        : "Partial #it{n}_{trk} Selection",
        "WW_full"             : "WW Selection",
        "WW_partial_massD2"   : "WW Partial Selection: mass + #it{D}_{2}",
        "WW_partial_massNtrk" : "WW Partial Selection: mass + #it{n}_{trk}",
        "WW_partial_ntrkD2"   : "WW Partial Selection: #it{D}_{2} + #it{n}_{trk}",
        "WW_partial_mass"     : "WW Partial Selection: mass",
        "WW_partial_D2"       : "WW Partial Selection: #it{D}_{2}",
        "WZ_full"             : "WZ Selection",
        "WZ_partial_massD2"   : "WZ Partial Selection: mass + #it{D}_{2}",
        "WZ_partial_massNtrk" : "WZ Partial Selection: mass + #it{n}_{trk}",
        "WZ_partial_ntrkD2"   : "WZ Partial Selection: #it{D}_{2} + #it{n}_{trk}",
        "WZ_partial_mass"     : "WZ Partial Selection: mass",
        "WZ_partial_D2"       : "WZ Partial Selection: #it{D}_{2}",
        "ZZ_full"             : "ZZ Selection",
        "ZZ_partial_massD2"   : "ZZ Partial Selection: mass + #it{D}_{2}",
        "ZZ_partial_massNtrk" : "ZZ Partial Selection: mass + #it{n}_{trk}",
        "ZZ_partial_ntrkD2"   : "ZZ Partial Selection: #it{D}_{2} + #it{n}_{trk}",
        "ZZ_partial_mass"     : "ZZ Partial Selection: mass",
        "ZZ_partial_D2"       : "ZZ Partial Selection: #it{D}_{2}",
        "Z_full"              : "Z Selection",
        "Z_partial_massD2"    : "Z Partial Selection: mass + #it{D}_{2}",
        "Z_partial_massNtrk"  : "Z Partial Selection: mass + #it{n}_{trk}",
        "Z_partial_ntrkD2"    : "Z Partial Selection: #it{D}_{2} + #it{n}_{trk}",
        "Z_partial_mass"      : "Z Partial Selection: mass",
        "Z_partial_D2"        : "Z Partial Selection: #it{D}_{2}",
        "W_full"              : "W Selection",
        "W_partial_massD2"    : "W Partial Selection: mass + #it{D}_{2}",
        "W_partial_massNtrk"  : "W Partial Selection: mass + #it{n}_{trk}",
        "W_partial_ntrkD2"    : "W Partial Selection: #it{D}_{2} + #it{n}_{trk}",
        "W_partial_mass"      : "W Partial Selection: mass",
        "W_partial_D2"        : "W Partial Selection: #it{D}_{2}"
        }
