root_generate_dictionary(
	StiRootIO_Cint
	StiTreeMaker.h
	TStiEvent.h
	TStiHit.h
	TStiKalmanTrack.h
	TStiKalmanTrackNode.h
	LINKDEF LinkDef.h
)

add_library(
	StiRootIO
	SHARED
	StiTreeMaker.cxx
	TStiEvent.cxx
	TStiHit.cxx
	TStiKalmanTrack.cxx
	TStiKalmanTrackNode.cxx
	StiRootIO_Cint.cxx
)

install(TARGETS StiRootIO DESTINATION ".$ENV{STAR_HOST_SYS}/lib")
