#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include <TChain.h>

#include "VVJJSelector.h"

int
main(int argc, char** argv)
{
    std::string input_path = argv[1];
    std::string output_path = argv[2];

    // load the input file
    std::ifstream input_file(input_path.c_str(), std::ifstream::in);
    if (!input_file.is_open()) {
        std::cout << "ERROR: failed to open input file list: " << input_path << std::endl;
        return EXIT_FAILURE;
    }

    // A map from generator names to vectors of ntuple filepaths that share that generator name.
    // "generator name" = 'data', 'pythia', 'herwig', 'powheg_pythia', etc
    std::unordered_map< std::string, std::vector<std::string> > ntuple_filepath_map;

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
    std::unordered_map<std::string, TChain*> tchains;
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
                return EXIT_FAILURE;
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
