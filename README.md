[![Build Status](https://travis-ci.org/plexoos/star-sti-tools.svg?branch=master)](https://travis-ci.org/plexoos/star-sti-tools)

This project offers a number of tools to support reconstruction and analysis of
the data collected by the STAR experiment at RHIC. The main objective is to
evaluate and validate the reconstruction of charged particle tracks measured by
the tracking detectors.

Currently we provide two tools `stiscan` ([StiScan/README.md](StiScan/README.md))
and `stihify` ([StiHify/README.md](StiHify/README.md)) the purpose of which is
to verify the accuracy of the modeled detector material and to measure the
efficiency of hits assigned during track reconstruction respectively.


How to build and install libraries and tools
============================================

The libraries and tools of star-sti-tools package significantly depend on ROOT
(http://root.cern.ch) and some select modules from the STAR software library.

...using cmake
--------------

In the following we assume that the user environment includes the standard shell
variables available for a typical user account of the STAR experiment. The
following environment variables can be set if needed:

    $STAR_LIB        # Used in supple/starsim_zslice_*.kumac
    $OPTSTAR
    $STAR_HOST_SYS

Checkout the code using one of the following commands:

    git clone https://github.com/plexoos/star-sti-tools.git   # If you do not have an account on github.com
    git clone git@github.com:plexoos/star-sti-tools.git       # otherwise.

Compile and build the tools:

    cd star-sti-tools/
    git clone --depth=1 --branch=ds-master https://github.com/star-bnl/star-db-calibrations.git contrib/star-db-calibrations
    git submodule update --init --depth=1
    mkdir build && cd build
    # See "Remark about C++11 at STAR" below
    cmake -DCMAKE_INSTALL_PREFIX=./ -DCMAKE_CXX_FLAGS="-m32 -std=c++0x" -DBOOST_ROOT=$OPTSTAR ../
    make -j4 && make install

*Remark about C++11 at STAR:* Our code extensively uses the features from the
C++11 standard and, therefore, a compiler with C++11 support is required to
compile it. As of May 2015 the default compiler in the STAR environment is
gcc-4.4.7 does not provide full support of C++11 but a newer version 4.8.2 is
also available. To use it just provide the following options to cmake:

    cmake -DCMAKE_CXX_COMPILER=/afs/rhic.bnl.gov/rcassoft/x8664_sl6/gcc482/bin/g++ \
          -DCMAKE_INSTALL_PREFIX=".$STAR_HOST_SYS/" \
          -DCMAKE_CXX_FLAGS="-m32 -std=c++0x" \
          -DBOOST_ROOT=$OPTSTAR ../
    export LD_LIBRARY_PATH+=":/afs/rhic.bnl.gov/rcassoft/x8664_sl6/gcc482/lib"
    make -j4 && make install

The make tool will place the libraries in the local `.slXX_gccXXX` directory.


...using cons within STAR environment
-------------------------------------

    starver dev
    export LD_LIBRARY_PATH+=":/afs/rhic.bnl.gov/rcassoft/x8664_sl6/gcc482/lib"
    git clone https://github.com/plexoos/star-sti-tools.git && cd star-sti-tools
    git submodule update --init --depth=1
    mkdir -p build-cons/StRoot && cd build-cons/StRoot
    ln -s ../../contrib/star-sti/Sti* ../../Sti* ../../GeaRootIO .
    cd ..
    cons "CXX=/afs/rhic.bnl.gov/rcassoft/x8664_sl6/gcc482/bin/g++"



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
