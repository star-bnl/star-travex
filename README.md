[![Build Status](https://travis-ci.org/star-bnl/star-travex.svg?branch=master)](https://travis-ci.org/star-bnl/star-travex)

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

- `stihify` ([stihify/README.md](src/stihify/README.md)) is used to measure the
efficiency of hits assigned to the track by a track reconstruction algorithm.
For single hit efficiency studies with `stihify` we developed a method
independent of the actual reconstruction algorithm used for finding particle
tracks.

- `vtxhist` and `vtxreco` ([vtxeval/README.md](src/vtxeval/README.md)) are aimed to
evaluate the performance of vertex finding algorithms.

- `stiscan` ([stiscan/README.md](src/stiscan/README.md)) is used to verify the
accuracy of the modeled detector material.

Documentation gerated by `doxygen` can be found at <http://star-bnl.github.io/star-travex>


How to build and install the libraries and tools
================================================


Prerequisites
-------------

- C++ compiler with C++11 support (e.g. g++ >= 4.8.2)
- ROOT (>= 5.34.30), http://root.cern.ch
- boost libraries (>= 1.54): `program_options`, `regex`, and `filesystem`
- This project also depends on some modules from the STAR software library.
  These are already included as dependencies in the `ext/` subdirectory.


Build with cmake
----------------

Checkout the code using the following command:

    $ git clone --recursively https://github.com/star-bnl/star-travex.git

Compile and build the tools:

    $ cd star-travex/
    $ mkdir build && cd build
    $ cmake ../
    $ make -j4 && make install

If the environment variable `$STAR_HOST_SYS` is specified the binaries will be
placed in the `CMAKE_INSTALL_PREFIX/.$STAR_HOST_SYS` directory with
`CMAKE_INSTALL_PREFIX` being the current directory by default.


Build within STAR environment
-----------------------------

We assume that the user environment includes the shell variables availabe within
a typical STAR account. Specifically, $OPTSTAR contains the prefix path and is
used to help `cmake` find the appropriate `boost` release.

    $ git clone --recursively https://github.com/star-bnl/star-travex.git
    $ cd star-travex/
    $ mkdir build && cd build
    $ cmake -D CMAKE_INSTALL_PREFIX=./ \
            -D BOOST_ROOT=$OPTSTAR \
            -D CMAKE_BUILD_TYPE=RelWithDebInfo ../
    $ make -j4 && make install
