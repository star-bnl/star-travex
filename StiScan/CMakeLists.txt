root_generate_dictionary(
	StiScan_Cint
	StiScanEvent.h
	StiScanHftTreeMaker.h
	LINKDEF LinkDef.h
)

add_library(
	StiScan
	SHARED
	StiScanEvent.cxx
	StiScanHftTreeMaker.cxx
	StiScanHistContainer.cxx
	StiScanHistsByVolume.cxx
	StiScanPrgOptions.cxx
	StiScanRootFile.cxx
	StiScan_Cint.cxx
)

install(TARGETS StiScan DESTINATION ".$ENV{STAR_HOST_SYS}/lib")
