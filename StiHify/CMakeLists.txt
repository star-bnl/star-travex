root_generate_dictionary(
	StiHify_Cint
	StiHifyEvent.h
	StiHifyTreeMaker.h
	LINKDEF StiHifyLinkDef.h
)

add_library(
	StiHify
	SHARED
	StiHifyEvent.cxx
	StiHifyTreeMaker.cxx
	StiHifyHistContainer.cxx
	StiHifyRatiosHistContainer.cxx
	StiHifyRootFile.cxx
	StiHify_Cint.cxx
)

install(TARGETS StiHify DESTINATION ".$ENV{STAR_HOST_SYS}/lib")
