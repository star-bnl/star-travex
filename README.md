[![Build Status](https://travis-ci.org/plexoos/star-sti-tools.svg?branch=master)](https://travis-ci.org/plexoos/star-sti-tools)

This package includes a collection of tools which can be used to evaluate and
validate reconstruction of charged particle tracks measured by tracking
detectors. The main objective of this toolkit is to design and implement
a detector independent utilities that can be applied to most common detector
configurations and reconstruction frameworks used in particle physics
experiments.

Currently we provide two tools `stiscan`
([StiScan/README.md](StiScan/README.md)) and `stihify`
([StiHify/README.md](StiHify/README.md)):

- `stiscan` is used to verify the accuracy of the modeled detector material

- `stihify` is used to measure the efficiency of hits assigned to the track by a
track reconstruction algorithm. For single hit efficiency studies with `stihify`
we developed a method independent of the actual reconstruction algorithm used
for finding particle tracks.

The code in this package is mainly tested with and designed to support
reconstruction and analysis of the data collected by the STAR experiment at RHIC
but we hope other experiments will find it useful as well.


How to build and install libraries and tools
============================================


Prerequisites
-------------

- C++ compiler with C++11 support (e.g. g++ >= 4.8.2)
- ROOT (>= 5.34.30), http://root.cern.ch
- boost libraries (>= 1.54): `program_options`, `regex`, and `filesystem`
- Some modules from the STAR software library already included as dependencies
in `ext/` subdirectory.


Build with cmake
----------------

Checkout the code using one of the following commands:

    git clone https://github.com/plexoos/star-sti-tools.git

Compile and build the tools:

    cd star-sti-tools/
    git submodule update --init --depth=50
    mkdir build && cd build
    cmake ../
    make -j4 && make install

The make tool will place the libraries in the local `.slXX_gccXXX` directory.


Build within STAR environment
-----------------------------

We assume that the user environment includes the standard shell variables
available for a typical user account of the STAR experiment. The following
environment variables should be set if needed:

    $STAR_LIB        # Used in supple/starsim_zslice_*.kumac
    $OPTSTAR         # The prefix path to installed boost release

    git clone https://github.com/plexoos/star-sti-tools.git
    cd star-sti-tools/
    git submodule update --init --depth=50
    mkdir build && cd build
    cmake -D CMAKE_INSTALL_PREFIX=./ -D BOOST_ROOT=$OPTSTAR ../

The make tool will place the libraries in the local `.slXX_gccXXX` directory.


Regular expressions matching geometry volumes used at STAR
==========================================================

    Beam pipe                   ^.*IDSM_1/PIPI_1/PBES_1$
    Field cages                 ^.*TPCE_1/T[IO]FC.*$
    PXL all volumes             ^.*IDSM_1/PXMO_.*$
    PXL sensitive volumes       ^.*IDSM_1/PXMO_1/PXLA_[\d]+/LADR_\d/PXSI_[\d]+/PLAC_1.*$
    PXL ribs                    ^.*IDSM_1/PXMO_1/PXLA_[\d]+/PXRB.*$
                                ^.*IDSM_1/PXMO_1/PXLA_[\d]+/PXLB.*$
    IST all volumes             ^.*IDSM_1/IBMO_.*$
    IST sensitive volumes       ^.*IDSM_1/IBMO_1/IBAM_[\d]+/IBLM_\d/IBSS_1.*$
    SST all volumes             ^.*IDSM_1/SFMO_.*$
    SST sensitive volumes       ^.*IDSM_1/SFMO_1/SFLM_[\d]+/SFSW_[\d]+/SFSL_1/SFSD_1.*$
