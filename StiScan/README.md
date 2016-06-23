StiScan tool and library
========================

To build the library and the executable follow the installation instructions in
the project's [README.md](../README.md). Once the `StiScan` library is built it
can be loaded and the `StiScanTreeMaker` can be added to the reconstruction
chain by using the `StiScanTreeMaker` option as:

    root4star -b -q -l 'bfc.C(<first_event>, <last_event>, "<your_bfc_options> StiScanTreeMaker", "<input_file>")'

For a concrete example see the section below. The `bfc.C` macro can take either
real or simulated data as input but for our studies of the detector material
distribution we focus on specially simulated event samples with tracks
perpendicular to the colliding beams. Such configuration of tracks makes the
interpretation of the final results much easier. For reference, we include
a simple `starsim` kumac (`supple/starsim_zslice_single_z.kumac`) to produce
events with tracks in transverse planes at arbitrary integer `z` values. To use
this kumac from your build directory just do:

    export STISCAN_Z=7
    starsim -w 0 -b ../supple/starsim_zslice_single_z.kumac

We also include a number of support shell scripts to automate the generation of
such events for different `z` values by running condor jobs on the farm (see
below).

Another way to produce a file with tracks generated in `z=const` planes is to
run a special ROOT macro (`supple/starsim_stepper.C`):

    root4star -b -l -q '../supple/starsim_stepper.C(7,10,50)'

which in addition to a usual `starsim_zslice.fz` file will produce a ROOT file
(`starsim_zslice.track_history.root`) with a TTree containing the information
about the track as GEANT propagates it through the detector material. That file
can be used together with the `*.fz` one in `stiscan` in order to produce
comparisons between the geometries as seen by the simulated and reconstructed
tracks.

The `starsim` commands from above examples should produce a `starsim_zslise.fz`
file that can be processed with the `bfc.C` macro as indicated earlier. The
`StiScanTreeMaker` option creates a ROOT file (\*.stiscan.root) with a TTree
that can be used to study track losses in the detector volumes with the help of
`stiscan` program.  `stiscan` accepts either a ROOT file with an stiscan TTree
or a text file with a list of such ROOT files (one per line) as input. To
produce a set of basic histograms with track energy lost in Sti volumes one can
do:

    stiscan -f path/to/my.stiscan.root

For more options type `stiscan --help`.

By default, only sensitive layers of the PXL, IST, and SSD detectors are used in
the energy loss analysis. One can easily specify any other volume or a set of
volumes to be considered in the analysis by using regular PERL expressions. Here
are a few examples:

    stiscan -f path/to/my.stiscan.root -p "^.*IDSM_1/PXMO_1/PXLA_[\d]+/PXT[RML]_.*$" -g
    stiscan -f path/to/my_stiscan_list -l path/to/my_volume_name_pattern_list -s 0.10 -g

Either a regex pattern or a text file with a list of regex patterns
(`my_volume_name_pattern_list` in the above example) can be provided. It is used
to match the names of Sti/TGeo volumes and to select only these specific physical
volumes.

*Note:* The program expects to find a ROOT file (`y2014a.root`) with the full
STAR geometry in the directory from where it was started. Such file can be
produced with the following command:

    root -l '$STAR/StarVMC/Geometry/macros/viewStarGeometry.C("y2014a")'


How to produce and reconstruct massive simulation
=================================================

A couple of shell scripts are provided to simplify the submission of condor jobs
to the farm. The following commands can be used as examples:

    supple/submit_jobs_starsim_zslice.sh supple/job_template_starsim_zslice_kumac.xml
    supple/submit_jobs_starsim_zslice.sh supple/job_template_starsim_zslice_macro.xml

    supple/submit_jobs_stiscan.sh /path/to/filelist_fz

`submit_jobs_stiscan_zslice.sh` takes an xml template as the only parameter. In
our case the `starsim` command is executed to produce \*.fz files with simulated
events.

`submit_jobs_stiscan.sh` utilizes the `supple/job_template_stiscan.xml`
template to run over the \*.fz files specified in the input list. The jobs run
the standard reconstruction and in addition creates a ROOT file with an stiscan
tree. The latter can be controlled by changing the BFC options hard-coded in the
script.


Reconstruction chain examples
=============================

Run over an fzd file produced by `starsim` with the following options to produce
a MuDst.root file with simulated tracker response and reconstruced events.

    root4star -q -b -l 'bfc.C(1, 100, "fzin y2015a AgML usexgeom FieldOn tpcRS MakeEvent VFMinuit Sti BAna Idst NoSsdIt NoSvtIt StiHftC TpcHitMover TpxClu l0 Tree logger tpcDB tags emcY2 EEfs -dstout IdTruth big MiniMcMk clearmem StiScanTreeMaker", "starsim_zslice.fz")'
