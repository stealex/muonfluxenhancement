# MuonFluxEnhancement


[![DOI](https://zenodo.org/badge/469697797.svg)](https://zenodo.org/badge/latestdoi/469697797)


Sample application implementing the algorithm described in [Eur. Phys. J. C 81, 767 (2021)](https://doi.org/10.1140/epjc/s10052-021-09541-7)
Original repository here: https://gitlab.cern.ch/sghinesc/muonfluxenhancement

The geometry is a simplified version of the NA62 TAX, consisting of two blocks of copper followed by 6 blocks of iron of the same length. The total length is appxoimately 3.2 meters.

Protons of 400 GeV/c are shot with a pencil beam in the middle of the transverse entrance plane of the first block and the use can select whether to enhance (bias) the muon flux resulting from the shower.

## Installation instructions
The setup has been tested only on `CentOS 7` machines with `CVMFS` access.

1. Clone the repository and `cd` to the top directory
2. `$ source env.sh`
3. `make -j N` from the top directory


## Running the application
`./build-cc7/Hadr01 macros/hadr01.in <seed>`. Read the macro file for more information 

**Only muons with track type == 1 are to be used** after any kind of biased run, regardless of the output tree.
