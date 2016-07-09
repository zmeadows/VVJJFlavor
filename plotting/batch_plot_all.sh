#!/bin/bash

RAW_FILEPATH=$1
BASE_NAME=$(basename $RAW_FILEPATH .root)
OUTPUT_ROOT_DIR="output/$BASE_NAME"

python batch_plot_qq_qg_gg_events_control_plots.py $RAW_FILEPATH $OUTPUT_ROOT_DIR
python batch_plot_qq_qg_gg_events_efficiency_plots.py $RAW_FILEPATH $OUTPUT_ROOT_DIR
python batch_plot_quark_gluon_jets_control_plots.py $RAW_FILEPATH $OUTPUT_ROOT_DIR
