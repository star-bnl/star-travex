root_generate_dictionary(
	StiScan_Cint
	H3D.h
	Profile2D.h
	Profile3D.h
	StiScanEvent.h
	StiScanHftTreeMaker.h
	LINKDEF LinkDef.h
)

add_library(
	StiScan
	SHARED
	H3D.cxx
	PrgOptionProcessor.cxx
	Profile2D.cxx
	Profile3D.cxx
	StiScanEvent.cxx
	StiScanHftTreeMaker.cxx
	StiScanHistContainer.cxx
	StiScanHistsByVolume.cxx
	StiScanPrgOptions.cxx
	StiScanRootFile.cxx
)

install(TARGETS StiScan DESTINATION ".$ENV{STAR_HOST_SYS}/lib")
