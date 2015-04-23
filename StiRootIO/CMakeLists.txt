root_generate_dictionary(
	StiRootIO_Cint
	H3D.h
	Profile2D.h
	Profile3D.h
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
	H3D.cxx
	PrgOptionProcessor.cxx
	Profile2D.cxx
	Profile3D.cxx
	StiTreeMaker.cxx
	TStiEvent.cxx
	TStiHit.cxx
	TStiKalmanTrack.cxx
	TStiKalmanTrackNode.cxx
	StiRootIO_Cint.cxx
)

install(TARGETS StiRootIO DESTINATION ".$ENV{STAR_HOST_SYS}/lib")
