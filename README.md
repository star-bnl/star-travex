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
in `contrib/` subdirectory.


Build with cmake
----------------

In the following we assume that the user environment includes the standard shell
variables available for a typical user account of the STAR experiment. The
following environment variables can be set if needed:

    $STAR_LIB        # Used in supple/starsim_zslice_*.kumac
    $OPTSTAR         # The prefix path to installed boost release
    $STAR_HOST_SYS

Checkout the code using one of the following commands:

    git clone https://github.com/plexoos/star-sti-tools.git   # If you do not have an account on github.com
    git clone git@github.com:plexoos/star-sti-tools.git       # otherwise.

Compile and build the tools:

    cd star-sti-tools/
    git submodule update --init --depth=1
    mkdir build && cd build
    # See "Remark about C++11 at STAR" below
    cmake -D CMAKE_INSTALL_PREFIX=./ -D CMAKE_CXX_FLAGS="-m32" -D BOOST_ROOT=$OPTSTAR ../
    make -j4 && make install

*Remark about C++11 at STAR:* Our code extensively uses the features from the
C++11 standard and, therefore, a compiler with C++11 support is required to
compile it. As of May 2015 the default compiler in the STAR environment is
gcc-4.4.7 does not provide full support of C++11 but a newer version 4.8.2 is
also available. To use it just provide the following options to cmake:

    cmake -D CMAKE_INSTALL_PREFIX=".$STAR_HOST_SYS/" -D CMAKE_CXX_FLAGS="-m32" -D BOOST_ROOT=$OPTSTAR ../
    make -j4 && make install

The make tool will place the libraries in the local `.slXX_gccXXX` directory.


Build with cons within STAR environment
-------------------------------------

    starver dev
    git clone https://github.com/plexoos/star-sti-tools.git && cd star-sti-tools
    git submodule update --init --depth=1
    mkdir -p build-cons/StRoot && cd build-cons/StRoot
    ln -s ../../contrib/star-sti/Sti* ../../Sti* ../../GeaRootIO .
    cd ..
    cons


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
