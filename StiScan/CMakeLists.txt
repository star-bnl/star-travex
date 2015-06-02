root_generate_dictionary(
	StiScan_Cint
	StiScanEvent.h
	StiScanTreeMaker.h
	LINKDEF StiScanLinkDef.h
)

add_library(
	StiScan
	SHARED
	StiScanEvent.cxx
	StiScanTreeMaker.cxx
	StiScanHistContainer.cxx
	StiScanRatiosHistContainer.cxx
	StiScanHistsByVolume.cxx
	StiScanPrgOptions.cxx
	StiScanRootFile.cxx
	StiScan_Cint.cxx
)

install(TARGETS StiScan DESTINATION ".$ENV{STAR_HOST_SYS}/lib")
