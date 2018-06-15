stihify: Track Reconstruction Evaluation at STAR
================================================

Introduction
============

The efficiency to find a hit in an active detector volume can be described
using the following components:

    P_measured = (P_instrumental + P_background) * P_corr * P_tracking

where

- `P_instrumental` (aka `P_detector`) is the probability for the
detector active media to produce a signal that can be recorded (Includes
efficiency of hardware and/or low level thresholds, pedestals, etc)

- `P_background` is the probability to have a hit within the window that is not
produced by the considered track (for example detector noise, other tracks, etc)

- `P_corr` include cluster/hit reconstruction efficiency, uncorrected unknown
distortions, inefficiency of known corrected distortions

- `P_tracking` is the pure tracking efficiency, i.e. the probability to assign
a hit to the track by the tracking algo. Includes algo specific requirements
such as number of time hits can be used/reused, hit acceptance/rejection cuts
(chi^2), etc.


How to use
==========

To build the library and the executable follow the installation instructions in
the project's [README.md](../../README.md). Once the `stihifylib` library is built it
can be loaded and the `StiHifyTreeMaker` can be added to the reconstruction
chain by using the `StiHifyTreeMaker` option as:

    root4star -b -q -l 'bfc.C(<first_event>, <last_event>, "<your_bfc_options> StiHifyTreeMaker", "<input_file>")'

In order to disable a sensitive Sti volume (with hits) one can provide a file
called `deactivate_sti_detectors.txt` with a list of regex patterns matching the
name of the volume to skip in tracking. Providing this file is optional but
required if one would like to estimate the true single hit efficiency of
a detector.

By default, all tracks and track nodes will be saved in the output tree. This is
an unlikely desirable feature as it can lead to a very large output tree. To
limit the number of saved track and track nodes one can provide another text
file called `save_sti_detectors.txt` containing regex patterns matching the name
of the Sti volumes crossed by tracks. In the simplest case both the
`deactivate_sti_detectors.txt` and `save_sti_detectors.txt` files can contain
the same set of regex patterns.

Once the tree is produced one can run the `stihify` program over it to create
a set of histograms with hit efficiencies.


Reconstruction chain examples
=============================

Note that in the following example the PXL hits are excluded from tracking
by using the `NoPxlIT`option.

    root4star -q -b -l 'bfc.C(1, 100, "P2014a mtd btof pxlHit istHit NoPxlIT BEmcChkStat CorrX OSpaceZ2 OGridLeak3D -hitfilt StiHifyTreeMaker", "st_physics_file.daq")'

The following command is for the pretty much standard reconstruction with all
HFT hits used in tracking.

    root4star -q -b -l 'bfc.C(1, 100, "P2014a mtd btof pxlHit istHit sstHit BEmcChkStat CorrX OSpaceZ2 OGridLeak3D -hitfilt StiHifyTreeMaker", "st_physics_file.daq")'


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
