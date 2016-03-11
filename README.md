[![Build Status](https://travis-ci.org/plexoos/star-sti-tools.svg?branch=master)](https://travis-ci.org/plexoos/star-sti-tools)

This package includes a collection of tools used by the STAR experiment to
evaluate reconstruction of charged particle tracks and vertices. The STAR
detector at the Relativistic Heavy Ion Collider (RHIC) in Brookhaven National
Laboratory is a multipurpose experiment designed to study properties of high and
medium energy nuclear collisions.

Although, the code in this package is mainly tested with and designed to support
reconstruction and analysis of the data collected by the STAR experiment we hope
other new or existing experiments will find it useful as well. This project can
also serve as an example for other implementations of track and vertex
evaluation toolkits who chose to save time and effort by building around
`travex`, a detector-independent suite for track and vertex reconstruction
evaluation [https://github.com/plexoos/travex].

The following individual tools are available:

- `stiscan` ([StiScan/README.md](StiScan/README.md)) is used to verify the
accuracy of the modeled detector material

- `stihify` ([StiHify/README.md](StiHify/README.md)) is used to measure the
efficiency of hits assigned to the track by a track reconstruction algorithm.
For single hit efficiency studies with `stihify` we developed a method
independent of the actual reconstruction algorithm used for finding particle
tracks.

- `vtxeval` is aimed to evaluate the performance of vertex finding algorithms.
Currently, the source code can be found in a separate repository
[https://github.com/star-bnl/star-vertex-eval] but we plan to transfer the code
into here, possibly renaming the entire project to `star-travex`.


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
    make -j4 && make install

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
