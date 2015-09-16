
The efficiency to pick up a hit in an active detector volume can be described
using the following components:

    P_measured = (P_instrumental + P_background) * P_corr * P_tracking

where

    P_instrumental (or P_detector) is essentially the probability for the
detector active media to produce a signal that can be recorded (Includes
efficiency of hardware and/or low level thresholds, pedestals, ...)

    P_background is the probability to have a hit within the window not
produced by the considered track (Includes detector noise, other tracks, ...)

    P_corr include cluster/hit reconstruction efficiency, uncorrected unknown
distortions, inefficiency of known corrected distortions

    P_tracking is the pure tracking efficiency, i.e. the probability to assign
a hit to the track by the tracking algo. Includes algo specific requirements
such as number of time hits can be used/reused, hit acceptance/rejection cuts
(chi^2), ...


StiHify tool and library
========================

To build the library and the executable follow the installation instructions in
the project's [README.md](../README.md). Once the `StiHify` library is built it
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
