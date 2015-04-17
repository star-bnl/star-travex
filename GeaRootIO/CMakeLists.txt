root_generate_dictionary(
	GeaRootIO_Cint
	TGeaEvent.h
	LINKDEF LinkDef.h
)

add_library(
	GeaRootIO
	SHARED
	TGeaEvent.cxx
	GeaRootIO_Cint.cxx
)

install(TARGETS GeaRootIO DESTINATION ".$ENV{STAR_HOST_SYS}/lib")


root_generate_dictionary(
	AgUStep_Cint
	AgUStep.h
	LINKDEF AgUStepLinkDef.h
)

add_library(
	AgUStep
	SHARED
	AgUStep.cxx
	AgUStep_Cint.cxx
)

install(TARGETS AgUStep DESTINATION ".$ENV{STAR_HOST_SYS}/lib")
