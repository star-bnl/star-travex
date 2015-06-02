StiHify tool and library
========================

To build the library and the executable follow the installation instructions in
the project's README.md. Once the `StiHify` library is built it can be loaded
and the `StiHifyTreeMaker` can be added to the reconstruction chain by using the
`StiHifyTreeMaker` option as:

    root4star -b -q -l 'bfc.C(<first_event>, <last_event>, "<your_bfc_options> StiHifyTreeMaker", "<input_file>")'


Reconstruction chain examples
=============================

Note that in the following example the PXL hits are excluded from tracking
by using the `NoPxlIT`option.

    root4star -q -b -l 'bfc.C(1, 100, "P2014a mtd btof pxlHit istHit NoPxlIT BEmcChkStat CorrX OSpaceZ2 OGridLeak3D -hitfilt StiHifyTreeMaker", "st_physics_file.daq")'
